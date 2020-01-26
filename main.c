// ctrl-c to quit currently
// do not build this directly. build linux_main.c or win_main.c
#include "main.h"

#include<signal.h> // interrupt
#include<time.h> // random

// @todo: turn this into state that gets passed around

typedef struct {
    int width;
    int height;
    char * buffer;
} Screen;

#define assert(expr, msg) if(!(expr)) { printf("%s:%d %s()\nFailed: %s\nMessage: %s\n",__FILE__,__LINE__, __func__, #expr, msg); *(volatile int *)0 = 0; }

#define map_width 34
#define map_height 16

// used in the beginning and in the interrupt 
void screen_init(Screen * screen) {
    platform_set_screen_size(&screen->width, &screen->height);
    screen->buffer = malloc(screen->height * screen->width);
}

void clear_buffer(Screen * screen) {
    for (int i = 0; i < screen->width * screen->height; ++i) {
        screen->buffer[i] = ' ';
    }
}
// BASICS
//
typedef struct {
    int x;
    int y;
} V2;

void writexy(Screen * screen, int init_x, int init_y, char * string) {
    int x = init_x;
    int y = init_y;

    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '\n') {
            ++y;
            if (y > screen->height) break;
            x = init_x;
        }
        else {
            assert(x +y * screen->width <= screen->width * screen->height, "writexy string is greater than screen size!");
            if (x < screen->width && y < screen->height) {
                screen->buffer[x + y * screen->width] = string[i];
            }
            ++x;
        }
    }
}
    

void print_buffer(Screen * screen) {
    platform_reset_cursor(0,0);
    assert(strlen(screen->buffer) <= screen->width * screen->height, "print_buffer string is greater than screen size!");
    for (int i = 0; screen->buffer[i] != '\0'; ++i) {
        printf("%c", screen->buffer[i]);
    }
    fflush(stdout);
}

void ending(Screen * screen) {
    clear_buffer(screen);
    char goodbye_msg[] = "Mistakes happen and are okay.";
    // centering
    int goodbye_x = screen->width/ 2 - (strlen(goodbye_msg)/ 2);
    int goodbye_y = (screen->height) / 2;
    writexy(screen, goodbye_x,goodbye_y,goodbye_msg);
    print_buffer(screen);
    platform_show_cursor();
}

void interrupt_handler(int sigint) {
    Screen screen;
    screen_init(&screen);
    ending(&screen);
    platform_show_cursor();
    abort();
}

char get_input(){
	system ("/bin/stty raw");
	char input = '0';
	while((input=getchar()) == 0) {
			  putchar(input);
	}
	system ("/bin/stty cooked");
	return input;
}



int main() {
    Screen screen;
    screen_init(&screen);
    
    platform_hide_cursor();
    signal(SIGINT, interrupt_handler);

	//char input = 0;
#define snowflakes_max 100 
    V2 snowflakes[snowflakes_max]; 
    for (int i = 0; i < snowflakes_max; ++i) {
        V2 *flake = &snowflakes[i];
        flake->y = -(rand() % screen.height);
        flake->x = rand() % screen.width;
    }
    srand(time(NULL));

	//for (; input != 'q';) {
    
    // at some point we'll want to just bring this into the game editor, and allow changing 
    // color + texture. then we'll just reference everything based on that. 
    // we can have texture packs and color packs. I like the idea of creative restrictions built in. 

    
    char landscape[] = 
        "--         -------         ----------                ---      -------          --------- \n"
        "  \\       /       \\      /           \\            /    \\----/       \\       /           \n"
        "   \\     /         \\    /             \\          /                   -------            \n"
        "    ----            ----               ----------                                       \n"
    ;
    char trees[] = 
        "|  |      |  |    |  |           | |    |  |     |  |     | |     |  |      | |           | |\n";
    ;
    
    int timer = 0;


    int floor[1000] = {0};
	for (; ;) {
        // clear buffer
        clear_buffer(&screen);

        ++timer;

        // draw landscape
        writexy(&screen, 0, screen.height - 9, landscape);
        writexy(&screen, 88, screen.height - 9, landscape);


        // draw trees
        for (int i = 0;i < screen.height -9; i++) {
            writexy(&screen, 0, i, trees);
            writexy(&screen, 88, i, trees);
        }


        // draw snowflake
        for (int i = 0; i < snowflakes_max; ++i) {
            V2 *flake = &snowflakes[i];
            flake->x += rand() % 3 - 1;
            flake->y++;
            // add to floor
            if (flake->y >= screen.height-5 && 
                flake->x > 0 && 
                flake-> x < screen.width-1)
            {
                // distrubute snow
                if (floor[flake->x - 1] < floor[flake->x]) {
                    floor[flake->x - 1]++;
                }
                else if (floor[flake->x + 1] < floor[flake->x]) {
                    floor[flake->x + 1]++;
                }
                else {
                    floor[flake->x]++;
                }
            }
            // create more snow
            if (flake->y >= screen.height-4) {
                flake->y = -(rand() % screen.height);
                flake->x = rand() % screen.width;
            }
            if (flake->y >= 0) {
                writexy(&screen, flake->x, flake->y, "*");
            }
        }

        // draw snow floor
        for (int x = 0; x < screen.width; ++x) {
            for (int floor_idx = 0; floor_idx < floor[x]; ++floor_idx) {
                writexy(&screen, x,screen.height-5 - floor_idx,  "*");
            }
        }

#if 1
        // title
        if (timer < 5*4) {
            char msg[] = "Rumbuli 1941\n";
            writexy(&screen, screen.width /2 - 12 /2, screen.height/2, msg);
        }

#endif
        // @Todo: write elements to array instead, then print all at once
        // @Todo: count time, subtract here, so that it is always the same framerate. 
        print_buffer(&screen);
        platform_sleep(250000);
	}
    // called in interrupt handler.. not really needed here right now
    ending(&screen);
}
