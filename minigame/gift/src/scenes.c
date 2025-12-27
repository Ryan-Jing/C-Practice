/**************************************************************************************************/
/**
 * @file scenes.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief Common helper functions for cutscenes
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

#include "common.h"

/*------------------------------------------------------------------------------------------------*/
/* HELPER FUNCTIONS                                                                               */
/*------------------------------------------------------------------------------------------------*/

int seconds_to_frames(float seconds)
{
    return (int)(seconds * TARGET_FPS);
}

float frames_to_seconds(int frames)
{
    return (float)frames / TARGET_FPS;
}

void draw_dialogue(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                                    const char *text, int dialogue_y, int chars_to_show)
{
    int box_left = 12;
    int box_right = TERMINAL_DISPLAY_WIDTH - 8;
    int max_lines = 15;

    int current_x = box_left;
    int current_line = 0;
    int chars_rendered = 0;

    for (int i = 0; text[i] != '\0' && chars_rendered < chars_to_show; i++)
    {
        char characters = text[i];

        if (characters == '\n')
        {
            current_line++;
            current_x = box_left;
            chars_rendered++;

            if (current_line >= max_lines)
                break;
            continue;
        }

        if (current_x >= box_right)
        {
            current_line++;
            current_x = box_left;

            if (current_line >= max_lines)
            {
                break;
            }
        }

        int text_y = dialogue_y + 2 + current_line;
        if (text_y < TERMINAL_DISPLAY_HEIGHT)  // Stay within bounds
        {
            terminal_display[text_y][current_x] = characters;
        }

        current_x++;
        chars_rendered++;
    }
}
