/**************************************************************************************************/
/**
 * @file terminal.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief
 *
 * @version 0.1
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "terminal.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

struct termios original_terminal_settings;

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void enter_alternate_screen(void) {
    printf("\033[?1049h"); // Switch to alternate screen buffer
    printf("\033[H");      // Move cursor to top-left
}

void exit_alternate_screen(void) {
    printf("\033[?1049l"); // Switch back to main screen buffer
}

void clear_terminal_screen(void) {
    printf("\033[2J"); // ANSI escape code to clear the screen
    printf("\033[H");  // ANSI escape code to move cursor to top-left corner
}

void disable_raw_mode(void) {
    exit_alternate_screen();
    tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal_settings);
}

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &original_terminal_settings);
    atexit(disable_raw_mode);

    struct termios raw_settings = original_terminal_settings;
    raw_settings.c_lflag &= ~(ECHO | ICANON);   // Disable echo and canonical mode
    raw_settings.c_cc[VMIN] = 0;                // Minimum number of bytes before read returns
    raw_settings.c_cc[VTIME] = 0;               // No timeout

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_settings);
}

// End of terminal.c