/**************************************************************************************************/
/**
 * @file terminal.h
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

#ifndef TERMINAL_H
#define TERMINAL_H

/*------------------------------------------------------------------------------------------------*/
// HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
// GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

#define TERMINAL_DISPLAY_WIDTH      70
#define TERMINAL_DISPLAY_HEIGHT     44

/*------------------------------------------------------------------------------------------------*/
// CLASS DECLARATIONS                                                                             */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
// FUNCTION DECLARATIONS                                                                          */
/*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************/
/**
 * @name    enter_alternate_screen
 * @brief   Enters the terminal's alternate screen buffer. This is done by sending the ANSI escape
 *          code "\033[?1049h" to the terminal. The alternate screen buffer allows for a separate
 *          screen to be used for applications, preserving the original screen content.
 *
 * @return  void
 */
/**************************************************************************************************/
void enter_alternate_screen(void);

/**************************************************************************************************/
/**
 * @name    exit_alternate_screen
 * @brief   Exits the terminal's alternate screen buffer, returning to the main screen buffer.
 *          This is done by sending the ANSI escape code "\033[?1049l" to the terminal, which
 *          restores the original screen content.
 *
 * @return  void
 */
/**************************************************************************************************/
void exit_alternate_screen(void);

/**************************************************************************************************/
/**
 * @name    enable_raw_mode
 * @brief   Enables raw mode for terminal input. In raw mode, input is read byte-by-byte without
 *          waiting for a newline, and special processing (like echoing input characters) is
 *          disabled.
 *
 *          This is useful for this project as it allows for real-time input handling, such as
 *          detecting key presses for jumping without requiring the user to press Enter.
 *
 * @return  void
 */
/**************************************************************************************************/
void enable_raw_mode(void);

/**************************************************************************************************/
/**
 * @name    disable_raw_mode
 * @brief   Disables raw mode for terminal input, restoring the original terminal settings.
 *
 * @return  void
 */
/**************************************************************************************************/
void disable_raw_mode(void);

/**************************************************************************************************/
/**
 * @name    clear_terminal_screen
 * @brief   Clears the terminal screen by sending ANSI escape codes to clear the display and move
 *          the cursor to the top-left corner.
 *
 * @return  void
 */
/**************************************************************************************************/
void clear_terminal_screen(void);

#endif // TERMINAL_H

// End of terminal.h
