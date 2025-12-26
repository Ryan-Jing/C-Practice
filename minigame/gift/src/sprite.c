/**************************************************************************************************/
/**
 * @file sprite.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief
 *
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "sprites.h"
#include "terminal.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void initialize_sprite(sprite *character)
{
    character->x = 10;
    character->y = TERMINAL_DISPLAY_HEIGHT - 3;
    character->velocity_y = 0;
    character->is_jumping = false;
}

void update_sprite_position(sprite *character)
{
    if (character->y < TERMINAL_DISPLAY_HEIGHT - 3 || character->velocity_y < 0)
    {
        character->velocity_y += 1;
        character->y += character->velocity_y;
        character->is_jumping = true;
    }
    else
    {
        character->y = TERMINAL_DISPLAY_HEIGHT - 3;
        character->velocity_y = 0;
        character->is_jumping = false;
    }

    // Safety checks for sprite position
    if (character->y >= TERMINAL_DISPLAY_HEIGHT - 3 )
    {
        character->y = TERMINAL_DISPLAY_HEIGHT - 3;
        character->velocity_y = 0;
        character->is_jumping = false;
    }
    if (character->y < 0)
    {
        character->y = 0;
    }
}

void sprite_jump(sprite *character)
{
    if (!character->is_jumping)
    {
        character->velocity_y = -3;
        character->is_jumping = true;
    }
}

void draw_ryan_sprite(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                     sprite *ryan)
{
    // Happy face character:
    //
    // Row 0: (._.)
    // Row 1: (   )
    // Row 2:  ' '
    //
    if (ryan->y >= 0 && ryan->y < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 0: (._.)
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 1] = '(';
        }
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 2] = '.';
        }
        if (ryan->x + 3 >= 0 && ryan->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 3] = '_';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 4] = '.';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 5] = ')';
        }
    }

    if (ryan->y + 1 >= 0 && ryan->y + 1 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 1: (   )
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 1] = '(';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 5] = ')';
        }
    }

    if (ryan->y + 2 >= 0 && ryan->y + 2 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 2:  ' '
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 2] = '\'';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 4] = '\'';
        }
    }
}

void draw_ryan_burgerking(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                     sprite *ryan)
{
    // Happy face character:
    //
    // Row 0: (._.)
    // Row 1: (   )/@
    // Row 2:  ' '
    //
    if (ryan->y >= 0 && ryan->y < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 0: (._.)
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 1] = '(';
        }
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 2] = '.';
        }
        if (ryan->x + 3 >= 0 && ryan->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 3] = '_';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 4] = '.';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 5] = ')';
        }
    }

    if (ryan->y + 1 >= 0 && ryan->y + 1 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 1: (   )/@
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 1] = '(';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 5] = ')';
        }
        if (ryan->x + 6 >= 0 && ryan->x + 6 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 6] = '/';
        }
        if (ryan->x + 7 >= 0 && ryan->x + 7 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 7] = '@';
        }
    }

    if (ryan->y + 2 >= 0 && ryan->y + 2 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 2:  ' '
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 2] = '\'';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 4] = '\'';
        }
    }
}

void draw_ryan_buff_sprite(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                     sprite *ryan)
{
    // Happy face character:
    //
    // Row 0: (._.)
    // Row 1:[(   )]
    // Row 2:  ' '
    //
    if (ryan->y >= 0 && ryan->y < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 0: (._.)
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 1] = '(';
        }
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 2] = '.';
        }
        if (ryan->x + 3 >= 0 && ryan->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 3] = '_';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 4] = '.';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y][ryan->x + 5] = ')';
        }
    }

    if (ryan->y + 1 >= 0 && ryan->y + 1 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 1: [(   )]
        if (ryan->x >= 0 && ryan->x < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x] = '[';
        }
        if (ryan->x + 1 >= 0 && ryan->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 1] = '(';
        }
        if (ryan->x + 5 >= 0 && ryan->x + 5 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 5] = ')';
        }
        if (ryan->x + 6 >= 0 && ryan->x + 6 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 1][ryan->x + 6] = ']';
        }
    }

    if (ryan->y + 2 >= 0 && ryan->y + 2 < TERMINAL_DISPLAY_HEIGHT)
    {
        // Row 2:  ' '
        if (ryan->x + 2 >= 0 && ryan->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 2] = '\'';
        }
        if (ryan->x + 4 >= 0 && ryan->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[ryan->y + 2][ryan->x + 4] = '\'';
        }
    }
}

// End of sprite.c