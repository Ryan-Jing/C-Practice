/**************************************************************************************************/
/**
 * @file background.c
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "background.h"
#include "textures.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

void initialize_particles(background_system *background)
{
    for (int i = 0; i < TERMINAL_DISPLAY_HEIGHT; i++)
    {
        background->particle_positions[i] = (float)(rand() % TERMINAL_DISPLAY_WIDTH);
    }
}

void initialize_background(background_system *background)
{
    // Layer 0: Clouds (farthest)
    background->layers[0].speed_multiplier = 0.1f;
    strcpy(background->layers[0].color_code, "\033[90m"); // Dark gray
    background->layers[0].element_count = 3;
    background->layers[0].elements[0].x = 20;
    background->layers[0].elements[0].texture = &cloud_small;
    background->layers[0].elements[1].x = 50;
    background->layers[0].elements[1].texture = &cloud_large;
    background->layers[0].elements[2].x = 65;
    background->layers[0].elements[2].texture = &cloud_small;
    // Layer 1: Mountains (medium-far)
    background->layers[1].speed_multiplier = 0.3f;
    strcpy(background->layers[1].color_code, "\033[37m");  // White
    background->layers[1].element_count = 2;
    background->layers[1].elements[0].x = 15;
    background->layers[1].elements[0].texture = &mountain_twin_peaks;  // From sprites.h
    background->layers[1].elements[1].x = 45;
    background->layers[1].elements[1].texture = &mountain_twin_peaks;

    // Layer 2: Houses/forests (medium-close)
    background->layers[2].speed_multiplier = 0.5f;
    strcpy(background->layers[2].color_code, "\033[97m");  // Bright white
    background->layers[2].element_count = 1;
    background->layers[2].elements[0].x = 30;
    background->layers[2].elements[0].texture = &bush;  // From sprites.h
    // background->layers[2].elements[1].x = 55;
    // background->layers[2].elements[1].texture = &house;

    // Layer 3: Trees/cacti (closest, full speed)
    background->layers[3].speed_multiplier = 0.9f;
    strcpy(background->layers[3].color_code, "\033[0m");   // Default
    background->layers[3].element_count = 2;
    background->layers[3].elements[0].x = 35;
    background->layers[3].elements[0].texture = &tree;
    background->layers[3].elements[1].x = 60;
    background->layers[3].elements[1].texture = &tree;

    initialize_particles(background);
}

void update_background_particles(background_system *background, float speed)
{
    for (int i = 0; i < TERMINAL_DISPLAY_HEIGHT; i++)
    {
        background->particle_positions[i] -= speed;

        // When particle goes off screen, wrap around
        if (background->particle_positions[i] < 0)
        {
            background->particle_positions[i] = TERMINAL_DISPLAY_WIDTH + (rand() % 10);
        }
    }
}

void update_background(background_system *background, float speed)
{
    for (int layer = 0; layer < NUM_LAYERS; layer++) {
          float layer_speed = speed * background->layers[layer].speed_multiplier;

          for (int i = 0; i < background->layers[layer].element_count; i++)
        {
              background->layers[layer].elements[i].x -= layer_speed;

              if (background->layers[layer].elements[i].x < -64) // Wrap elements back around
              {
                  background->layers[layer].elements[i].x = TERMINAL_DISPLAY_WIDTH + (rand() % 80);

                  if (layer == LAYER_CLOUDS)
                  {
                      if (rand() % 2 == 0)
                      {
                        background->layers[layer].elements[i].texture = &cloud_large;
                      }
                      else
                      {
                        background->layers[layer].elements[i].texture = &cloud_small;
                      }

                  }
                  else if (layer == LAYER_MOUNTAINS)
                  {
                      // Randomly pick mountain type
                      int mountain_choice = rand() % 3;
                      if (mountain_choice == 0)
                      {
                          background->layers[layer].elements[i].texture = &mountain_small;
                      }
                      else if (mountain_choice == 1)
                      {
                          background->layers[layer].elements[i].texture = &mountain_large;
                      }
                      else
                      {
                          background->layers[layer].elements[i].texture = &mountain_twin_peaks;
                      }

                  }
                  else if (layer == LAYER_HOUSES)
                  {
                      // Randomly pick house or bush
                      if (rand() % 2 == 0)
                      {
                          background->layers[layer].elements[i].texture = &bush;
                      }
                      else
                      {
                        //   background->layers[layer].elements[i].texture = &house;
                      }

                  }
                  else // if (layer == LAYER_TREES)
                  {
                      background->layers[layer].elements[i].texture = &tree;
                  }
              }
          }
      }

    update_background_particles(background, speed);
}

// End of background.c