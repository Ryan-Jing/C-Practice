/**************************************************************************************************/
/**
 * @file scene_six.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 6: SWINGSET
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
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_swingset(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
    cutscene->swing_x = TERMINAL_DISPLAY_WIDTH + 500;
}

void update_cutscene_swingset(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0:
        update_background(bg, 1.5);
        cutscene->ryan.x -= 2;

        if (cutscene->ryan.x <= player->x + 9)
        {
            cutscene->ryan.x = player->x + 9;
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
        }
        break;

    case 1:
        update_background(bg, 1.5);
        cutscene->swing_x -= 1.5f * 0.9f;
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(5.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2 && cutscene->swing_x <= player->x + 20)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2:
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        else if (cutscene->current_dialogue >= 6)
        {
            cutscene->phase = 3;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3:
        update_background(bg, 1.5);
        cutscene->swing_x -= 1.5f * 0.9f;
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        else if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 4;
            cutscene->frame_timer = 0;
        }
        break;

    case 4:
        cutscene->ryan.x += 2;

        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH + 10)
        {
            cutscene->finished = true;
        }
        break;
    }
}

void render_cutscene_swingset(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, bg);

    int swing_y = TERMINAL_DISPLAY_HEIGHT - swing_set.height - 1;
    draw_object(terminal_display, &swing_set, (int)cutscene->swing_x, swing_y);

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    const char *dialogue_text = NULL;

    if (cutscene->phase == 1)
    {
        const char *dialogues_phase1[] = {
            "Ryan: Hey look, there's something up ahead!",
            "Damn I shouldn't have gotten Burger King. I don't have any popcorn chicken, just some left over chicken nuggets..."
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase1[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 2)
    {
        // Standstill at swing, main conversation
        const char *dialogues_phase2[] = {
            "Ryan: The swingset! \n\nI like to think about our first date on this couch swing, at your house. We were very young looking back. \n\nYou were the first person I saw after months of lockdown. What a wonderful feeling that was.",
            "I was so nervous to see you, and I wanted to look my best and impress you.\n\n"
            "Andddd, my idea to do that was popcorn chicken! I thought it would make you as happy as it made me.",
            "Even more crazy is that my mom dropped me off. I really got my mom to drop me off for our first date!",
            "But after you came to get me on that park bench, I got to sit beside you and talk to you on your deck.\n\n"
            "I got to spend time with you, sitting right next to you, and I loved that.",
            "Many years later, I came back to your house, but now as your boyfriend.",
            "More things will come after this moment, many many more lovely things, but this will always be our first date, the most wonderful first date."
        };
        if (cutscene->current_dialogue < 6)
        {
            dialogue_text = dialogues_phase2[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 3)
    {
        // Walking away
        const char *dialogues_phase3[] = {
            "Ryan: I have to go prepare something special for you.",
            "I'll be back soon, I promise!"
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase3[cutscene->current_dialogue];
        }
    }

    if (dialogue_text != NULL)
    {
        draw_dialogue(terminal_display, dialogue_text,
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
