/**************************************************************************************************/
/**
 * @file scene_two.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief CUTSCENE 2: SECOND ENCOUNTER
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

#define NUM_LINES_SCENE_2 7

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_second(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = player->x + 80;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_second(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg;
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

    if (cutscene->phase == 1)
    {
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(8.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_2)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
        }
    }

    else if (cutscene->phase == 2)
    {
        cutscene->ryan.x += 2;

        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH - 3)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_second(cutscene_status *cutscene,
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
            "Ryan: Oh hello again! How are you enjoying the view!\n\n",
            "You know my love, I always wanted us to walk and hike through the mountains together. I know that time will come soon. I brought you here to share about how much I love you.",
            "From the moment I met you, what I loved the most was that you were willing to listen to me. You were true, kind and fair. I felt like I knew you and had you with me.",
            "Even though we didn't see each other much in person, I always knew you were there for me",
            "After so many years of knowing each other, and even after some poor choices on my end, and our distances, I always thought of you and missed you. You were always 'my Rachell', whether in context clarifiction, or just within my own heart, I knew you were 'my Rachell'.",
            "I liked seeing you around, talking and hanging out with you. I liked calling you and hearing youre voice. \n\nI liked seeing your face when we called. Even if we didn't do much, I enjoyed just being with you.",
            "I have to get going, I'm getting a little hungry. I need to scope out what's around."
        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_2)
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
