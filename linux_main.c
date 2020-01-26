#include "main.h"
#include <sys/ioctl.h> // screen_size
#include <unistd.h>

void platform_set_screen_size(int * screen_width, int * screen_height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *screen_height = w.ws_row;
    *screen_width = w.ws_col;
}
void platform_sleep(int micro_seconds) {
    usleep(micro_seconds);
}
void platform_reset_cursor(int y, int x) {
    printf("\033[%d;%dH", y,x);
}
void platform_hide_cursor() {
}
void platform_show_cursor() {
}

#include"main.c"
