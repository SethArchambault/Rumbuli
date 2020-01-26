#include "main.h"
#include<windows.h>

void platform_set_screen_size(int * screen_width, int * screen_height) {
    // @Todo: get correct screen_height * width
  
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *screen_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    *screen_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void platform_sleep(int micro_seconds) {
    Sleep(micro_seconds/100000);
}
void platform_reset_cursor(int y, int x) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
}
void platform_hide_cursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = 0;
    SetConsoleCursorInfo(out, &cursorInfo);
}
void platform_show_cursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = 1;
    SetConsoleCursorInfo(out, &cursorInfo);
}

#include "main.c"

