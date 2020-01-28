#include "main.h"
#include<windows.h>
#include<conio.h>


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
void platform_setup() {
	/*
	system ("/bin/stty raw");
    printf("\e[?25l");
    */
}
void platform_breakdown() {
	/*
	//system ("/bin/stty echo");
    //system("setterm -cursor on");
	system ("/bin/stty cooked");
    printf("\e[?25h");
    */
}

int platform_check_input(void) {
	return _kbhit() != 0;
}

char platform_get_input(){
	char input = '0';
	input = getch();
	return input;
}

void platform_time_setup(void * timebase_info) {
}
uint64_t platform_time() {
    return 0;
}
uint64_t platform_time_to_micro(uint64_t perf_elapsed, void * timebase_info_void) {
    return 0;
}

#include "main.c"

