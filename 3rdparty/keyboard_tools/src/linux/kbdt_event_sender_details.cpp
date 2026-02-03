#include <kbdt_details.hpp>

#include <cstring>
#include <string>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/uinput.h>
#include <linux/input-event-codes.h>

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

#define UINPUT_NAME "JC Keyboard Tools Uinput"

class KbdUInput
{
public:
    KbdUInput() = default;

    KbdUInput(const std::string& uinputName)
    {
        setup(uinputName);
    }

    ~KbdUInput()
    {
        unsetup();
    }

    bool setup(const std::string& uinputName)
    {
        if (isSetup())
            return false;

        constexpr const char* uinputFilename = "/dev/uinput";
        fd_ = open(uinputFilename, O_WRONLY | O_NONBLOCK);
        if (fd_ == -1)
            return false;

        struct uinput_setup usetup = {0};
        usetup.id.bustype = BUS_USB;
        strcpy(usetup.name, uinputName.c_str());
        ioctl(fd_, UI_DEV_SETUP, &usetup);

        ioctl(fd_, UI_SET_EVBIT, EV_KEY);
        for (size_t i = 0; i < KEY_MAX; ++i)
            ioctl(fd_, UI_SET_KEYBIT, i);

        ioctl(fd_, UI_DEV_CREATE);

        return true;
    }

    bool unsetup()
    {
        if (isSetup())
        {
            ioctl(fd_, UI_DEV_DESTROY);
            close(fd_);
            return true;
        }
        return false;
    }

    int fd() const
    {
        return fd_;
    }

    bool isSetup() const
    {
        return fd_ != -1;
    }

    size_t sendEvents(const std::vector<KeyEvent>& events)
    {
        if (!isSetup())
            return 0;

        static struct input_event syncIe = createSyncEvent();

        size_t sent = 0;
        struct input_event ies[2] = {0};
        for (size_t i = 0; i < events.size(); ++i)
        {
            keyEventToInputEvent(events[i], ies[0]);
            ies[1] = syncIe;
            constexpr size_t inputEventPairSize = 2 * sizeof(struct input_event);
            ssize_t wsize = write(fd_, ies, inputEventPairSize);
            if (wsize == inputEventPairSize)
                sent++;
        }

        return sent;
    }

private:
    static struct input_event createSyncEvent()
    {
        struct input_event ev = {0};

        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        ev.time.tv_sec = 0;
        ev.time.tv_usec = 0;

        return ev;
    }

    int fd_ = -1;
};

static KbdUInput uinput(UINPUT_NAME);

size_t sendEvents(const std::vector<KeyEvent>& events)
{
    return uinput.sendEvents(events);
}

} // namespace details

} // namespace kbdt
