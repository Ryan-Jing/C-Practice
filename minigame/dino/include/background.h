/**************************************************************************************************/
/**
 * @file background.h
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

#ifndef BACKGROUND_H
#define BACKGROUND_H

/*------------------------------------------------------------------------------------------------*/
// HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include "terminal.h"
#include "ascii.h"

/*------------------------------------------------------------------------------------------------*/
// GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

#define NUM_LAYERS                  4
#define MAX_ELEMENTS_PER_LAYER      10

/*------------------------------------------------------------------------------------------------*/
// CLASS DECLARATIONS                                                                             */
/*------------------------------------------------------------------------------------------------*/

typedef struct
{
    float x;
    const ascii_object *texture;
} background_element;

typedef struct
{
    background_element elements[MAX_ELEMENTS_PER_LAYER];
    int element_count;
    float speed_multiplier;
    char color_code[10];
} parallax_layer;

typedef struct
{
    parallax_layer layers[NUM_LAYERS];
    float particle_positions[TERMINAL_DISPLAY_HEIGHT];
} background_system;

typedef enum {
    LAYER_CLOUDS = 0,
    LAYER_MOUNTAINS = 1,
    LAYER_HOUSES = 2,
    LAYER_TREES = 3
} layer_type;

/*------------------------------------------------------------------------------------------------*/
// FUNCTION DECLARATIONS                                                                          */
/*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 *
 */
/**************************************************************************************************/
void initialize_particles(background_system *background);

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 *
 */
/**************************************************************************************************/
void initialize_background(background_system *background);

/**************************************************************************************************/
/**
 * @name    update_background_particles
 * @brief   Updates the positions of background particles to create a scrolling effect.
 *          Each particle's position is decremented by the specified speed.
 *
 *
 * @param background
 * @param speed       Speed at which particles move leftward, not technically pixels per frame but
 *                    a float value to allow for smoother movement.
 *
 * @return  void
 */
/**************************************************************************************************/
void update_background_particles(background_system *background, float speed);

/**************************************************************************************************/
/**
 * @name
 * @brief
 *
 *
 * @param background
 * @param speed
 *
 */
/**************************************************************************************************/
void update_background(background_system *background, float speed);

#endif // BACKGROUND_H

// End of background.h