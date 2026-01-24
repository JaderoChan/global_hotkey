#ifndef KBDT_LINUX_UTILS_H
#define KBDT_LINUX_UTILS_H

#include <stdbool.h>

#include <linux/uinput.h>

bool is_character_device(const char* filename);

bool is_keyboard_device(int fd);

bool is_self_uinput(int fd);

void set_usetup_id(struct uinput_setup* usetup);

int setup_keyboard_uinput();

void unsetup_keyboard_uinput(int fd);

char* copy_str_to_new(const char* str);

#endif // !KBDT_LINUX_UTILS_H
