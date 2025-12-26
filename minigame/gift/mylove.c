/**************************************************************************************************/
/**
 * @file mylove.c
 * @author Ryan Jing
 * @brief  A gift of our adventures together in C
 *
 * @version 0.2
 * @date 2025-12-09
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <termios.h> // For terminal I/O settings
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#include "textures.h"
#include "sprites.h"
#include "background.h"
#include "terminal.h"
#include "render.h"
#include "scenes.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define NUM_CUTSCENES 9

// DEBUG: Set to desired cutscene start time in seconds (0 = normal, 180 = cutscene 6, 210 = cutscene 7)
// Comment out or set to 0 for normal playback
#define DEBUG_START_TIME_SECONDS 400

/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

typedef enum
{
    GAME_STATE_PLAYING,
    GAME_STATE_CUTSCENE,
    GAME_STATE_PAUSED
} game_state;

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

int main()
{
    srand(time(NULL)); // Seed random number generator

    sprite character;
    background_system background;
    game_state current_game_state = GAME_STATE_PLAYING;

    bool terminate_execution = false;
#ifdef DEBUG_START_TIME_SECONDS
    int frame_count = seconds_to_frames((float)DEBUG_START_TIME_SECONDS);
#else
    int frame_count = 0;
#endif
    float display_scroll_speed = 1.5;
    int active_cutscene_index = -1;  // No active cutscene

    initialize_sprite(&character);
    initialize_background(&background);

    enable_raw_mode();
    enter_alternate_screen();

    // Define all cutscenes with trigger times in seconds (15 seconds between each)
    cutscene_definition cutscenes[NUM_CUTSCENES] =
    {
        {
            .trigger_time_seconds = 0.0f,
            .has_triggered = false,
            .init = init_cutscene_intro,
            .update = update_cutscene_intro,
            .render = render_cutscene_intro
        },
        {
            .trigger_time_seconds = 60.0f,
            .has_triggered = false,
            .init = init_cutscene_greeting,
            .update = update_cutscene_greeting,
            .render = render_cutscene_greeting
        },
        {
            .trigger_time_seconds = 140.0f,
            .has_triggered = false,
            .init = init_cutscene_second,
            .update = update_cutscene_second,
            .render = render_cutscene_second
        },
        {
            .trigger_time_seconds = 210.0f,
            .has_triggered = false,
            .init = init_cutscene_bk_walk,
            .update = update_cutscene_bk_walk,
            .render = render_cutscene_bk_walk
        },
        {
            .trigger_time_seconds = 300.0f,
            .has_triggered = false,
            .init = init_cutscene_look,
            .update = update_cutscene_look,
            .render = render_cutscene_look
        },
        {
            .trigger_time_seconds = 320.0f,
            .has_triggered = false,
            .init = init_cutscene_buff_ryan,
            .update = update_cutscene_buff_ryan,
            .render = render_cutscene_buff_ryan
        },
        {
            .trigger_time_seconds = 360.0f,
            .has_triggered = false,
            .init = init_cutscene_swingset,
            .update = update_cutscene_swingset,
            .render = render_cutscene_swingset
        },
        {
            .trigger_time_seconds = 460.0f,
            .has_triggered = false,
            .init = init_cutscene_garden,
            .update = update_cutscene_garden,
            .render = render_cutscene_garden
        },
        {
            .trigger_time_seconds = 560.0f,
            .has_triggered = false,
            .init = init_cutscene_the_end,
            .update = update_cutscene_the_end,
            .render = render_cutscene_the_end
        }
    };

    while(!terminate_execution)
    {
        char keyboard_input;

        if (read(STDIN_FILENO, &keyboard_input, 1) == 1)
        {
            if (keyboard_input == ' ')
            {
                if (current_game_state == GAME_STATE_PLAYING)
                {
                    sprite_jump(&character);
                }
                else if (current_game_state == GAME_STATE_CUTSCENE)
                {
                    // Space can skip dialogue, typewrite effect
                    cutscenes[active_cutscene_index].status.current_dialogue++;
                    cutscenes[active_cutscene_index].status.dialogue_char_count = 0;
                }
            }
            else if (keyboard_input == 'q' || keyboard_input == 'Q')
            {
                terminate_execution = true;
            }
        }

        float current_time = frames_to_seconds(frame_count);

        if (current_game_state == GAME_STATE_PLAYING)
        {
            for (int i = 0; i < NUM_CUTSCENES; i++)
            {
                if (!cutscenes[i].has_triggered &&
                    current_time >= cutscenes[i].trigger_time_seconds)
                {
                    current_game_state = GAME_STATE_CUTSCENE;
                    active_cutscene_index = i;
                    cutscenes[i].has_triggered = true;
                    cutscenes[i].init(&cutscenes[i].status, &character);
                    break;
                }
            }
        }

        if (current_game_state == GAME_STATE_PLAYING)
        {
            update_sprite_position(&character);
            update_background(&background, display_scroll_speed);
            render(&character, &background);
        }

        else if (current_game_state == GAME_STATE_CUTSCENE)
        {
            cutscene_definition *active = &cutscenes[active_cutscene_index];

            active->update(&active->status, &character, &background);
            active->render(&active->status, &character, &background);

            if (active->status.finished)
            {
                if (active_cutscene_index == NUM_CUTSCENES - 1)
                {
                    terminate_execution = true; // End game after final cutscene
                }
                else if (active->status.chain_to_next)
                {
                    // Immediately trigger next cutscene
                    int next_index = active_cutscene_index + 1;
                    cutscenes[next_index].has_triggered = true;
                    cutscenes[next_index].init(&cutscenes[next_index].status, &character);
                    active_cutscene_index = next_index;
                    // Stay in GAME_STATE_CUTSCENE
                }
                else
                {
                    current_game_state = GAME_STATE_PLAYING;
                    active_cutscene_index = -1;
                }
            }
        }

        usleep(FRAME_TIME_US);

        frame_count++;
    }

    disable_raw_mode();
    printf("MWAH!!\n");

    return 0;
}

// End of mylove.c
