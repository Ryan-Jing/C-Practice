/**************************************************************************************************/
/**
 * @file render.h
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

#ifndef RENDER_H
#define RENDER_H

/*------------------------------------------------------------------------------------------------*/
// HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include "terminal.h"
#include "ascii.h"
#include "sprites.h"
#include "background.h"

/*------------------------------------------------------------------------------------------------*/
// GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
// CLASS DECLARATIONS                                                                             */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
// FUNCTION DECLARATIONS                                                                          */
/*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************/
/**
 * @name    draw_object
 * @brief   General-purpose function to draw any ASCII sprite to the terminal display buffer.
 *          Loops through the sprite's dimensions and copies each character to the display.
 *
 * @param   terminal_display  The 2D character array representing the terminal screen
 * @param   sprite           Pointer to the ascii_object structure containing the sprite data
 * @param   x                X-coordinate (column) where the sprite's top-left corner will be drawn
 * @param   y                Y-coordinate (row) where the sprite's top-left corner will be drawn
 *
 * @return  void
 */
/**************************************************************************************************/
void draw_object(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                      const ascii_object *sprite, int x, int y);

/**************************************************************************************************/
/**
 * @name    draw_sprite
 * @brief   Draws the player sprite character to the terminal display buffer.
 *
 * @param   terminal_display  The 2D character array representing the terminal screen
 * @param   character        Pointer to the sprite structure containing position data
 *
 * @return  void
 */
/**************************************************************************************************/
void draw_sprite(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                 sprite *character);

/**************************************************************************************************/
/**
 * @name    render
 * @brief   Main render function that draws all game elements and outputs to the terminal.
 *
 * @param   character    Pointer to the player sprite
 * @param   background   Pointer to the background system containing parallax layers
 *
 * @return  void
 */
/**************************************************************************************************/
void render(sprite *character, background_system *background);

#endif // RENDER_H

// End of render.h