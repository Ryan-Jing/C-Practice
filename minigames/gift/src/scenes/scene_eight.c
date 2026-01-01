/**************************************************************************************************/
/**
 * @file scene_eight.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 8: THE END
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

#define NUM_LINES_SCENE_8 6

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_the_end(cutscene_status *cutscene, sprite *player)
{
    (void)player; // Unused parameter

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_the_end(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;
    cutscene->dialogue_char_count += 2;

    if (cutscene->current_dialogue >= 1 && cutscene->frame_timer > seconds_to_frames(30.0f))
    {
        cutscene->finished = true; // End after final message
    }

    int dialogue_index = cutscene->frame_timer / seconds_to_frames(6.0f);

    if (dialogue_index > cutscene->current_dialogue)
    {
        cutscene->current_dialogue = dialogue_index;
        cutscene->dialogue_char_count = 0;
    }
}

void render_cutscene_the_end(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    // Blank screen like the intro
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    const char *dialogues[] =
    {
        "I want to spend the rest of my life with you my love, and I want you to always be reminded of how much I love you.",
        "No matter if you have your books, your cards, your letters, and now this, I want you to always know that I love you.",
        "Just type in ./mylove to see this again whenever and wherever you want.",
        "I love you, my love.",
        "Forever and always,\n\nRyan",
        " ",
        " "
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_8)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                      TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++) putchar(terminal_display[y][x]);
        putchar('\n');
    }
    fflush(stdout);
}
