/**************************************************************************************************/
/**
 * @file scenes.h
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief Dynamic cutscene system with multiple cutscenes
 *
 * @version 0.2
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

#ifndef SCENES_H
#define SCENES_H

/*------------------------------------------------------------------------------------------------*/
// HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "sprites.h"
#include "background.h"

/*------------------------------------------------------------------------------------------------*/
// MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define TARGET_FPS 30
#define FRAME_TIME_US (1000000 / TARGET_FPS)  // Microseconds per frame for usleep

/*------------------------------------------------------------------------------------------------*/
// TYPE DEFINITIONS                                                                               */
/*------------------------------------------------------------------------------------------------*/

typedef struct
{
    int frame_timer;
    int current_dialogue;
    int dialogue_char_count;  // Tracks typewriter effect (character-by-character rendering)
    sprite ryan;
    bool ryan_finished_walking;
    bool finished;        // Cutscene sets this when done
    bool chain_to_next;   // If true, immediately trigger next cutscene instead of returning to gameplay
    int phase;            // Track which phase of cutscene we're in
    float garden_x;       // Track garden position for cutscene 7
    float swing_x;        // Track swing position for cutscene 6
} cutscene_status;

// Function pointer types for cutscenes
typedef void (*cutscene_init_fn)(cutscene_status*, sprite*);
typedef void (*cutscene_update_fn)(cutscene_status*, sprite*, background_system*);
typedef void (*cutscene_render_fn)(cutscene_status*, sprite*, background_system*);

typedef struct
{
    float trigger_time_seconds;  // When to trigger (in seconds)
    bool has_triggered;          // Has it been triggered?

    cutscene_init_fn init;
    cutscene_update_fn update;
    cutscene_render_fn render;

    cutscene_status status;      // State for this specific cutscene
} cutscene_definition;

/*------------------------------------------------------------------------------------------------*/
// HELPER FUNCTIONS                                                                               */
/*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 * @param seconds
 *
 * @return int
 */
/**************************************************************************************************/
int seconds_to_frames(float seconds);

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 * @param frames
 *
 * @return float
 */
/**************************************************************************************************/
float frames_to_seconds(int frames);

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 * @param terminal_display
 * @param text
 * @param dialogue_y
 * @param chars_to_show
 *
 */
/**************************************************************************************************/
void draw_dialogue(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                   const char *text, int dialogue_y, int chars_to_show);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 0: LETTER                                                                             */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_intro(cutscene_status *cutscene, sprite *player);
void update_cutscene_intro(cutscene_status *cutscene, sprite *player, background_system *background);
void render_cutscene_intro(cutscene_status *cutscene, sprite *player,
                           background_system *background);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 1: GREETING                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_greeting(cutscene_status *cs, sprite *player);
void update_cutscene_greeting(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_greeting(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 2: SECOND ENCOUNTER                                                                   */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_second(cutscene_status *cs, sprite *player);
void update_cutscene_second(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_second(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 3: Burger King Walk                                                                   */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_bk_walk(cutscene_status *cs, sprite *player);
void update_cutscene_bk_walk(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_bk_walk(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 4: Look at each other                                                                 */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_look(cutscene_status *cs, sprite *player);
void update_cutscene_look(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_look(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 5: Buff 1Ryan                                                                          */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_buff_ryan(cutscene_status *cs, sprite *player);
void update_cutscene_buff_ryan(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_buff_ryan(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 6: Sad                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_swingset(cutscene_status *cs, sprite *player);
void update_cutscene_swingset(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_swingset(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 7: Garden                                                            */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_garden(cutscene_status *cs, sprite *player);
void update_cutscene_garden(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_garden(cutscene_status *cs, sprite *player, background_system *bg);

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 8: The End                                                                            */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_the_end(cutscene_status *cs, sprite *player);
void update_cutscene_the_end(cutscene_status *cs, sprite *player, background_system *bg);
void render_cutscene_the_end(cutscene_status *cs, sprite *player, background_system *bg);

#endif // SCENES_H

// End of scenes.h
