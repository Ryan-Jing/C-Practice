/**************************************************************************************************/
/**
 * @file shapes_config.h
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief Pre-defined shape configurations including cube and pizza box examples
 *
 * @version 0.1
 * @date 2025-11-29
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

#ifndef SHAPES_CONFIG_H
#define SHAPES_CONFIG_H

#include "shape.h"

/*------------------------------------------------------------------------------------------------*/
/* PIZZA BOX CONFIGURATION                                                                        */
/*------------------------------------------------------------------------------------------------*/

// Pizza pattern for the top face
static const char *pizza_pattern[] = {
    "          .###########.          ",
    "       .##################.      ",
    "     .######################.    ",
    "   .##########################.  ",
    "  .############################. ",
    " .##############################.",
    ".#####O########O######O#########.",
    ".##############################. ",
    ".####O#########O######O#######.  ",
    ".###########################.    ",
    " .#####O######O######O#####.     ",
    "  .#######################.      ",
    "   .##O#######O######O##.        ",
    "     .#################.         ",
    "       .#############.           ",
    "          .#######.              "
};

// White/blank pattern for sides
static const char *white_pattern[] = {
    " "
};

static ShapeConfig pizza_box = {
    .dimensions = {
        .x_half_size = 20.0f,   // Wide (pizza box width)
        .y_half_size = 2.0f,    // Very thin (pizza box height)
        .z_half_size = 20.0f    // Deep (pizza box depth)
    },
    .faces = {
        // Front face - white
        { .pattern = white_pattern, .width = 1, .height = 1 },
        // Right face - white
        { .pattern = white_pattern, .width = 1, .height = 1 },
        // Left face - white
        { .pattern = white_pattern, .width = 1, .height = 1 },
        // Back face - white
        { .pattern = white_pattern, .width = 1, .height = 1 },
        // Bottom face - white
        { .pattern = white_pattern, .width = 1, .height = 1 },
        // Top face - pizza!
        { .pattern = pizza_pattern, .width = 33, .height = 16 }
    }
};

/*------------------------------------------------------------------------------------------------*/
/* REGULAR CUBE CONFIGURATION                                                                     */
/*------------------------------------------------------------------------------------------------*/

static ShapeConfig regular_cube = {
    .dimensions = {
        .x_half_size = 20.0f,
        .y_half_size = 20.0f,
        .z_half_size = 20.0f
    },
    .faces = {
        // Front face
        { .pattern = NULL, .width = 1, .height = 1 },  // Will use default '@'
        // Right face
        { .pattern = NULL, .width = 1, .height = 1 },  // Will use default '$'
        // Left face
        { .pattern = NULL, .width = 1, .height = 1 },  // Will use default '~'
        // Back face
        { .pattern = NULL, .width = 1, .height = 1 },  // Will use default '#'
        // Bottom face
        { .pattern = NULL, .width = 1, .height = 1 },  // Will use default ';'
        // Top face
        { .pattern = NULL, .width = 1, .height = 1 }   // Will use default '+'
    }
};

/*------------------------------------------------------------------------------------------------*/
/* RECTANGULAR BOX CONFIGURATION (Example of non-cube dimensions)                                */
/*------------------------------------------------------------------------------------------------*/

static const char *front_text[] = {
    "  FRONT  "
};

static const char *back_text[] = {
    "  BACK   "
};

static ShapeConfig rectangular_box = {
    .dimensions = {
        .x_half_size = 25.0f,   // Wide
        .y_half_size = 15.0f,   // Medium height
        .z_half_size = 8.0f     // Shallow depth
    },
    .faces = {
        // Front face - text pattern
        { .pattern = front_text, .width = 9, .height = 1 },
        // Right face - solid
        { .pattern = NULL, .width = 1, .height = 1 },
        // Left face - solid
        { .pattern = NULL, .width = 1, .height = 1 },
        // Back face - text pattern
        { .pattern = back_text, .width = 9, .height = 1 },
        // Bottom face - solid
        { .pattern = NULL, .width = 1, .height = 1 },
        // Top face - solid
        { .pattern = NULL, .width = 1, .height = 1 }
    }
};

#endif // SHAPES_CONFIG_H
