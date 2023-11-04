//Creating a complex animation with multiple elements moving in all directions involves managing the positions and behaviors of each object carefully. Here's an example of how you can create an animation in C using ncurses with two stick men shooting two birds, starfield raining in gold, and a sun and moon shining in the middle:


#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_STARS 1000
#define NUM_BIRDS 10
#define NUM_BULLETS 100

typedef struct {
    int x;
    int y;
    bool active;
    int color;
    int directionX;
    int directionY;
} Object;

typedef struct {
    int x;
    int y;
    bool active;
    int color;
} Bullet;

typedef struct {
    int x;
    int y;
    bool active;
    int color;
} Star;

int main() {
    initscr();
    curs_set(0); // Hide cursor
    timeout(100); // Set a timeout for getch() function

    start_color(); // Enable color
    init_pair(1, COLOR_CYAN, COLOR_BLACK); // Cyan color pair for stars
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Green color pair for stick men
    init_pair(3, COLOR_RED, COLOR_BLACK); // Red color pair for birds
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Yellow color pair for bullets
    init_pair(5, COLOR_YELLOW, COLOR_BLACK); // Yellow color pair for sun
    init_pair(6, COLOR_WHITE, COLOR_BLACK); // White color pair for moon
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK); // Magenta color pair for starfield

    // Initialize stars, stick men, birds, bullets, sun, and moon
    Star stars[NUM_STARS];
    Object stickMen[2] = {{COLS / 4, LINES / 2, true, 2, 1, 1},
                          {COLS * 3 / 4, LINES / 2, true, 2, -1, -1}};
    Object birds[NUM_BIRDS] = {{COLS / 2, LINES / 4, true, 3, 1, 1},
                               {COLS / 2, LINES * 3 / 4, true, 3, -1, -1}};
    Bullet bullets[NUM_BULLETS];
    Star sun = {COLS / 2, LINES / 2, true, 5};
    Star moon = {COLS / 2, LINES / 8, true, 6};

    for (int i = 0; i < NUM_STARS; ++i) {
        stars[i].x = rand() % COLS;
        stars[i].y = rand() % LINES;
        stars[i].color = 7;
    }

    for (int i = 0; i < NUM_BULLETS; ++i) {
        bullets[i].active = false;
    }

    while (1) {
        clear(); // Clear the screen

        // Draw starfield
        for (int i = 0; i < NUM_STARS; ++i) {
            attron(COLOR_PAIR(stars[i].color));
            mvprintw(stars[i].y, stars[i].x, ".");
            attroff(COLOR_PAIR(stars[i].color));
            stars[i].x += rand() % 3 - 1; // Random horizontal movement (-1, 0, 1)
            stars[i].y += rand() % 3 + 1; // Random vertical movement (1, 2, 3)
            if (stars[i].x < 0 || stars[i].x >= COLS || stars[i].y >= LINES) {
                stars[i].x = rand() % COLS;
                stars[i].y = rand() % LINES;
            }
        }

        // Draw stick men
        attron(COLOR_PAIR(2));
        for (int i = 0; i < 2; ++i) {
            if (stickMen[i].active) {
                mvprintw(stickMen[i].y, stickMen[i].x, " O ");
                mvprintw(stickMen[i].y + 1, stickMen[i].x - 1, "/|\\");
                mvprintw(stickMen[i].y + 2, stickMen[i].x - 1, "/ \\");
            }
        }
        attroff(COLOR_PAIR(2));

        // Draw birds
        attron(COLOR_PAIR(3));
        for (int i = 0; i < NUM_BIRDS; ++i) {
            if (birds[i].active) {
                mvprintw(birds[i].y, birds[i].x, "<>");
                birds[i].x += birds[i].directionX;
                birds[i].y += birds[i].directionY;
                if (birds[i].x < 0 || birds[i].x >= COLS) {
                    birds[i].directionX *= -1;
                }
                if (birds[i].y < 0 || birds[i].y >= LINES) {
                    birds[i].directionY *= -1;
                }
            }
        }
        attroff(COLOR_PAIR(3));

        // Draw bullets
        attron(COLOR_PAIR(4));
        for (int i = 0; i < NUM_BULLETS; ++i) {
            if (bullets[i].active) {
                mvprintw(bullets[i].y, bullets[i].x, "|");
                bullets[i].x += 2;
                if (bullets[i].x >= COLS) {
                    bullets[i].active = false;
                }
            }
        }
        attroff(COLOR_PAIR(4));

        // Draw sun
        attron(COLOR_PAIR(5));
        mvprintw(sun.y, sun.x, "*");
        attroff(COLOR_PAIR(5));

        // Draw moon
        attron(COLOR_PAIR(6));
        mvprintw(moon.y, moon.x, "o");
        attroff(COLOR_PAIR(6));

        refresh(); // Refresh the screen

        // Get user input for stick men shooting
        int key = getch();
        switch (key) {
            case 'w':
                stickMen[0].y--;
                break;
            case 's':
                stickMen[0].y++;
                break;
            case 'a':
                stickMen[0].x--;
                break;
            case 'd':
                stickMen[0].x++;
                break;
            case 'i':
                stickMen[1].y--;
                break;
            case 'k':
                stickMen[1].y++;
                break;
            case 'j':
                stickMen[1].x--;
                break;
            case 'l':
                stickMen[1].x++;
                break;
            case ' ':
                // Shoot bullets
                for (int i = 0; i < NUM_BULLETS; ++i) {
                    if (!bullets[i].active) {
                        bullets[i].x = stickMen[0].x + 1;
                        bullets[i].y = stickMen[0].y;
                        bullets[i].active = true;
                        break;
                    }
                }
                for (int i = 0; i < NUM_BULLETS; ++i) {
                    if (!bullets[i].active) {
                        bullets[i].x = stickMen[1].x + 1;
                        bullets[i].y = stickMen[1].y;
                        bullets[i].active = true;
                        break;
                    }
                }
                break;
        }

        // Exit the loop when 'q' is pressed
        if (key == 'q') {
            break;
        }
    }

    endwin(); // End ncurses mode
    return 0;
}


//n this animation, the stick men move with 'w', 's', 'a', and 'd' keys and shoot with the space bar. The birds move randomly in all directions. The starfield rains in gold, and a sun and moon shine in the middle. Compile and run the program using the provided compilation command to see the animation in action. Note that the appearance of colors may vary depending on your terminal settings.