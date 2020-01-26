// ctrl-c to quit currently

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h> // random
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>


#define assert(expr, msg) if(!(expr)) { printf("%s:%d %s()\nFailed: %s\nMessage: %s\n",__FILE__,__LINE__, __func__, #expr, msg); *(volatile int *)0 = 0; }

#define map_width 34
#define map_height 16
int screen_width;
int screen_height;


// BASICS
//
typedef struct {
    int x;
    int y;
} V2;

void writexy(char * screen_buffer, int init_x, int init_y, char * string) {
    int x = init_x;
    int y = init_y;

    for (int i = 0; string[i] != '\0'; ++i) {
        if (string[i] == '\n') {
            ++y;
            if (y > screen_height) break;
            x = init_x;
        }
        else {
            assert(x +y * screen_width <= screen_width * screen_height, "writexy string is greater than screen size!");
            if (x < screen_width && y < screen_height) {
                screen_buffer[x + y * screen_width] = string[i];
            }
            ++x;
        }
    }
}
    

void print_buffer(char * string) {
    printf("\033[%dA", screen_height); // Move up X lines;
    printf("\033[%dD", screen_width); // Move left X column;
    for (int i = 0; string[i] != '\0'; ++i) {
        printf("%c", string[i]);
    }
     fflush(stdout);
    //printf("\n");
    //printf("\033[%dA", 1); // Move up X lines;
}

void printxy(int x, int y, char * string) {
    printf("\033[%dA", screen_height); // Move up X lines;
    printf("\033[%dD", screen_width); // Move left X column;
    if (y != 0) {
        printf("\033[%dB", y); // Move downX lines;
    }
    if (x != 0) {
        printf("\033[%dC", x); // Move right X column;
    }
    int additional_newline = 0;
    for (int i = 0; string[i] != '\0'; ++i) {
        assert(i <= screen_width * screen_height, "print string is greater than screen size!");
        printf("%c", string[i]);
        if (string[i] == '\n') {
            ++additional_newline;
            printf("\033[%dC", x); // Move right X column;
        } 

    }
    printf("\033[%dA", y + additional_newline); // Move up X lines;
    printf("\033[%dD", x); // Move left X column;
    printf("\033[%dB", screen_height); // Move down X lines;
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
    { // get screen height and width
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        screen_height = w.ws_row;
        screen_width = w.ws_col;
    }

    char * screen_buffer = malloc(screen_height * screen_width);


	//char input = 0;
#define snowflakes_max 100 
    V2 snowflakes[snowflakes_max]; 
    for (int i = 0; i < snowflakes_max; ++i) {
        V2 *flake = &snowflakes[i];
        flake->y = -(rand() % screen_height);
        flake->x = rand() % screen_width;
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
        for (int i = 0; i < screen_height * screen_width; ++i) {
            screen_buffer[i] = ' ';
        }
        ++timer;

        // draw landscape
        writexy(screen_buffer, 0, screen_height - 9, landscape);
        writexy(screen_buffer, 88, screen_height - 9, landscape);


        // draw trees
        for (int i = 0;i < screen_height -9; i++) {
            writexy(screen_buffer, 0, i, trees);
            writexy(screen_buffer, 88, i, trees);
            //writexy(screen_buffer, 0, screen_height - screen_height* 0.3-40 + i, trees);
            //writexy(screen_buffer, 88, screen_height - screen_height* 0.3-40 + i, trees);
        }


        // draw snowflake
        for (int i = 0; i < snowflakes_max; ++i) {
            V2 *flake = &snowflakes[i];
            flake->x += rand() % 3 - 1;
            flake->y++;
            // add to floor
            if (flake->y >= screen_height-5 && 
                flake->x > 0 && 
                flake-> x < screen_width-1)
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
            if (flake->y >= screen_height-4) {
                flake->y = -(rand() % screen_height);
                flake->x = rand() % screen_width;
            }
            if (flake->y >= 0) {
                writexy(screen_buffer, flake->x, flake->y, "*");
            }
        }

        // draw snow floor
        for (int x = 0; x < screen_width; ++x) {
            for (int floor_idx = 0; floor_idx < floor[x]; ++floor_idx) {
                writexy(screen_buffer, x,screen_height-5 - floor_idx,  "*");
            }
        }

#if 1
        // title
        if (timer < 5*4) {
            char msg[] = "Rumbuli 1941\n";
            writexy(screen_buffer, screen_width /2 - 12 /2, screen_height/2, msg);
        }

#endif
        // @Todo: write elements to array instead, then print all at once
        // @Todo: count time, subtract here, so that it is always the same framerate. 
        print_buffer(screen_buffer);
        usleep(250000);
        //usleep(2500);
	}

    // never reached
    char goodbye_msg[] = "Mistakes happen and are okay.";
    // centering
    int goodbye_x = screen_width/ 2 - (strlen(goodbye_msg)/ 2);
    int goodbye_y = (screen_height) / 2;
    printxy(goodbye_x,goodbye_y,goodbye_msg);
}
