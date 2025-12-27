/**************************************************************************************************/
/**
 * @file scene_five.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 5: BUFF RYAN
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

#define NUM_LINES_SCENE_5 3

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = player->x + 9;
    cutscene->ryan.y = player->y;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0:
        cutscene->dialogue_char_count += 2;

        {
            int dialogue_index = cutscene->frame_timer / seconds_to_frames(5.0f);
            if (dialogue_index > cutscene->current_dialogue)
            {
                cutscene->current_dialogue = dialogue_index;
                cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
            }
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_5)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
        }
        break;

    case 1:
        cutscene->ryan.x += 2;

        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH + 10)
        {
            cutscene->finished = true;
        }
        break;
    }
}

void render_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, bg);

    draw_sprite(terminal_display, player);
    draw_ryan_buff_sprite(terminal_display, &cutscene->ryan);

    const char *dialogues[] = {
        "Ryan: Oh wait, also, we're kind of at the point in our story where I've been working out a bit. Take a look!",
        "Rachel: ...",
        "Ryan: Well, I hope I'm getting bigger."
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_5)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
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
