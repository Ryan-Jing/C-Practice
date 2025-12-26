/**************************************************************************************************/
/**
 * @file shape.h
 * @author Ryan Jing (r5jing@uwaterloo.ca)
 * @brief Shape configuration for 3D renderer - supports any 6-sided shape with custom face patterns
 *
 * @version 0.1
 * @date 2025-11-29
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

#ifndef SHAPE_H
#define SHAPE_H

#include <stddef.h>

/*------------------------------------------------------------------------------------------------*/
/* SHAPE CONFIGURATION                                                                            */
/*------------------------------------------------------------------------------------------------*/

/**
 * Face pattern structure
 * - pattern: 2D array of characters to display on the face
 * - width: number of columns in the pattern
 * - height: number of rows in the pattern
 */
typedef struct {
    const char **pattern;
    int width;
    int height;
} FacePattern;

/**
 * Shape dimensions structure
 * Defines the half-dimensions of the 6-sided shape
 * - x_half_size: half-width (distance from center to left/right faces)
 * - y_half_size: half-height (distance from center to top/bottom faces)
 * - z_half_size: half-depth (distance from center to front/back faces)
 */
typedef struct {
    float x_half_size;  // Width (affects left/right faces)
    float y_half_size;  // Height (affects top/bottom faces)
    float z_half_size;  // Depth (affects front/back faces)
} ShapeDimensions;

/**
 * Complete shape configuration
 * - dimensions: the 3D dimensions of the shape
 * - faces[6]: patterns for each face in order:
 *   [0] = front face (facing -Z)
 *   [1] = right face (facing +X)
 *   [2] = left face (facing -X)
 *   [3] = back face (facing +Z)
 *   [4] = bottom face (facing -Y)
 *   [5] = top face (facing +Y)
 */
typedef struct {
    ShapeDimensions dimensions;
    FacePattern faces[6];
} ShapeConfig;

/*------------------------------------------------------------------------------------------------*/
/* HELPER MACROS                                                                                  */
/*------------------------------------------------------------------------------------------------*/

/**
 * Create a solid pattern (single character repeated)
 * Usage: SOLID_PATTERN('@', 10, 10)
 */
#define SOLID_PATTERN(ch, w, h) { .pattern = NULL, .width = w, .height = h }

/*------------------------------------------------------------------------------------------------*/
/* SHAPE DEFINITIONS                                                                              */
/*------------------------------------------------------------------------------------------------*/

/**
 * Get a character from a face pattern at normalized coordinates (0.0 to 1.0)
 * Returns the pattern character, or a default if pattern is NULL (solid fill)
 */
static inline char get_face_character(const FacePattern *face, float u, float v, char default_char)
{
    if (face->pattern == NULL) {
        return default_char;
    }

    // Normalize coordinates to pattern indices
    int x = (int)(u * (face->width - 1));
    int y = (int)(v * (face->height - 1));

    // Clamp to valid range
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= face->width) x = face->width - 1;
    if (y >= face->height) y = face->height - 1;

    return face->pattern[y][x];
}

#endif // SHAPE_H
