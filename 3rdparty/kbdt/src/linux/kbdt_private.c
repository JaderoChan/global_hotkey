#include <kbdt_private.h>

#include <stdbool.h>        // bool
#include <stdint.h>         // int64_t
#include <string.h>         // strcat, strcmp
#include <stdlib.h>         // free

#include <errno.h>
#include <dirent.h>         // dirent
#include <fcntl.h>          // open
#include <linux/input.h>    // input_event
#include <poll.h>           // poll
#include <pthread.h>        // pthread_mutex
#include <sys/eventfd.h>    // eventfd
#include <sys/inotify.h>    // inotify
#include <unistd.h>         // read, write, close

#include "container.h"
#include "defs.h"
#include "event_converter.h"
#include "utils.h"

static pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
#define LOCK(mtx)   pthread_mutex_lock(mtx)
#define UNLOCK(mtx) pthread_mutex_unlock(mtx)

static int g_uinput_fd;

static keyboard_event_handler g_handler = NULL;

#define ET_END          1
#define ET_SET_HANDLER  2
static int g_event_fd;
static int64_t g_event_data;

static int g_inotify_fd;

static struct container g_pollfds;
static struct container g_evdev_names;

static int emit_event(int event_type, int64_t data);

static void try_add_evdev_fd(const char* evdev_name);
static void remove_evdev_fd(const char* evdev_name);

static int setup_uinput_fd();
static int setup_event_fd();
static int setup_inotify_fd();
static int setup_evdev_fds();

static void unsetup_uinput_fd();
static void unsetup_event_fd();
static void unsetup_inotify_fd();
static void unsetup_evdev_fds();

static int init();
static void clear();

static void handle_devices_changed();
static void handle_input_event(int evdev_index);

int do_before_start_work()
{
    return init();
}

int do_before_end_work()
{
    return emit_event(ET_END, 0);
}

void work()
{
    set_run_success();
    bool should_close = false;
    while (!should_close)
    {
        int ret = poll((struct pollfd*) g_pollfds.data, g_pollfds.size, -1);
        if (ret == -1)
            continue;

        // My event was detected.
        struct pollfd* pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, 0);
        if (pollfd->revents & POLLIN)
        {
            int64_t et;
            ssize_t rdsize = read(g_event_fd, &et, 8);
            if (rdsize == 8)
            {
                switch (et)
                {
                    case ET_END:
                        should_close = true;
                        break;
                    case ET_SET_HANDLER:
                        LOCK(&g_mtx);
                        g_handler = (keyboard_event_handler) g_event_data;
                        UNLOCK(&g_mtx);
                        break;
                    default:
                        break;
                }
            }
        }

        // The input devices has changed.
        pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, 1);
        if (pollfd->revents & POLLIN)
            handle_devices_changed();

        // Is input devices has event?
        for (size_t i = g_pollfds.size - g_evdev_names.size; i < g_pollfds.size; ++i)
        {
            pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, i);
            if (pollfd->revents & POLLIN)
                handle_input_event(i - (g_pollfds.size - g_evdev_names.size));
        }
    }

    clear();
}

int set_event_handler_p(keyboard_event_handler handler)
{
    return emit_event(ET_SET_HANDLER, (int64_t) handler);
}

size_t send_events_p(struct keyboard_event* events, size_t event_count)
{
    static struct input_event syn_ie = {
        .type = EV_SYN,
        .code = SYN_REPORT,
        .value = 0,
        .time.tv_sec = 0,
        .time.tv_usec = 0
    };

    char* buf = global_buffer();
    size_t sent = 0;
    for (size_t i = 0; i < event_count; ++i)
    {
        struct keyboard_event* event = &events[i];
        struct input_event* ie = (struct input_event*) buf;
        keyboard_event_to_input_event(event, ie);
        ie++;
        *ie = syn_ie;
        static size_t size = 2 * sizeof(struct input_event);
        ssize_t wrsize = write(g_uinput_fd, buf, size);
        if (wrsize == size)
            sent++;
    }
    return sent;
}

int emit_event(int event_type, int64_t data)
{
    int64_t et = event_type;
    LOCK(&g_mtx);
    g_event_data = data;
    UNLOCK(&g_mtx);
    ssize_t wrsize = write(g_event_fd, &et, 8);
    if (wrsize != 8)
        return errno;
    return KBDT_RC_SUCCESS;
}

void try_add_evdev_fd(const char* evdev_name)
{
    char* strbuf = global_buffer();
    strbuf[0] = '\0';
    strcat(strbuf, EVDEV_DIR);
    strcat(strbuf, evdev_name);

    if (!is_character_device(strbuf))
        return;

    int evdev_fd = open(strbuf, O_RDONLY | O_NONBLOCK);
    if (evdev_fd == -1)
        return;

    if (is_self_uinput(evdev_fd))
        return;

    if (!is_keyboard_device(evdev_fd))
        return;

    struct pollfd pollfd;
    pollfd.fd = evdev_fd;
    pollfd.events = POLLIN;
    pollfd.revents = 0;
    add_item_to_container(&g_pollfds, &pollfd);
    char* name = copy_str_to_new(evdev_name);
    add_item_to_container(&g_evdev_names, &name);
}

void remove_evdev_fd(const char* evdev_name)
{
    for (size_t i = 0; i < g_evdev_names.size; ++i)
    {
        char* name = *(char**) get_item_in_container(&g_evdev_names, i);
        if (strcmp(name, evdev_name) == 0)
        {
            size_t pollfd_i = g_pollfds.size - g_evdev_names.size + i;
            struct pollfd* pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, pollfd_i);
            close(pollfd->fd);
            remove_item_in_container(&g_pollfds, pollfd_i);
            free(name);
            remove_item_in_container(&g_evdev_names, i);
            return;
        }
    }
}

int setup_uinput_fd()
{
    g_uinput_fd = setup_keyboard_uinput();
    if (g_uinput_fd == -1)
        return errno;
    return KBDT_RC_SUCCESS;
}

int setup_event_fd()
{
    g_event_fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (g_event_fd == -1)
        return errno;
    struct pollfd pollfd;
    pollfd.fd = g_event_fd;
    pollfd.events = POLLIN;
    pollfd.revents = 0;
    add_item_to_container(&g_pollfds, &pollfd);
    return KBDT_RC_SUCCESS;
}

int setup_inotify_fd()
{
    g_inotify_fd = inotify_init();
    if (g_inotify_fd == -1)
        return errno;

    int wd = inotify_add_watch(g_inotify_fd, EVDEV_DIR, IN_CREATE | IN_DELETE);
    if (wd == -1)
    {
        close(g_inotify_fd);
        return errno;
    }

    struct pollfd pollfd;
    pollfd.fd = g_inotify_fd;
    pollfd.events = POLLIN;
    pollfd.revents = 0;
    add_item_to_container(&g_pollfds, &pollfd);
    return KBDT_RC_SUCCESS;
}

int setup_evdev_fds()
{
    DIR* dir = opendir(EVDEV_DIR);
    struct dirent* ent = readdir(dir);
    while (ent)
    {
        try_add_evdev_fd(ent->d_name);
        ent = readdir(dir);
    }
    closedir(dir);
    return KBDT_RC_SUCCESS;
}

void unsetup_uinput_fd()
{
    unsetup_keyboard_uinput(g_uinput_fd);
}

void unsetup_event_fd()
{
    close(g_event_fd);
}

void unsetup_inotify_fd()
{
    close(g_inotify_fd);
}

void unsetup_evdev_fds()
{
    for (size_t i = 0; i < g_evdev_names.size; ++i)
    {
        size_t pollfd_i = g_pollfds.size - g_evdev_names.size + i;
        struct pollfd* pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, pollfd_i);
        close(pollfd->fd);
        char* name = *(char**) get_item_in_container(&g_evdev_names, i);
        free(name);
    }
}

int init()
{
    construct_container(&g_pollfds, sizeof(struct pollfd), 0);
    construct_container(&g_evdev_names, sizeof(char*), 0);

    int rc = setup_event_fd();
    if (rc != KBDT_RC_SUCCESS)
        return rc;

    rc = setup_inotify_fd();
    if (rc != KBDT_RC_SUCCESS)
    {
        unsetup_event_fd();
        return rc;
    }

    rc = setup_uinput_fd();
    if (rc != KBDT_RC_SUCCESS)
    {
        unsetup_event_fd();
        unsetup_inotify_fd();
        return rc;
    }

    rc = setup_evdev_fds();
    if (rc != KBDT_RC_SUCCESS)
    {
        unsetup_event_fd();
        unsetup_inotify_fd();
        unsetup_uinput_fd();
        return rc;
    }

    return KBDT_RC_SUCCESS;
}

void clear()
{
    unsetup_event_fd();
    unsetup_inotify_fd();
    unsetup_uinput_fd();
    unsetup_evdev_fds();

    destroy_container(&g_pollfds);
    destroy_container(&g_evdev_names);
}

void handle_devices_changed()
{
    char* buf = global_buffer();
    ssize_t rdsize = read(g_inotify_fd, buf, BUFFER_SIZE);
    if (rdsize <= 0)
        return;

    char* p = buf;
    while (p < buf + rdsize)
    {
        struct inotify_event* ev = (struct inotify_event*) p;
        if (ev->mask == IN_CREATE && ev->len > 0)
            try_add_evdev_fd(ev->name);
        else if (ev->mask == IN_DELETE && ev->len > 0)
            remove_evdev_fd(ev->name);
        p += sizeof(struct inotify_event) + ev->len;
    }
}

void handle_input_event(int evdev_index)
{
    size_t pollfd_i = evdev_index + g_pollfds.size - g_evdev_names.size;
    struct pollfd* pollfd = (struct pollfd*) get_item_in_container(&g_pollfds, pollfd_i);
    int evdev_fd = pollfd->fd;
    char* buf = global_buffer();
    ssize_t rdsize = read(evdev_fd, buf, BUFFER_SIZE);
    if (rdsize <= 0)
        return;

    if (!g_handler)
        return;

    char* p = buf;
    struct keyboard_event event;
    while (p < buf + rdsize)
    {
        struct input_event* ie = (struct input_event*) p;
        if (ie->type == EV_SYN)
        {
            g_handler(&event);
            memset(&event, 0, sizeof(event));
            p += sizeof(struct input_event);
            continue;
        }
        else if (ie->type == EV_KEY)
        {
            keyboard_event_from_input_event(ie, &event);
        }

        p += sizeof(struct input_event);
    }
}
