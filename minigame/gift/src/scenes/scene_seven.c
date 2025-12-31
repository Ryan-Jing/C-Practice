/**************************************************************************************************/
/**
 * @file scene_seven.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 7: GARDEN
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

void init_cutscene_garden(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->chain_to_next = false;
    cutscene->phase = 0;
    cutscene->garden_x = TERMINAL_DISPLAY_WIDTH + 900;
}

void update_cutscene_garden(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0:
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 15)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 1:
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2:
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f;
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(9.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 3;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3:
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f;

        if (cutscene->garden_x <= player->x - 3)
        {
            cutscene->phase = 4;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 4:
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 5;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 5:
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f;
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(7.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 6;
            cutscene->frame_timer = 0;
        }
        break;

    case 6:
        player->x -= 2;
        cutscene->ryan.x -= 2;

        if (player->x < -10 && cutscene->ryan.x < -10)
        {
            cutscene->finished = true;
            cutscene->chain_to_next = true;
        }
        break;
    }
}

void render_cutscene_garden(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    draw_scene_background(terminal_display, bg);

    if (cutscene->phase >= 2)
    {
        int garden_y = TERMINAL_DISPLAY_HEIGHT - garden.height;
        draw_object(terminal_display, &garden, (int)cutscene->garden_x, garden_y);
    }

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    const char *dialogue_text = NULL;

    if (cutscene->phase == 1)
    {
        const char *dialogues_phase1[] = {
            "Ryan: Phew! You are still here. Thank you for waiting!",
            "After our first date, so much has changed until now. There were ups and downs, but eventually I was able to call you mine. My Rachell was my Rachell even more. I got to hug you, hold your hand, kiss you, and love you more and more every day.",
            "We pray for us every day, to grow ourselves and love each other more and more, and I look back and cannot see anywhere how that has not happened.",
            "My love for you has grown and grown. My care for you has deepened. My admiration for you has exploded.",
            "While sometimes times may get busy, stressful, and hard, I know we always have each other. We also have what's ahead to look forward to."
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase1[cutscene->current_dialogue];
        }
    }

    else if (cutscene->phase == 2)
    {
        const char *dialogues_phase2[] = {
            "Ryan: Come, let's walk together.\n"
            "There's a beautiful garden up ahead I want to show you.",
            "Rachell: :)"
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase2[cutscene->current_dialogue];
        }
    }

    else if (cutscene->phase == 4)
    {
        // In the garden dialogue
        const char *dialogues_phase4[] = {
            "Ryan: Look! All the veggies and and all the flowers. Our garden my love.\n"
            "A wonderful land that we can grow and cultivate together.",
            "Rachell: <3",
            "Ryan: I want to stay in this garden with you forever and ever. Taking care of the plants and animals around us.\n"
            "We can plant your tomatoes, I can plant my hot peppers, and we can grow so many beautiful flowers together."
        };

        if (cutscene->current_dialogue < 3)
        {
            dialogue_text = dialogues_phase4[cutscene->current_dialogue];
        }
    }

    else if (cutscene->phase == 5)
    {
        // Walking away dialogue
        const char *dialogues_phase5[] = {
            "Ryan: Let's keep walking my love. There is more and more up ahead. So much more.\n"
            "All the milestones we will go through together. All the memories we will make together.",
            "Rachell: :)"
        };
        if (cutscene->current_dialogue < NUM_LINES_SCENE_8)
        {
            dialogue_text = dialogues_phase5[cutscene->current_dialogue];
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
