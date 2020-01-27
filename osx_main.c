#include "main.h"
#include <sys/ioctl.h> // screen_size
#include <unistd.h>

#include <sys/select.h> // input

// performance
    #include <mach/mach.h>
    #include <mach/mach_time.h>

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
void platform_setup() {
	system ("/bin/stty raw");
    printf("\e[?25l");
}
void platform_breakdown() {
	//system ("/bin/stty echo");
    //system("setterm -cursor on");
	system ("/bin/stty cooked");
    printf("\e[?25h");
}

int platform_check_input(void) {

  struct timeval tv;
  fd_set read_fd;

  // no wait
  tv.tv_sec=0;
  tv.tv_usec=0;

  // init
  FD_ZERO(&read_fd);

  int stdin_id = 0;
  /* Makes select() ask if input is ready:
   * 0 is the file descriptor for stdin    */
  FD_SET(stdin_id,&read_fd);

  if(select(stdin_id + 1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) != -1) {
	if(FD_ISSET(0,&read_fd)) {
		return 1;
	}
  }

  /* no characters were pending or error */
  return 0;
}

char platform_get_input(){
	system ("/bin/stty raw");
	system ("/bin/stty -echo");
	char input = '0';
    input = getchar();
    //fseek(stdin,0,SEEK_END);
	//system ("/bin/stty cooked");
	return input;
}

void platform_time_setup(void * timebase_info) {
    timebase_info = malloc(sizeof (mach_timebase_info_data_t));
    mach_timebase_info((mach_timebase_info_data_t *)timebase_info);
}
uint64_t platform_time() {
    return mach_absolute_time();
}
uint64_t platform_time_to_micro(uint64_t perf_elapsed, void * timebase_info_void) {
    static mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);
    uint64_t elapsedNano = perf_elapsed * timebase_info.numer / timebase_info.denom;
    float elapsedMicro = elapsedNano / 1000.0f;
    float elapsedMili = elapsedMicro / 1000.0f;
    return elapsedMili;
}

#include"main.c"
