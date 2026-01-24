#ifndef KBDT_LINUX_DEFS_H
#define KBDT_LINUX_DEFS_H

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 256
#endif // !BUFFER_SIZE

#define EVDEV_DIR       "/dev/input/"
#define UINPUT_FILE     "/dev/uinput"
#define UINPUT_NAME     "JC Keyboard Tool Uinput"

char* global_buffer();

#endif // !KBDT_LINUX_DEFS_H
