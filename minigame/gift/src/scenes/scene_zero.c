/**************************************************************************************************/
/**
 * @file scene_zero.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 0: INTRODUCTION AND LETTER
 *
 * @version 0.1
 * @date 2025-12-27
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include "common.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define NUM_LINES_SCENE_0 3

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_intro(cutscene_status *cutscene, sprite *player)
{
    (void)player; // Unused parameter
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_intro(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;

    if (frames_to_seconds(cutscene->frame_timer) > 1.0f && cutscene->phase == 0)
    {
        cutscene->phase = 1;
    }

    else if (cutscene->phase == 1)
    {
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(15.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_0)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
        }
    }

    else if (cutscene->phase == 2)
    {
        cutscene->finished = true;
    }
}

void render_cutscene_intro(cutscene_status *cutscene,
                           sprite *player, background_system *background)
{
    (void)player;       // Unused parameter
    (void)background;   // Unused parameter

    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    if (cutscene->phase == 1)
    {
        const char *dialogues[] =
        {
            "Hi my love,\n\n"
            "I've been working away parcticing C, to make this gift for you, my most wonderful and sweet love.\n\n"
            "I want this to be a interactive gift for you, to remember our most lovely moments.\n\n"
            "I hope you can come back to this when you are feeling down, and feel my love.\n\n"
            "You are the love of my life. I love you forever.\n\n",
            "Here is our love in C.\n\n"
            "Ryan\n\n"
            "...",
            "Press q to quit, space to skip dialogue.\n\n"
            "..."

        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_0)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                         TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    if (cutscene->phase == 2)
    {
        for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
        {
            for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
            {
                terminal_display[y][x] = ' ';  // Blank screen
            }
        }
    }

    clear_terminal_screen();

    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }

    fflush(stdout);
}
