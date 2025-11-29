/**************************************************************************************************/
/**
 * @file cube.c
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

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define DISPLAY_WIDTH 80   // Adjust to match your terminal width
#define DISPLAY_HEIGHT 44

/*------------------------------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/

float cube_width = 20;

float cube_position_x, cube_position_y, cube_position_z;
float rotation_angle_A, rotation_angle_B, rotation_angle_C;
float z_depth_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

char display_frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
int display_background_ascii_character = ' ';
int display_view_distance = 150;
float display_field_of_view = 50;
float display_cube_density = 0.2;
float display_y_offset = 0;
float display_x_offset = 5;
float display_aspect_ratio = 1.5;  // Adjust to fix stretching (higher = wider cube)

float cube_x, cube_y, cube_z;
float inverse_z;
int buffers_index;
int cube_x_coordinate_3d_projected, cube_y_coordinate_3d_projected;

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
 * @name calculate_cube_display_output
 * @brief Calculates the display output for a rotating 3D cube by rendering its surfaces
 *
 *
 * @return void
 */
/**************************************************************************************************/
void calculate_cube_display_output();

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

int calculate_x_coordinate_3d_projection(float temporary_inverse_z, float temporary_cube_x)
{
    return (int)(DISPLAY_WIDTH / 2 +
                (display_field_of_view * temporary_inverse_z * temporary_cube_x * display_aspect_ratio)
                - display_x_offset);
}

int calculate_y_coordinate_3d_projection(float temporary_inverse_z, float temporary_cube_y)
{
    return (int)(DISPLAY_HEIGHT / 2 +
                (display_field_of_view * temporary_inverse_z * temporary_cube_y)
                + display_y_offset);
}

void calculate_cube_display_output()
{
    // float ;
    // float ;

    for (float cube_x_position = -cube_width;  cube_x_position < cube_width; cube_x_position += display_cube_density)
    {
        for (float cube_y_position = -cube_width; cube_y_position < cube_width; cube_y_position += display_cube_density)
        {
            // Front face
            calculate_surface_render(cube_x_position, cube_y_position, -cube_width, '@');

            // Right face
            calculate_surface_render(cube_width, cube_y_position, cube_x_position, '$');

            // Left face
            calculate_surface_render(-cube_width, cube_y_position, -cube_x_position, '~');

            // Back face
            calculate_surface_render(-cube_x_position, cube_y_position, cube_width, '#');

            // Bottom face
            calculate_surface_render(cube_x_position, -cube_width, -cube_y_position, ';');

            // Top face
            calculate_surface_render(cube_x_position, cube_width, cube_y_position, '+');
        }
    }
}

void calculate_surface_render(float cube_x, float cube_y, float cube_z, int ascii_character) {
    float rotated_x = calculate_x_rotation(cube_x, cube_y, cube_z);
    float rotated_y = calculate_y_rotation(cube_x, cube_y, cube_z);
    float rotated_z = calculate_z_rotation(cube_x, cube_y, cube_z) + display_view_distance;

    inverse_z = 1 / rotated_z;

    cube_x_coordinate_3d_projected = calculate_x_coordinate_3d_projection(inverse_z, rotated_x);
    cube_y_coordinate_3d_projected = calculate_y_coordinate_3d_projection(inverse_z, rotated_y);

    buffers_index = cube_x_coordinate_3d_projected + cube_y_coordinate_3d_projected * DISPLAY_WIDTH;

    if (buffers_index >= 0 && buffers_index < DISPLAY_WIDTH * DISPLAY_HEIGHT) {
        if (inverse_z > z_depth_buffer[buffers_index]) {
            z_depth_buffer[buffers_index] = inverse_z;
            display_frame_buffer[buffers_index] = ascii_character;
        }
  }
}
int main() {
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

        calculate_cube_display_output();

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