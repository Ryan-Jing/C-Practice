/**************************************************************************************************/
/**
 * @file textures.h
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

#ifndef TEXTURES_H
#define TEXTURES_H

/*------------------------------------------------------------------------------------------------*/
// HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include "terminal.h"
#include "ascii.h"

/*------------------------------------------------------------------------------------------------*/
// GLOBAL VARIABLES                                                                               */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
// CLASS DECLARATIONS                                                                             */
/*------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------*/
/* SPRITE DEFINITIONS                                                                            */
/*------------------------------------------------------------------------------------------------*/

static const char *cloud_lines_large[] = {
    "      ___         ",
    "    .6   )_       ",
    "___6________)-____"
};

static const ascii_object cloud_large = {
    .lines = cloud_lines_large,
    .width = 17,
    .height = 3
};

static const char *cloud_lines_small[] = {
    "         ",
    "  .__    ",
    "_(___*-__"
};

static const ascii_object cloud_small = {
    .lines = cloud_lines_small,
    .width = 9,
    .height = 3
};

static const char *mountain_lines_small[] = {
    "    _^-_        ",
    "   / .  \\      ",
    "  /      \\/\\  ",
    " /   --.  /  \\ ",
    "/__  _ __  ___\\"
};

static const ascii_object mountain_small = {
    .lines = mountain_lines_small,
    .width = 15,
    .height = 5
};

static const char *mountain_lines_large[] = {
    "                                   _^-_                            ",
    "                                  /    \\                          ",
    "                                 /     \\                          ",
    "                               _/_       \\                        ",
    "                           /--/     _     \\                       ",
    "                          /      __/ \\..  \\                      ",
    "            ____         /   __./           \\   __.^.             ",
    "           /     \\   __/                    \\-/     \\_.         ",
    "       ___/        \\/                         \\         \\       ",
    "      /    ___.      \\_                        \\          \\     ",
    "     /    /            \\___.                    \\__         \\   ",
    "   /  .../                  \\__.                    \\..__    \\  ",
    "  / ..          __. ..                 ..--_.              \\--__  ",
    "/_____    _____  _ _____   _______________ _    ___     __    ___\\"
};

static const ascii_object mountain_large = {
    .lines = mountain_lines_large,
    .width = 64,
    .height = 13
};

static const char *mountain_twin_peaks_lines[] = {
    "                                         ",
    "                    _^-_.                ",
    "             __    /     \\              ",
    "            /  \\ /  --_.  \\            ",
    "         __^.    /          \\__         ",
    "        /      .   -  .        \\_.      ",
    "      _/       .__/  ..           \\     ",
    "  ___/     . .__        .___        \\_  ",
    " / ___/         --          \\___    __\\"
};

static const ascii_object mountain_twin_peaks = {
    .lines = mountain_twin_peaks_lines,
    .width = 45,
    .height = 10
};

static const char *tree_lines[] = {
    "  ***  ",
    " ***** ",
    "*******",
    "   |   ",
    "   |   "
};

static const ascii_object tree = {
    .lines = tree_lines,
    .width = 7,
    .height = 5
};

static const char *house_lines[] = {
    "   ____   ",
    "  /    \\  ",
    " /______\\ ",
    " |      | ",
    " |  []  | ",
    " |______| "
};

static const ascii_object house = {
    .lines = house_lines,
    .width = 9,
    .height = 6
};

static const char *bush_lines[] = {
    " *** ",
    "*****",
    " |||"
};
static const ascii_object bush = {
    .lines = bush_lines,
    .width = 5,
    .height = 3
};

#endif // TEXTURES_H

// End of textures.h