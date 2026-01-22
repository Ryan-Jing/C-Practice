/**************************************************************************************************/
/**
 * @file pomodoro.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief
 *
 * @version 0.1
 * @date 2026-01-01
 *
 * @copyright Copyright (c) 2026
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define WORK_TIMER_DEFAULT_MINUTES 25
#define BREAK_TIMER_DEFAULT_MINUTES 5

/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

typedef enum
{
    timer_work,
    timer_break
} timer_type_t;

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/

// While not necessary to declare prototypes for these functions, these are defined here for clarity
// and my own learning purposes.

/**************************************************************************************************/
/**
 * @name    update_sketchybar
 * @brief   Updates the sketchybar pomodoro item with the given label and icon.
 *
 *          This function calls the sketchybar command line tool to set the label and icon
 *          using the --set option. It also ensures the divider is shown when updating.
 *
 * @param label     "label" of the sketchybar widget item
 * @param icon      "icon" of the sketchybar widget item
 *
 * @return void
 */
/**************************************************************************************************/
void update_sketchybar(const char *label, const char *icon);

/**************************************************************************************************/
/**
 * @name    play_work_sound_notification
 * @brief   Plays a sound notifying that the work timer has changed
 *
 *
 *
 */
/**************************************************************************************************/
void play_work_sound_notification();

/**************************************************************************************************/
/**
 * @name    play_break_sound_notification
 * @brief   Plays a sound notifying that the break timer has changed
 *
 *
 *
 */
/**************************************************************************************************/
void play_break_sound_notification();

/**************************************************************************************************/
/**
 * @name    run_timer
 * @brief   Runs a countdown timer for the specified number of minutes, updating sketchybar
 *          each second.
 *
 *
 * @param timer_minutes
 * @param icon
 * @param timer_type
 *
 */
/**************************************************************************************************/
void run_timer(int timer_minutes, const char *icon, timer_type_t timer_type);

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void update_sketchybar(const char *label, const char *icon)
{
    char sketchybar_command[512];

    snprintf(sketchybar_command, sizeof(sketchybar_command),
             "sketchybar --set pomodoro label=\"%s\" icon=\"%s\" drawing=on "
             "--set pomodoro.divider drawing=on",
             label, icon);

    // System is a C function that passes a command to the host operating system. Very useful!
    system(sketchybar_command);
}

void play_work_sound_notification()
{
    // Play a built in Apple library chime sound
    system("afplay /System/Library/Sounds/Pop.aiff &");
}

void play_break_sound_notification()
{
    // Play a built in Apple library chime sound
    system("afplay /System/Library/Sounds/Purr.aiff &");
}

void run_timer(int timer_minutes, const char *icon, timer_type_t timer_type)
{
    time_t start_time = time(NULL);
    time_t end_time = start_time + (timer_minutes * 60);
    int last_remaining_timestamp = -1;

    if (timer_type == timer_work)
    {
        play_work_sound_notification();
    }
    else if (timer_type == timer_break)
    {
        play_break_sound_notification();
    }

    while (1)
    {
        time_t current_time = time(NULL);
        int remaining_time = (int)difftime(end_time, current_time);

        if (remaining_time < 0)
        {
            break;
        }

        // Since we are checking the time every ten seconds, and the remaining_time is a seconds
        // value, we only update when the remaining_time changes by the whole second.
        if (remaining_time != last_remaining_timestamp)
        {
            char time_stamp_string[16];

            snprintf(time_stamp_string, sizeof(time_stamp_string), "%02d:%02d",
                     remaining_time / 60, remaining_time % 60);
            update_sketchybar(time_stamp_string, icon);

            last_remaining_timestamp = remaining_time;
        }

        // Check timer 10 times per second to ensure accuracy
        usleep(100000);
    }
}

int main(int argc, char *argv[])
{
    int work_timer_minutes = WORK_TIMER_DEFAULT_MINUTES;
    int break_timer_minutes = BREAK_TIMER_DEFAULT_MINUTES;
    // atoi is a C standard library function that converts a string to an integer
    if (argc >= 2)
    {
        work_timer_minutes = atoi(argv[1]);
    }

    if (argc >= 3)
    {
        break_timer_minutes = atoi(argv[2]);
    }

    while (1)
    {
        // Monocraft Nerd Font hourglass icon: ⌛ (U+231B)
        run_timer(work_timer_minutes, "\u231B", timer_work);

        // Monocraft Nerd Font scissors icon: ⚓ (U+2693) | ✂ (U+2702)
        run_timer(break_timer_minutes, "\u2693", timer_break);
    }

    return 0;
}
