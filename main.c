// ctrl-c to quit currently
// do not build this directly. build linux_main.c or win_main.c

#include<signal.h> // interrupt
#include<time.h> // random

// @todo: turn this into state that gets passed around

int player_x = 0;

typedef struct {
    int width;
    int height;
    char * buffer;
} Screen;

typedef struct {
    int x;
    int y;
} V2;

#define assert(expr, msg) if(!(expr)) { printf("%s:%d %s()\nFailed: %s\nMessage: %s\n",__FILE__,__LINE__, __func__, #expr, msg); *(volatile int *)0 = 0; }

#define map_width 34
#define map_height 16

void clear_buffer(Screen * screen) {
    int i;
    for (i = 0; i < screen->width * screen->height; ++i) {
        screen->buffer[i] = ' ';
    }
    ++i;
    screen->buffer[i] = '\0';

}

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
    int buffer_len = strlen(screen->buffer);
    int buffer_max_size = screen->width * screen->height + 1;
    assert(buffer_len <= buffer_max_size, "print_buffer string is greater than screen size!");
    printf("%c", screen->buffer);
    fflush(stdout);
}

// used in the beginning and in the interrupt 
void screen_init(Screen * screen) {
    platform_set_screen_size(&screen->width, &screen->height);
    screen->buffer = malloc(screen->height * screen->width + 1);
}

void ending(Screen * screen) {
    clear_buffer(screen);
    char goodbye_msg[] = "Mistakes happen and are okay.";
    // centering
    int goodbye_x = screen->width/ 2 - (strlen(goodbye_msg)/ 2);
    int goodbye_y = (screen->height) / 2;
    writexy(screen, goodbye_x,goodbye_y,goodbye_msg);
    print_buffer(screen);
    platform_breakdown();
}

void interrupt_handler(int sigint) {
    Screen screen;
    screen_init(&screen);
    ending(&screen);
    abort();
}

int main() {
    Screen screen;
    screen_init(&screen);
    
    platform_setup();
    signal(SIGINT, interrupt_handler);

	char input = 0;
#define snowflakes_max 100 
    V2 snowflakes[snowflakes_max]; 
    for (int i = 0; i < snowflakes_max; ++i) {
        V2 *flake = &snowflakes[i];
        flake->y = -(rand() % screen.height);
        flake->x = rand() % screen.width;
    }
    srand(time(NULL));

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
    void * timebase_info = NULL; 
    platform_time_setup(timebase_info);
	for (; input != 'q';) {
        uint64_t perf_start = platform_time();
        input = 0;
        while(platform_check_input()) { 
		     input = platform_get_input();
        }
        int prev_player_x = player_x;
        if (input == 'd') {
            player_x++;
        }
        if (input == 'a') {
            player_x--;
        }
        if (floor[player_x+5] > floor[prev_player_x+5]+3 ) {
            player_x = prev_player_x;
        }
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
        // draw player
        for (int y = 0; y < 10; ++y) {
            for (int x = 0; x < 10; ++x) {
                int current_x =  player_x +x;
                int floor_current = floor[current_x];
                int floor_player = floor[player_x+5];
                /*
                if (y == 0 &&
                    floor_current > floor_player &&
                    floor_current < floor_player + 5) {
                    floor[current_x]--; 
                }
                */
                writexy(&screen, current_x, screen.height-5 - y - floor[player_x+5], "@");
            }
        }


        // title
        if (timer < 5*4) {
            char msg[] = "Rumbuli 1941\n";
            writexy(&screen, screen.width /2 - 12 /2, screen.height/2, msg);
        }

        // @Todo: count time, subtract here, so that it is always the same framerate. 
        uint64_t perf_end = platform_time();
        uint64_t perf_elapsed = perf_end-perf_start;
        //uint64_t perf_micro = perf_elapsed /2;
        uint64_t perf_micro = platform_time_to_micro(perf_elapsed, timebase_info);
        char perf_time_string[40];
        int time_to_wait = 20000;
        sprintf(perf_time_string, "%d %llu", perf_micro < time_to_wait, perf_micro);
        writexy(&screen, 0, 0, perf_time_string);

        print_buffer(&screen);

        if (perf_micro < time_to_wait) {
            platform_sleep(time_to_wait - perf_micro);
        }
        //platform_sleep(time_to_wait);

	}
    // called in interrupt handler.. not really needed here right now
    ending(&screen);
}
