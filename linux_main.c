#include "main.h"
#include <sys/ioctl.h>
#include <unistd.h>

void platform_setScreenSize(int * screen_width, int * screen_height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *screen_height = w.ws_row;
    *screen_width = w.ws_col;
}
void platform_sleep(int micro_seconds) {
    usleep(micro_seconds);
}
void platform_resetCursor(int y, int x) {
    printf("\033[%d;%dH", y,x);
}

#include"main.c"
