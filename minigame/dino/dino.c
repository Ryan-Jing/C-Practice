/**************************************************************************************************/
/**
 * @file dino.c
 * @author Ryan Jing
 * @brief Creating terminal browser no internet dino jumping game in C
 *
 * @version 0.1
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <termios.h> // For terminal I/O settings
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "textures.h"
#include "sprites.h"
#include "background.h"
#include "terminal.h"
#include "render.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/

// While function prototypes are not required for this file, I am writing these prototypes for
// clarity and explanation of the purpose of each function for my own learning.

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

int main() {
    srand(time(NULL)); // Seed random number generator

    sprite character;
    background_system background;

    initialize_sprite(&character);
    initialize_background(&background);

    enable_raw_mode();
    enter_alternate_screen();

    bool terminate_execution = false;
    int frame_count = 0;
    float display_scroll_speed = 1.5;

    while(!terminate_execution)
    {
        char keyboard_input;

        if (read(STDIN_FILENO, &keyboard_input, 1) == 1)
        {
            if (keyboard_input == ' ')
            {
                sprite_jump(&character);
            }

            else if (keyboard_input == 'q' || keyboard_input == 'Q')
            {
                terminate_execution = true;
            }
        }

        update_sprite_position(&character);
        update_background(&background, display_scroll_speed);

        render(&character, &background);

        usleep(40000); // Roughly 30 FPS

        frame_count++;
    }

    disable_raw_mode();
    printf("Game Over!\n");

    return 0;
}

// End of dino.c