/**************************************************************************************************/
/**
 * @file scene_one.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 1: GREETING
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

#define NUM_LINES_SCENE_1 7

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_greeting(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH - 5;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->ryan_finished_walking = false;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_greeting(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg; // Unused parameter
    cutscene->frame_timer++;

    if (cutscene->phase == 0)
    {
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->ryan_finished_walking = true;
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
        }
    }

    else if (cutscene->phase == 1)
    {
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(8.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_1)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
        }
    }

    else if (cutscene->phase == 2)
    {
        cutscene->ryan.x -= 2;

        if (cutscene->ryan.x < 0)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_greeting(cutscene_status *cutscene,
                              sprite *player, background_system *background)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, background);

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    if (cutscene->phase == 1)
    {
        const char *dialogues[] =
        {
            "Ryan: Hi my love. Welcome to this little world I've made for us.",
            "You can see it has many of the most special natural formations such as ANSII trees and mountains, all of which I designed myself. Also, press your spacebar to skip/speed the dialogue.",
            "But more importantly, I hope you can have some of our special moments for you to remember.",
            "I'd like to walk with you through some of our memorable times together, and have this storybook always with you forever and ever. \n\nYou go first, I will catch up to you.",
            "Cue the music!",
            "...",
            "i guess i haven't implemented that yet. Maybe you can add your own music!"
        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_1)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                         TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
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
