#include "main.h"
#include<windows.h>

void platform_setScreenSize(int * screen_width, int * screen_height) {
    // @Todo: get correct screen_height * width
  
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void platform_setRandomSeed() {
    // @Todo: get random seed
}
void platform_sleep(int micro_seconds) {
    Sleep(micro_seconds/100000);
}

#include "main.c"

