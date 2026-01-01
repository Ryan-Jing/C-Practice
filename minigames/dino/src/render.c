/**************************************************************************************************/
/**
 * @file render.c
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

#include "render.h"
#include "sprites.h"
#include "background.h"

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

void draw_object(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                      const ascii_object *sprite, int x, int y)
{
    for (int row = 0; row < sprite->height; row++)
    {
        for (int col = 0; col < sprite->width; col++)
        {
            // Check bounds and draw character
            if (y + row >= 0 && y + row < TERMINAL_DISPLAY_HEIGHT &&
                x + col >= 0 && x + col < TERMINAL_DISPLAY_WIDTH)
            {
                char ch = sprite->lines[row][col];
                // Only draw non-space characters to allow transparency
                if (ch != ' ')
                {
                    terminal_display[y + row][x + col] = ch;
                }
            }
        }
    }
}

void draw_sprite(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH], sprite *character)
{
    // Human-like sprite

        // (n_n)
        // {   }
        //  ` `

    //

        // Row 0: (n_n)
    if (character->y >= 0 && character->y < TERMINAL_DISPLAY_HEIGHT) {
        if (character->x < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y][character->x] = '(';
        }
        if (character->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y][character->x + 1] = 'n';
        }
        if (character->x + 2 < TERMINAL_DISPLAY_WIDTH) terminal_display[character->y][character->x + 2] = '_';
        {
            terminal_display[character->y][character->x + 2] = '_';
        }
        if (character->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y][character->x + 3] = 'n';
        }
        if (character->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y][character->x + 4] = ')';
        }
    }

    // Row 1: {   }
    if (character->y + 1 >= 0 && character->y + 1 < TERMINAL_DISPLAY_HEIGHT) {
        if (character->x < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 1][character->x] = '{';
        }
        if (character->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 1][character->x + 1] = ' ';
        }
        if (character->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 1][character->x + 2] = ' ';
        }
        if (character->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 1][character->x + 3] = ' ';
        }
        if (character->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 1][character->x + 4] = '}';
        }
    }
    // Row 2:  ` `
    if (character->y + 2 >= 0 && character->y + 2 < TERMINAL_DISPLAY_HEIGHT) {
        if (character->x < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 2][character->x] = ' ';
        }
        if (character->x + 1 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 2][character->x + 1] = '`';
        }
        if (character->x + 2 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 2][character->x + 2] = ' ';
        }
        if (character->x + 3 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 2][character->x + 3] = '`';
        }
        if (character->x + 4 < TERMINAL_DISPLAY_WIDTH)
        {
            terminal_display[character->y + 2][character->x + 4] = ' ';
        }
    }
}

void render(sprite *character, background_system *background)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize terminal display with spaces
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw parallax layers
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < background->layers[layer].element_count; i++)
        {
            int element_x = (int)background->layers[layer].elements[i].x;
            const ascii_object *texture = background->layers[layer].elements[i].texture;

            int element_y = 0;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 5 + (i % 3) * 3;
            }
            else if (layer ==  LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else // Most closse layer (trees)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw background particles (scrolling dust/dots)
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 4 == 0)
        {
            int particle_x = (int)background->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }

    }

    draw_sprite(terminal_display, character);

    clear_terminal_screen();

    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }

    printf("\nPress SPACE to jump | Press Q to quit\n");
    fflush(stdout); // fflush stdout to ensure immediate display
}

// End of render.c