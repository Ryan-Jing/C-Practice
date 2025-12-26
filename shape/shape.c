/**************************************************************************************************/
/**
 * @file shape.c
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief A simple 3D cube renderer in C using ASCII characters.
 *
 * @version 0.1
 * @date 2025-11-29
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "shape.h"
#include "shapes_config.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define DISPLAY_WIDTH 90   // Adjust to match your terminal width
#define DISPLAY_HEIGHT 44

/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

ShapeConfig *current_shape;  // Pointer to the current shape configuration

float rotation_angle_A, rotation_angle_B, rotation_angle_C;
float z_depth_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

char display_frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
int display_background_ascii_character = ' ';
int display_view_distance = 100;
float display_field_of_view = 50;
float display_density = 0.2;
float display_y_offset = 0;
float display_x_offset = -10;
float display_aspect_ratio = 1.5;  // Adjust to fix stretching (higher = wider cube)

float inverse_z;
int buffers_index;
int x_coordinate_3d_projected, y_coordinate_3d_projected;

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION PROTOTYPES                                                                            */
/*------------------------------------------------------------------------------------------------*/

/**************************************************************************************************/
/**
 * @name calculate_x_rotation, calculate_y_rotation, calculate_z_rotation
 * @brief Calculates the rotated coordinates of a point in 3D space based on
 *        rotation angles A, B, and C.
 *
 * @param cube_i
 * @param cube_j
 * @param cube_k
 *
 * @return float
 */
/**************************************************************************************************/
float calculate_x_rotation(float cube_i, float cube_j, float cube_k);
float calculate_y_rotation(float cube_i, float cube_j, float cube_k);
float calculate_z_rotation(float cube_i, float cube_j, float cube_k);

/**************************************************************************************************/
/**
 * @name calculate_x_coordinate_3d_projection, calculate_y_coordinate_3d_projection
 * @brief Calculates the 2D projected coordinates from 3D coordinates using a simple
 *        perspective projection.
 *
 * @param temporary_inverse_z
 * @param temporary_cube_x
 *
 * @return int
 */
/**************************************************************************************************/
int calculate_x_coordinate_3d_projection(float temporary_inverse_z, float temporary_cube_x);
int calculate_y_coordinate_3d_projection(float temporary_inverse_z, float temporary_cube_y);

/**************************************************************************************************/
/**
 * @name calculate_shape_display_output
 * @brief Calculates the display output for a rotating 3D shape by rendering its surfaces
 *
 *
 * @return void
 */
/**************************************************************************************************/
void calculate_shape_display_output();

/**************************************************************************************************/
/**
 * @name calculate_surface_render
 * @brief  Calculates the rendering of a single surface of the cube given its 3D coordinates and
 *         the ASCII character to represent that surface.
 *
 *
 * @param cube_x
 * @param cube_y
 * @param cube_z
 * @param ascii_character
 *
 * @return void
 */
/**************************************************************************************************/
void calculate_surface_render(float cube_x, float cube_y, float cube_z,
                              int ascii_character);

/*------------------------------------------------------------------------------------------------*/
/* FUNCTION DEFINITIONS                                                                           */
/*------------------------------------------------------------------------------------------------*/

float calculate_x_rotation(float cube_i, float cube_j, float cube_k)
{
    return cube_j * sin(rotation_angle_A) * sin(rotation_angle_B) * cos(rotation_angle_C) -
           cube_k * cos(rotation_angle_A) * sin(rotation_angle_B) * cos(rotation_angle_C) +
           cube_j * cos(rotation_angle_A) * sin(rotation_angle_C) +
           cube_k * sin(rotation_angle_A) * sin(rotation_angle_C) +
           cube_i * cos(rotation_angle_B) * cos(rotation_angle_C);
}

float calculate_y_rotation(float cube_i, float cube_j, float cube_k)
{
    return cube_j * cos(rotation_angle_A) * cos(rotation_angle_C) +
           cube_k * sin(rotation_angle_A) * cos(rotation_angle_C) -
           cube_j * sin(rotation_angle_A) * sin(rotation_angle_B) * sin(rotation_angle_C) +
           cube_k * cos(rotation_angle_A) * sin(rotation_angle_B) * sin(rotation_angle_C) -
           cube_i * cos(rotation_angle_B) * sin(rotation_angle_C);
}

float calculate_z_rotation(float cube_i, float cube_j, float cube_k)
{
    return cube_k * cos(rotation_angle_A) * cos(rotation_angle_B) -
           cube_j * sin(rotation_angle_A) * cos(rotation_angle_B) +
           cube_i * sin(rotation_angle_B);
}

int calculate_x_coordinate_3d_projection(float temporary_inverse_z, float temporary_x)
{
    return (int)(DISPLAY_WIDTH / 2 +
                (display_field_of_view * temporary_inverse_z * temporary_x * display_aspect_ratio)
                - display_x_offset);
}

int calculate_y_coordinate_3d_projection(float temporary_inverse_z, float temporary_y)
{
    return (int)(DISPLAY_HEIGHT / 2 +
                (display_field_of_view * temporary_inverse_z * temporary_y)
                + display_y_offset);
}

void calculate_shape_display_output()
{
    ShapeDimensions *dim = &current_shape->dimensions;

    // Front face (z = -z_half_size)
    for (float x = -dim->x_half_size; x < dim->x_half_size; x += display_density)
    {
        for (float y = -dim->y_half_size; y < dim->y_half_size; y += display_density)
        {
            float u = (x + dim->x_half_size) / (2.0f * dim->x_half_size);
            float v = (y + dim->y_half_size) / (2.0f * dim->y_half_size);
            char ch = get_face_character(&current_shape->faces[0], u, v, '@');
            calculate_surface_render(x, y, -dim->z_half_size, ch);
        }
    }

    // Right face (x = +x_half_size)
    for (float z = -dim->z_half_size; z < dim->z_half_size; z += display_density)
    {
        for (float y = -dim->y_half_size; y < dim->y_half_size; y += display_density)
        {
            float u = (z + dim->z_half_size) / (2.0f * dim->z_half_size);
            float v = (y + dim->y_half_size) / (2.0f * dim->y_half_size);
            char ch = get_face_character(&current_shape->faces[1], u, v, '$');
            calculate_surface_render(dim->x_half_size, y, z, ch);
        }
    }

    // Left face (x = -x_half_size)
    for (float z = -dim->z_half_size; z < dim->z_half_size; z += display_density)
    {
        for (float y = -dim->y_half_size; y < dim->y_half_size; y += display_density)
        {
            float u = (-z + dim->z_half_size) / (2.0f * dim->z_half_size);
            float v = (y + dim->y_half_size) / (2.0f * dim->y_half_size);
            char ch = get_face_character(&current_shape->faces[2], u, v, '~');
            calculate_surface_render(-dim->x_half_size, y, -z, ch);
        }
    }

    // Back face (z = +z_half_size)
    for (float x = -dim->x_half_size; x < dim->x_half_size; x += display_density)
    {
        for (float y = -dim->y_half_size; y < dim->y_half_size; y += display_density)
        {
            float u = (-x + dim->x_half_size) / (2.0f * dim->x_half_size);
            float v = (y + dim->y_half_size) / (2.0f * dim->y_half_size);
            char ch = get_face_character(&current_shape->faces[3], u, v, '#');
            calculate_surface_render(-x, y, dim->z_half_size, ch);
        }
    }

    // Bottom face (y = -y_half_size)
    for (float x = -dim->x_half_size; x < dim->x_half_size; x += display_density)
    {
        for (float z = -dim->z_half_size; z < dim->z_half_size; z += display_density)
        {
            float u = (x + dim->x_half_size) / (2.0f * dim->x_half_size);
            float v = (-z + dim->z_half_size) / (2.0f * dim->z_half_size);
            char ch = get_face_character(&current_shape->faces[4], u, v, ';');
            calculate_surface_render(x, -dim->y_half_size, -z, ch);
        }
    }

    // Top face (y = +y_half_size)
    for (float x = -dim->x_half_size; x < dim->x_half_size; x += display_density)
    {
        for (float z = -dim->z_half_size; z < dim->z_half_size; z += display_density)
        {
            float u = (x + dim->x_half_size) / (2.0f * dim->x_half_size);
            float v = (z + dim->z_half_size) / (2.0f * dim->z_half_size);
            char ch = get_face_character(&current_shape->faces[5], u, v, '+');
            calculate_surface_render(x, dim->y_half_size, z, ch);
        }
    }
}

void calculate_surface_render(float x, float y, float z, int ascii_character) {
    float rotated_x = calculate_x_rotation(x, y, z);
    float rotated_y = calculate_y_rotation(x, y, z);
    float rotated_z = calculate_z_rotation(x, y, z) + display_view_distance;

    inverse_z = 1 / rotated_z;

    x_coordinate_3d_projected = calculate_x_coordinate_3d_projection(inverse_z, rotated_x);
    y_coordinate_3d_projected = calculate_y_coordinate_3d_projection(inverse_z, rotated_y);

    buffers_index = x_coordinate_3d_projected + y_coordinate_3d_projected * DISPLAY_WIDTH;

    if (buffers_index >= 0 && buffers_index < DISPLAY_WIDTH * DISPLAY_HEIGHT) {
        if (inverse_z > z_depth_buffer[buffers_index]) {
            z_depth_buffer[buffers_index] = inverse_z;
            display_frame_buffer[buffers_index] = ascii_character;
        }
  }
}
int main() {
    // Initialize with pizza box - change to &regular_cube or &rectangular_box to see different shapes
    current_shape = &pizza_box;

    printf("\x1b[2J");  // Clear screen

    while(1)
    {
        // Set memory for buffers:
        // - memset function takes in the following parameters:
        //   1. Pointer to the block of memory to fill
        //   2. Value to be set
        //   3. Number of bytes to be set to the value

        // Here, we set the display frame buffer to the background ASCII character
        memset(display_frame_buffer,
               display_background_ascii_character,
               DISPLAY_WIDTH * DISPLAY_HEIGHT);

        // Here, we set the z-depth buffer to 0
        memset(z_depth_buffer, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT * 4);

        calculate_shape_display_output();

        printf("\x1b[H");  // Reset cursor to top-left position

        // Output the display frame buffer to the console
        for (int k = 0; k < DISPLAY_WIDTH * DISPLAY_HEIGHT; k++) {
            putchar(k % DISPLAY_WIDTH ? display_frame_buffer[k] : 10);
        }

        rotation_angle_A += 0.05;
        rotation_angle_B += 0.05;
        rotation_angle_C += 0.01;

        usleep(30000);  // Sleep for 60 milliseconds
    }
    return 0; // Program should never reach here
}