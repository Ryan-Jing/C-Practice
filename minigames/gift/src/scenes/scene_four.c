/**************************************************************************************************/
/**
 * @file scene_four.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 4: LOOK AT EACH OTHER
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

#define NUM_LINES_SCENE_4 2

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_look(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH;
    cutscene->ryan.y = player->y;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_look(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg;
    cutscene->frame_timer++;

    if (cutscene->phase == 0)
    {
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
    }
    else if (cutscene->phase == 1)
    {
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(4.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_4)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_look(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, bg);

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    const char *dialogues[] = {
        "Ryan: Still coming along my love? Lets go!",
        "There's just a few more things ahead I want to share with you."
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_4)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    // Render
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
