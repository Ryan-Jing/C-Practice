/**************************************************************************************************/
/**
 * @file scene_three.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 3: BURGER KING WALK
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

void init_cutscene_bk_walk(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_bk_walk(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0:
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 1:
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(4.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2:
        update_background(bg, 1.5);
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 3;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3:
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        else if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 4;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 4:
        cutscene->ryan.x += 2;
        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH)
        {
            cutscene->phase = 5;
        }
        break;

    case 5:
        cutscene->finished = true;
        break;
    }
}

void render_cutscene_bk_walk(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, bg);

    // Draw sprites
    draw_sprite(terminal_display, player);
    draw_ryan_burgerking(terminal_display, &cutscene->ryan);

    if (cutscene->phase == 1)
    {
        const char *dialogues[] = {
            "Ryan: Well... hm, I got hungry, so I got some Burger King. Want to keep walking and talk while I eat chicken nuggets?",
            "Rachell: ...",
            "Ryan: I will take that as a yes. Lets go!"
        };

        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }
    else if (cutscene->phase == 2)
    {
        const char *dialogues[] = {
            "Ryan: One of my most memorable moments with you before we started dating was when I got Burger King and we went out to walk around at Waterloo park.",
            "I know I missed you, I know I wanted to see you, and I definitely was not having a great time in my life at all. But you were there for me, beside me being present with me and open to listen.",
            "I don't remember everything I said, but what I remember was you being happy about me being me. You always made me feel heard and safe."
        };

        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    else if (cutscene->phase == 3)
    {
        const char *dialogues[] = {
            "Ryan: I want to do the same for you. I want to always be there for you, to support you and uplift you through any challenges you face. I want you to always feel loved, safe, and protected.",
            "When you need anyone to call, need someone to walk with, need someone to support you, I want to always be there for you.",
            "Let me go throw this out in the trash."
        };
        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
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
