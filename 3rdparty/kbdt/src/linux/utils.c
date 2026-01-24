#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "defs.h"
#include "uinput_id.h"

bool is_character_device(const char* filename)
{
    struct stat st;
    if (stat(filename, &st) == -1)
        return false;
    return S_ISCHR(st.st_mode);
}

bool is_keyboard_device(int fd)
{
    unsigned long ev_bits = 0;
    ioctl(fd, EVIOCGBIT(0, sizeof(ev_bits)), &ev_bits);
    if ((ev_bits & (1 << EV_KEY)) == 0)
        return false;

    char key_bits[KEY_MAX / 8 + 1];
    ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(key_bits)), key_bits);
    int requested_keys[] = {KEY_0, KEY_A, KEY_SPACE, KEY_ESC};
    size_t requested_size = sizeof(requested_keys) / sizeof(int);
    for (size_t i = 0; i < requested_size; ++i)
    {
        if ((key_bits[requested_keys[i] / 8] & (1 << requested_keys[i] % 8)) == 0)
            return false;
    }
    return true;
}

bool is_self_uinput(int fd)
{
    char* buf = global_buffer();
    if (ioctl(fd, EVIOCGNAME(BUFFER_SIZE), buf) == -1)
        return false;

    bool is_same_name = strcmp(UINPUT_NAME, buf) == 0;
    if (!is_same_name)
        return false;

    struct input_id id;
    if (ioctl(fd, EVIOCGID, &id) == -1)
        return false;
    uint32_t uid;
    uid |= (uint32_t) id.product << 16;
    uid |= (uint32_t) id.version;
    bool is_same_id = uid == uinput_id();
    return is_same_id;
}

void set_usetup_id(struct uinput_setup* usetup)
{
    usetup->id.product = (short) (uinput_id() >> 16);
    usetup->id.version = (short) (uinput_id());
}

int setup_keyboard_uinput()
{
    int fd = open(UINPUT_FILE, O_WRONLY | O_NONBLOCK);
    if (fd == -1)
        return fd;

    struct uinput_setup usetup = {0};
    usetup.id.bustype = BUS_USB;
    set_usetup_id(&usetup);
    strcpy(usetup.name, UINPUT_NAME);
    ioctl(fd, UI_DEV_SETUP, &usetup);

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    for (size_t i = 0; i < KEY_MAX; ++i)
        ioctl(fd, UI_SET_KEYBIT, i);

    ioctl(fd, UI_DEV_CREATE);

    return fd;
}

void unsetup_keyboard_uinput(int fd)
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

char* copy_str_to_new(const char* str)
{
    size_t size = strlen(str) + 1;
    char* newstr = malloc(size);
    memcpy(newstr, str, size);
    return newstr;
}
