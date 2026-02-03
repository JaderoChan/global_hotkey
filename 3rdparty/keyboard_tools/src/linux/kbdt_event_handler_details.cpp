#include <kbdt_details.hpp>

#include <string>
#include <atomic>
#include <thread>

#include <errno.h>
#include <dirent.h>         // dirent
#include <fcntl.h>          // open
#include <unistd.h>         // read, write, close
#include <poll.h>           // poll
#include <sys/eventfd.h>    // eventfd
#include <sys/inotify.h>    // inotify
#include <sys/stat.h>       // stat
#include <linux/input.h>    // input_event

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

#define EVDEV_DIR "/dev/input/"

#define WORK_ET_END         1
#define WORK_ET_SET_HANDLER 2

static KeyEventHandler eventHandler = nullptr;

static std::atomic<int64_t> workEventData{-1};

static std::vector<struct pollfd> pollFds(2);  // Reserve 2 pollfd for 'workEventFd' and 'inotifyFd'.
static std::vector<std::string> evdevNames;

static bool isCharacterDevice(const std::string& filename)
{
    struct stat st;
    if (stat(filename.c_str(), &st) == -1)
        return false;
    return S_ISCHR(st.st_mode);
}

static bool isKeyboardDevice(int fd)
{
    unsigned long evBis = 0;
    ioctl(fd, EVIOCGBIT(0, sizeof(evBis)), &evBis);
    if ((evBis & (1 << EV_KEY)) == 0)
        return false;

    char keyBits[KEY_MAX / 8 + 1];
    ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits);
    int checkedKeys[] = {
        KEY_0, KEY_A, KEY_SPACE, KEY_ESC
    };
    size_t checkedCount = sizeof(checkedKeys) / sizeof(int);
    for (size_t i = 0; i < checkedCount; ++i)
    {
        if ((keyBits[checkedKeys[i] / 8] & (1 << checkedKeys[i] % 8)) == 0)
            return false;
    }
    return true;
}

static int emitWorkEvent(int64_t eventType, int64_t eventData)
{
    workEventData = eventData;
    ssize_t wsize = write(pollFds[0].fd, &eventType, 8);
    if (wsize != 8)
        return errno;
    return KBDT_RC_SUCCESS;
}

static int setupWorkEventFd()
{
    int workEventFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (workEventFd == -1)
        return errno;

    struct pollfd pollFd;
    pollFd.fd = workEventFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;
    pollFds[0] = pollFd;

    return KBDT_RC_SUCCESS;
}

static void unsetupWorkEventFd()
{
    close(pollFds[0].fd);
    pollFds[0].fd = -1;
}

static int setupInotifyFd()
{
    int inotifyFd = inotify_init();
    if (inotifyFd == -1)
        return errno;

    int wd = inotify_add_watch(inotifyFd, EVDEV_DIR, IN_CREATE | IN_DELETE);
    if (wd == -1)
    {
        close(inotifyFd);
        return errno;
    }

    struct pollfd pollFd;
    pollFd.fd = inotifyFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;
    pollFds[1] = pollFd;

    return KBDT_RC_SUCCESS;
}

static void unsetupInotifyFd()
{
    close(pollFds[1].fd);
    pollFds[1].fd = -1;
}

static void addEvdevFd(const std::string& evdevName)
{
    std::string filename = EVDEV_DIR + evdevName;
    printf("Add evdev Fd: %s\n", filename.c_str());

    if (!isCharacterDevice(filename))
        return;

    int evdevFd = open(filename.c_str(), O_RDONLY | O_NONBLOCK);
    if (evdevFd == -1)
        return;

    if (!isKeyboardDevice(evdevFd))
        return;

    struct pollfd pollFd;
    pollFd.fd = evdevFd;
    pollFd.events = POLLIN;
    pollFd.revents = 0;

    pollFds.push_back(pollFd);
    evdevNames.push_back(evdevName);
}

static void removeEvdevFd(const std::string& evdevName)
{
    for (size_t i = 0; i < evdevNames.size(); ++i)
    {
        if (evdevName == evdevNames[i])
        {
            int idx = i + 2;
            close(pollFds[idx].fd);
            pollFds.erase(pollFds.begin() + idx);
            evdevNames.erase(evdevNames.begin() + i);
            return;
        }
    }
}

static int setupEvdevFds()
{
    DIR* dir = opendir(EVDEV_DIR);
    struct dirent* ent = readdir(dir);
    while (ent)
    {
        addEvdevFd(ent->d_name);
        ent = readdir(dir);
    }
    closedir(dir);

    return KBDT_RC_SUCCESS;
}

static void unsetupEvdevFds()
{
    for (auto it = pollFds.begin() + 2; it != pollFds.end();)
    {
        close(it->fd);
        it = pollFds.erase(it);
    }
    evdevNames.clear();
}

static int setups()
{
    int rc = setupWorkEventFd();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    rc = setupInotifyFd();
    if (rc != KBDT_RC_SUCCESS)
    {
        unsetupWorkEventFd();
        return rc;
    }

    rc = setupEvdevFds();
    if (rc != KBDT_RC_SUCCESS)
    {
        unsetupWorkEventFd();
        unsetupInotifyFd();
        return rc;
    }

    return KBDT_RC_SUCCESS;
}

static void unsetups()
{
    unsetupWorkEventFd();
    unsetupInotifyFd();
    unsetupEvdevFds();

    eventHandler = nullptr;
    workEventData = -1;
}

constexpr int fdEventBufferSize = 255;
static char fdEventBuffer[fdEventBufferSize] = {0};

static void handleDevicesChanged()
{
    ssize_t rsize = read(pollFds[1].fd, fdEventBuffer, fdEventBufferSize);
    if (rsize <= 0)
        return;

    char* p = fdEventBuffer;
    while (p < fdEventBuffer + rsize)
    {
        struct inotify_event* ev = (struct inotify_event*) p;
        printf("Evdev Changed: %s\n", ev->name);
        if (ev->mask == IN_CREATE && ev->len > 0)
            addEvdevFd(ev->name);
        else if (ev->mask == IN_DELETE && ev->len > 0)
            removeEvdevFd(ev->name);
        p += sizeof(struct inotify_event) + ev->len;
    }
}

static void handleInputEvent(int evdevFd)
{
    ssize_t rsize = read(evdevFd, fdEventBuffer, fdEventBufferSize);
    if (rsize <= 0)
        return;

    if (!eventHandler)
        return;

    char* p = fdEventBuffer;
    KeyEvent event;
    while (p < fdEventBuffer + rsize)
    {
        struct input_event* ie = (struct input_event*) p;
        if (ie->type == EV_SYN)
        {
            eventHandler(event);
            p += sizeof(struct input_event);
            continue;
        }
        else if (ie->type == EV_KEY)
        {
            keyEventFromInputEvent(event, *ie);
        }

        p += sizeof(struct input_event);
    }
}

int initialize()
{
    return setups();
}

int finalize()
{
    return emitWorkEvent(WORK_ET_END, 0);
}

int setEventHandler(KeyEventHandler handler)
{
    return emitWorkEvent(WORK_ET_SET_HANDLER, (int64_t) handler);
}

void work()
{
    setRunSuccess();
    bool shouldClose = false;
    while (!shouldClose)
    {
        int ret = poll(pollFds.data(), pollFds.size(), -1);
        if (ret == -1)
            continue;

        // Handle work event.
        struct pollfd pollFd = pollFds[0];
        if (pollFd.revents & POLLIN)
        {
            int64_t et;
            ssize_t rsize = read(pollFd.fd, &et, 8);
            if (rsize == 8)
            {
                switch (et)
                {
                    case WORK_ET_END:
                        shouldClose = true;
                        break;
                    case WORK_ET_SET_HANDLER:
                        eventHandler = (KeyEventHandler) workEventData.load();
                        break;
                    default:
                        break;
                }
            }
        }

        // Handle input devices changed event.
        pollFd = pollFds[1];
        if (pollFd.revents & POLLIN)
            handleDevicesChanged();

        // Handle input devices events.
        for (size_t i = 2; i < pollFds.size(); ++i)
        {
            pollFd = pollFds[i];
            if (pollFd.revents & POLLIN)
                handleInputEvent(pollFd.fd);
        }
    }

    unsetups();
}

} // namespace details

} // namespace kbdt
