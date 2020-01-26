#include "main.h"
#include <sys/ioctl.h> // screen_size
#include <unistd.h>

// input
#include <sys/select.h>

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
	system ("/bin/stty -echo");
}
void platform_show_cursor() {
	system ("/bin/stty echo");
}
void platform_remap_signals() {
	//system ("/bin/stty intr q");
	//system ("/bin/stty intr d");
}
void platform_reset_signals() {
	//system ("/bin/stty intr ^c");
}

int kbhit(void) {

	system ("/bin/stty raw");
	system ("/bin/stty -echo");
/*
fd_set s_rd, s_wr, s_ex;
FD_ZERO(&s_rd);
FD_ZERO(&s_wr);
FD_ZERO(&s_ex);
FD_SET(fileno(stdin), &s_rd);
select(fileno(stdin)+1, &s_rd, &s_wr, &s_ex, NULL);
printf("okay!");
abort();
return 0;
*/

  struct timeval tv;
  fd_set read_fd;

  /* Do not wait at all, not even a microsecond */
  tv.tv_sec=0;
  tv.tv_usec=0;

  /* Must be done first to initialize read_fd */
  FD_ZERO(&read_fd);

  /* Makes select() ask if input is ready:
   * 0 is the file descriptor for stdin    */
  FD_SET(0,&read_fd);

  /* The first parameter is the number of the
   * largest file descriptor to check + 1. */
  if(select(1, &read_fd,NULL, /*No writes*/NULL, /*No exceptions*/&tv) == -1) {
    return 0;  /* An error occured */
  }

  /*  read_fd now holds a bit map of files that are
   * readable. We test the entry for the standard
   * input (file 0). */
  
	if(FD_ISSET(0,&read_fd)) {
		return 1;
	}

	system ("/bin/stty cooked");
  /* no characters were pending */
  return 0;
}

#include"main.c"
