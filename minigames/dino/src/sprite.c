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
    character->y = TERMINAL_DISPLAY_HEIGHT - 4;
    character->velocity_y = 0;
    character->is_jumping = false;
}

void update_sprite_position(sprite *character)
{
    if (character->y < TERMINAL_DISPLAY_HEIGHT - 4 || character->velocity_y < 0)
    {
        character->velocity_y += 1;
        character->y += character->velocity_y;
        character->is_jumping = true;
    }
    else
    {
        character->y = TERMINAL_DISPLAY_HEIGHT - 4;
        character->velocity_y = 0;
        character->is_jumping = false;
    }

    // Safety checks for sprite position
    if (character->y >= TERMINAL_DISPLAY_HEIGHT - 4 )
    {
        character->y = TERMINAL_DISPLAY_HEIGHT - 4;
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
        character->velocity_y = -4;
        character->is_jumping = true;
    }
}

// End of sprite.c