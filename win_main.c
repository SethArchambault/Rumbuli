#include<windows.h>

void platform_setScreenSize(int * screen_width, int * screen_height) {
    // @Todo: get correct screen_height * width
    * screen_height = 100;
    * screen_width = 100;
}

void platform_setRandomSeed() {
    // @Todo: get random seed
}
void platform_sleep(int micro_seconds) {
    Sleep(micro_seconds/100000);
}

#include<main.c>

