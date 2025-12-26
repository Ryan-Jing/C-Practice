/**************************************************************************************************/
/**
 * @file scenes.c
 * @author  Ryan Jing (r5jing@uwaterloo.ca)
 * @brief   Cutscene implementations
 *
 *          SCENE 1: INTRODUCTION AND LETTER (BLANK)
 *
 *          SCENE 2: GREETING
 *
 *          SCENE 3: SECOND ENCOUNTER
 *
 *          SCENE 4: BURGER KING WALK
 *
 *          SCENE 5: MORE TALKING
 *
 *          SCENE 6: BUFF RYAN
 *
 *          SCENE 7: SWINGSET
 *
 *          SCENE 8: FINAL TALK, FUTURE, GARDEN
 *
 *          SCENE 9: THE END (BLANK)
 *
 * @version 0.2
 * @date 2025-12-11
 *
 * @copyright Copyright (c) 2025
 *
 */
/**************************************************************************************************/

/*------------------------------------------------------------------------------------------------*/
/* HEADERS                                                                                        */
/*------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include "scenes.h"
#include "render.h"
#include "terminal.h"
#include "textures.h"

/*------------------------------------------------------------------------------------------------*/
/* MACROS                                                                                         */
/*------------------------------------------------------------------------------------------------*/

#define NUM_LINES_SCENE_0 3
#define NUM_LINES_SCENE_1 7
#define NUM_LINES_SCENE_2 7
#define NUM_LINES_SCENE_4 2
#define NUM_LINES_SCENE_5 3
#define NUM_LINES_SCENE_6 5
#define NUM_LINES_SCENE_7 6
#define NUM_LINES_SCENE_8 6

/*------------------------------------------------------------------------------------------------*/
/* HELPER FUNCTIONS                                                                               */
/*------------------------------------------------------------------------------------------------*/

int seconds_to_frames(float seconds)
{
    return (int)(seconds * TARGET_FPS);
}

float frames_to_seconds(int frames)
{
    return (float)frames / TARGET_FPS;
}

void draw_dialogue(char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH],
                                    const char *text, int dialogue_y, int chars_to_show)
{
    // Text rendering with word wrapping
    int box_left = 12;
    int box_right = TERMINAL_DISPLAY_WIDTH - 8;
    int max_lines = 15;

    int current_x = box_left;
    int current_line = 0;
    int chars_rendered = 0;

    for (int i = 0; text[i] != '\0' && chars_rendered < chars_to_show; i++)
    {
        char characters = text[i];

        if (characters == '\n')
        {
            current_line++;
            current_x = box_left;
            chars_rendered++;

            if (current_line >= max_lines)
                break;
            continue;
        }

        if (current_x >= box_right)
        {
            current_line++;
            current_x = box_left;

            if (current_line >= max_lines)
            {
                break;
            }
        }

        int text_y = dialogue_y + 2 + current_line;
        if (text_y < TERMINAL_DISPLAY_HEIGHT)  // Stay within bounds
        {
            terminal_display[text_y][current_x] = characters;
        }

        current_x++;
        chars_rendered++;
    }
}

/*------------------------------------------------------------------------------------------------*/
/* CUTSCENE 0: INTRODUCTION AND LETTER                                                            */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_intro(cutscene_status *cutscene, sprite *player)
{
    (void)player; // Unused parameter
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_intro(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;

    if (frames_to_seconds(cutscene->frame_timer) > 1.0f && cutscene->phase == 0)
    {
        cutscene->phase = 1;
    }

    else if (cutscene->phase == 1)
    {
        // Typewriter effect: add 2 characters every frame
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(15.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_0)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;  // Reset timer for walk-out phase
        }
    }

    else if (cutscene->phase == 2)
    {
        cutscene->finished = true;
    }
}

void render_cutscene_intro(cutscene_status *cutscene,
                           sprite *player, background_system *background)
{
    (void)player; // Unused parameter
    (void)background; // Unused parameter

    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';  // Blank screen
        }
    }

    // Draw dialogue (only during phase 1)
    if (cutscene->phase == 1)
    {
        const char *dialogues[] =
        {
            "Hi my love,\n\n"
            "I've been working away parcticing C, to make this gift for you, my most wonderful and sweet love.\n\n"
            "I want this to be a interactive gift for you, to remember our most lovely moments.\n\n"
            "I hope you can come back to this when you are feeling down, and feel my love.\n\n"
            "You are the love of my life. I love you forever.\n\n",
            "Here is our love in C.\n\n"
            "Ryan\n\n"
            "...",
            "Press q to quit, space to skip dialogue.\n\n"
            "..."

        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_0)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                         TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    if (cutscene->phase == 2)
    {
        for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
        {
            for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
            {
                terminal_display[y][x] = ' ';  // Blank screen
            }
        }
    }

    // Render
    clear_terminal_screen();

    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }

    fflush(stdout);

}



/*------------------------------------------------------------------------------------------------*/
/* CUTSCENE 1: GREETING                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_greeting(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH - 5;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->ryan_finished_walking = false;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_greeting(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg; // Unused parameterq
    cutscene->frame_timer++;

    // Phase 0: NPC walks in from right
    if (cutscene->phase == 0)
    {
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->ryan_finished_walking = true;
            cutscene->phase = 1;
            cutscene->frame_timer = 0;  // Reset timer for dialogue phase
        }
    }

    // Phase 1: Show dialogue
    else if (cutscene->phase == 1)
    {
        // Typewriter effect: add 2 characters every frame
        cutscene->dialogue_char_count += 2;

        // Auto-advance dialogue every 4 seconds, but don't override manual skips
        int dialogue_index = cutscene->frame_timer / seconds_to_frames(8.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_1)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;  // Reset timer for walk-out phase
        }
    }

    // Phase 2: NPC walks out to the left
    else if (cutscene->phase == 2)
    {
        cutscene->ryan.x -= 2;

        // When NPC is completely off-screen, end cutscene
        if (cutscene->ryan.x < 0)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_greeting(cutscene_status *cutscene,
                              sprite *player, background_system *background)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw frozen background
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < background->layers[layer].element_count; i++)
        {
            int element_x = (int)background->layers[layer].elements[i].x;
            const ascii_object *texture = background->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)background->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue (only during phase 1)
    if (cutscene->phase == 1)
    {
        const char *dialogues[] =
        {
            "Ryan: Hi my love. Welcome to this little world I've made for us.",
            "You can see it has many of the most special natural formations such as ANSII trees and mountains, all of which I designed myself. Also, press your spacebar to skip/speed the dialogue.",
            "But more importantly, I hope you can have some of our special moments for you to remember.",
            "I'd like to walk with you through some of our memorable times together, and have this storybook always with you forever and ever. \n\nYou go first, I will catch up to you.",
            "Cue the music!",
            "...",
            "i guess i haven't implemented that yet. Maybe you can add your own music!"
        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_1)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                         TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
/* CUTSCENE 2: SECOND ENCOUNTER                                                                   */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_second(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = player->x + 80;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_second(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg; // Unused parameter
    cutscene->frame_timer++;

    // Phase 0: NPC walks in from right
    if (cutscene->phase == 0)
    {
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->ryan_finished_walking = true;
            cutscene->phase = 1;
            cutscene->frame_timer = 0;  // Reset timer for dialogue phase
        }
    }

    // Phase 1: Show dialogue
    if (cutscene->phase == 1)
    {
        // Typewriter effect: add 2 characters every frame
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(8.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
        }

        // After showing both dialogues, move to walk-out phase
        if (cutscene->current_dialogue >= NUM_LINES_SCENE_2)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
        }
    }

    // Phase 2: NPC walks out to the right
    else if (cutscene->phase == 2)
    {
        cutscene->ryan.x += 2;  // Walk to the right (away from player)

        // When NPC is completely off-screen, end cutscene
        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH - 3)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_second(cutscene_status *cutscene,
                            sprite *player, background_system *background)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw frozen background
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < background->layers[layer].element_count; i++)
        {
            int element_x = (int)background->layers[layer].elements[i].x;
            const ascii_object *texture = background->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)background->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue (only during phase 1)
    if (cutscene->phase == 1)
    {
        const char *dialogues[] =
        {
            "Ryan: Oh hello again! How are you enjoying the view!\n\n",
            "You know my love, I always wanted us to walk and hike through the mountains together. I know that time will come soon. I brought you here to share about how much I love you.",
            "From the moment I met you, what I loved the most was that you were willing to listen to me. You were true, kind and fair. I felt like I knew you and had you with me.",
            "Even though we didn't see each other much in person, I always knew you were there for me",
            "After so many years of knowing each other, and even after some poor choices on my end, and our distances, I always thought of you and missed you. You were always 'my Rachell', whether in context clarifiction, or just within my own heart, I knew you were 'my Rachell'.",
            "I liked seeing you around, talking and hanging out with you. I liked calling you and hearing youre voice. \n\nI liked seeing your face when we called. Even if we didn't do much, I enjoyed just being with you.",
            "I have to get going, I'm getting a little hungry. I need to scope out what's around."
        };

        if (cutscene->current_dialogue < NUM_LINES_SCENE_2)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                         TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 3: Burger King Walk                                                                   */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_bk_walk(cutscene_status *cutscene, sprite *player)
{
    // Start Ryan off-screen to the right
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH;
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0; // Start with phase 0: Ryan walks in
}

void update_cutscene_bk_walk(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0: // Ryan walks in
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->phase = 1; // Transition to stationary dialogue
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 1: // Stationary dialogue
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(4.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 2; // Transition to walking dialogue
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2: // Walking together with dialogue
        update_background(bg, 1.5);
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 3; // Transition to final dialogue
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3: // Stop walking, final dialogue
        cutscene->dialogue_char_count += 2;
        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }
        else if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 4; // Transition to final dialogue
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 4: // Ryan walks away
        cutscene->ryan.x += 2;
        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH)
        {
            cutscene->phase = 5; // Off-screen, end the scene
        }
        break;

    case 5: // End cutscene
        cutscene->finished = true;
        break;
    }
}

void render_cutscene_bk_walk(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw background elements
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < bg->layers[layer].element_count; i++)
        {
            int element_x = (int)bg->layers[layer].elements[i].x;
            const ascii_object *texture = bg->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)bg->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    // Draw sprites
    draw_sprite(terminal_display, player);
    draw_ryan_burgerking(terminal_display, &cutscene->ryan);

    // Handle dialogue based on phase
    if (cutscene->phase == 1)
    {
        const char *dialogues[] = {
            "Ryan: Well... hm, I got hungry, so I got some Burger King. Want to keep walking and talk while I eat chicken nuggets?",
            "Rachell: ...",
            "Ryan: I will take that as a yes. Lets go!"
        };

        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }
    else if (cutscene->phase == 2)
    {
        const char *dialogues[] = {
            "Ryan: One of my most memorable moments with you before we started dating was when I got Burger King and we went out to walk around at Waterloo park.",
            "I know I missed you, I know I wanted to see you, and I definitely was not having a great time in my life at all. But you were there for me, beside me being present with me and open to listen.",
            "I don't remember everything I said, but what I remember was you being happy about me being me. You always made me feel heard and safe."
        };

        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    else if (cutscene->phase == 3)
    {
        const char *dialogues[] = {
            "Ryan: I want to do the same for you. I want to always be there for you, to support you and uplift you through any challenges you face. I want you to always feel loved, safe, and protected.",
            "When you need anyone to call, need someone to walk with, need someone to support you, I want to always be there for you.",
            "Let me go throw this out in the trash."
        };
        if (cutscene->current_dialogue < 3)
        {
            draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue], TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
        }
    }

    // Render the final buffer
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}


/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 4: Look at each other                                                                 */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_look(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH;
    cutscene->ryan.y = player->y;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_look(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)bg;
    cutscene->frame_timer++;

    if (cutscene->phase == 0) // Ryan walks in
    {
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 10)
        {
            cutscene->phase = 1; // Transition to stationary dialogue
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
    }
    else if (cutscene->phase == 1)
    {
        // Typewriter effect: add 2 characters every frame
        cutscene->dialogue_char_count += 2;

        int dialogue_index = cutscene->frame_timer / seconds_to_frames(4.0f);

        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
        }

        // After showing all dialogues, end cutscene
        if (cutscene->current_dialogue >= NUM_LINES_SCENE_4)
        {
            cutscene->finished = true;
        }
    }
}

void render_cutscene_look(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw frozen background
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < bg->layers[layer].element_count; i++)
        {
            int element_x = (int)bg->layers[layer].elements[i].x;
            const ascii_object *texture = bg->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)bg->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue
    const char *dialogues[] = {
        "Ryan: Still coming along my love? Lets go!",
        "There's just a few more things ahead I want to share with you."
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_4)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 5: Buff Ryan                                                                          */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = player->x + 9;
    cutscene->ryan.y = player->y;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}

void update_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0: // Phase 0: Dialogue
        cutscene->dialogue_char_count += 2;

        // Auto-advance dialogue every 5 seconds
        int dialogue_index = cutscene->frame_timer / seconds_to_frames(5.0f);
        if (dialogue_index > cutscene->current_dialogue)
        {
            cutscene->current_dialogue = dialogue_index;
            cutscene->dialogue_char_count = 0;  // Reset typewriter for new dialogue
        }

        if (cutscene->current_dialogue >= NUM_LINES_SCENE_5)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
        }
        break;

    case 1: // Phase 1: Ryan walks away to the right
        cutscene->ryan.x += 2;

        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH + 10)
        {
            cutscene->finished = true;
        }
        break;
    }
}

void render_cutscene_buff_ryan(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw frozen background
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < bg->layers[layer].element_count; i++)
        {
            int element_x = (int)bg->layers[layer].elements[i].x;
            const ascii_object *texture = bg->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)bg->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    draw_sprite(terminal_display, player);
    draw_ryan_buff_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue
    const char *dialogues[] = {
        "Ryan: Oh wait, also, we're kind of at the point in our story where I've been working out a bit. Take a look!",
        "Rachel: ...",
        "Ryan: Well, I hope I'm getting bigger."
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_5)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 6: Swingset                                                                           */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_swingset(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10; // Start off-screen right
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
    cutscene->swing_x = TERMINAL_DISPLAY_WIDTH + 500; // Track swing position
}

void update_cutscene_swingset(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0: // Phase 0: Ryan walks in from the right
        update_background(bg, 1.5);
        cutscene->ryan.x -= 2; // Ryan walks in from the right

        // Once Ryan reaches beside the player, transition to walking together
        if (cutscene->ryan.x <= player->x + 9)
        {
            cutscene->ryan.x = player->x + 9;
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
        }
        break;

    case 1: // Phase 1: Walking forward together, swing scrolls in, dialogue
        update_background(bg, 1.5);
        cutscene->swing_x -= 1.5f * 0.9f; // Swing moves at closest layer speed
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(5.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        // After 2 dialogue lines, transition when swing reaches characters
        if (cutscene->current_dialogue >= 2 && cutscene->swing_x <= player->x + 20)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2: // Phase 2: Standstill at swing, main conversation
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        // After 6 dialogue lines, start walking away
        else if (cutscene->current_dialogue >= 6)
        {
            cutscene->phase = 3;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3: // Phase 3: Walking away with dialogue
        update_background(bg, 1.5);
        cutscene->swing_x -= 1.5f * 0.9f; // Swing continues scrolling away
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        // After 2 dialogue lines, Ryan walks away
        else if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 4;
            cutscene->frame_timer = 0;
        }
        break;

    case 4: // Phase 4: Ryan walks away to the right (off screen right)
        cutscene->ryan.x += 2;

        if (cutscene->ryan.x > TERMINAL_DISPLAY_WIDTH + 10)
        {
            cutscene->finished = true;
        }
        break;
    }
}

void render_cutscene_swingset(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw frozen background
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < bg->layers[layer].element_count; i++)
        {
            int element_x = (int)bg->layers[layer].elements[i].x;
            const ascii_object *texture = bg->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)bg->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    // Draw swing set (at ground level)
    int swing_y = TERMINAL_DISPLAY_HEIGHT - swing_set.height - 1;
    draw_object(terminal_display, &swing_set, (int)cutscene->swing_x, swing_y);

    // Draw sprites
    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue based on phase
    const char *dialogue_text = NULL;

    if (cutscene->phase == 1)
    {
        // Walking together, swing scrolls in
        const char *dialogues_phase1[] = {
            "Ryan: Hey look, there's something up ahead!",
            "Damn I shouldn't have gotten Burger King. I don't have any popcorn chicken, just some left over chicken nuggets..."
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase1[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 2)
    {
        // Standstill at swing, main conversation
        const char *dialogues_phase2[] = {
            "Ryan: The swingset! \n\nI like to think about our first date on this couch swing, at your house. We were very young looking back. \n\nYou were the first person I saw after months of lockdown. What a wonderful feeling that was.",
            "I was so nervous to see you, and I wanted to look my best and impress you.\n\n"
            "Andddd, my idea to do that was popcorn chicken! I thought it would make you as happy as it made me.",
            "Even more crazy is that my mom dropped me off. I really got my mom to drop me off for our first date!",
            "But after you came to get me on that park bench, I got to sit beside you and talk to you on your deck.\n\n"
            "I got to spend time with you, sitting right next to you, and I loved that.",
            "Many years later, I came back to your house, but now as your boyfriend.",
            "More things will come after this moment, many many more lovely things, but this will always be our first date, the most wonderful first date."
        };
        if (cutscene->current_dialogue < 6)
        {
            dialogue_text = dialogues_phase2[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 3)
    {
        // Walking away
        const char *dialogues_phase3[] = {
            "Ryan: I have to go prepare something special for you.",
            "I'll be back soon, I promise!"
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase3[cutscene->current_dialogue];
        }
    }

    if (dialogue_text != NULL)
    {
        draw_dialogue(terminal_display, dialogue_text,
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 7: Garden                                                                             */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_garden(cutscene_status *cutscene, sprite *player)
{
    cutscene->ryan.x = TERMINAL_DISPLAY_WIDTH + 10; // Start off-screen right
    cutscene->ryan.y = player->y;
    cutscene->ryan.velocity_y = 0;
    cutscene->ryan.is_jumping = false;
    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->chain_to_next = false;
    cutscene->phase = 0;
    cutscene->garden_x = TERMINAL_DISPLAY_WIDTH + 360;
}

void update_cutscene_garden(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    cutscene->frame_timer++;

    switch (cutscene->phase)
    {
    case 0: // Phase 0: Ryan walks in from the right
        cutscene->ryan.x -= 1;
        if (cutscene->ryan.x <= player->x + 15)
        {
            cutscene->phase = 1;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 1: // Phase 1: Stop and have initial conversation (stationary)
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 2;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 2: // Phase 2: Start walking together with dialogue
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f; // Garden moves at closest layer speed
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(9.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 3;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 3: // Phase 3: Garden scrolls in, characters walk into garden
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f;

        // Transition when garden has scrolled so characters are inside it
        if (cutscene->garden_x <= player->x)
        {
            cutscene->phase = 4;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 4: // Phase 4: In the garden, stationary with dialogue
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(8.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        if (cutscene->current_dialogue >= 3)
        {
            cutscene->phase = 5;
            cutscene->frame_timer = 0;
            cutscene->current_dialogue = 0;
            cutscene->dialogue_char_count = 0;
        }
        break;

    case 5: // Phase 5: Walking away from garden with dialogue
        update_background(bg, 1.5);
        cutscene->garden_x -= 1.5f * 0.9f; // Garden continues scrolling away
        cutscene->dialogue_char_count += 2;

        if (cutscene->frame_timer > seconds_to_frames(7.0f))
        {
            cutscene->current_dialogue++;
            cutscene->dialogue_char_count = 0;
            cutscene->frame_timer = 0;
        }

        // After dialogue, transition to walking off together
        if (cutscene->current_dialogue >= 2)
        {
            cutscene->phase = 6;
            cutscene->frame_timer = 0;
        }
        break;

    case 6: // Phase 6: Both characters walk off-screen to the left together
        player->x -= 2;
        cutscene->ryan.x -= 2;

        // When both are off-screen left, finish and chain to next cutscene
        if (player->x < -10 && cutscene->ryan.x < -10)
        {
            cutscene->finished = true;
            cutscene->chain_to_next = true; // Immediately trigger next cutscene
        }
        break;
    }
}

void render_cutscene_garden(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];

    // Initialize display
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    // Draw ground
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 1][x] = '=';
    }

    // Draw background mountain base
    for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
    {
        terminal_display[TERMINAL_DISPLAY_HEIGHT - 11][x] = '.';
    }

    // Draw background elements
    for (int layer = 0; layer < NUM_LAYERS; layer++)
    {
        for (int i = 0; i < bg->layers[layer].element_count; i++)
        {
            int element_x = (int)bg->layers[layer].elements[i].x;
            const ascii_object *texture = bg->layers[layer].elements[i].texture;

            int element_y;
            if (layer == LAYER_CLOUDS)
            {
                element_y = 1 + (i % 3) * 3;
            }
            else if (layer == LAYER_MOUNTAINS)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 10;
            }
            else if (layer == LAYER_HOUSES)
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 3;
            }
            else
            {
                element_y = TERMINAL_DISPLAY_HEIGHT - texture->height - 1;
            }

            draw_object(terminal_display, texture, element_x, element_y);
        }
    }

    // Draw particles
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT - 1; y++)
    {
        if (y % 3 == 0)
        {
            int particle_x = (int)bg->particle_positions[y];
            if (particle_x >= 0 && particle_x < TERMINAL_DISPLAY_WIDTH)
            {
                terminal_display[y][particle_x] = '.';
            }
        }
    }

    // Draw garden (at ground level, phases 2-5)
    if (cutscene->phase >= 2)
    {
        int garden_y = TERMINAL_DISPLAY_HEIGHT - garden.height;
        draw_object(terminal_display, &garden, (int)cutscene->garden_x, garden_y);
    }

    // Draw sprites
    draw_sprite(terminal_display, player);
    draw_ryan_sprite(terminal_display, &cutscene->ryan);

    // Draw dialogue based on phase
    const char *dialogue_text = NULL;

    // Phase 0: Ryan walks in (no dialogue)

    if (cutscene->phase == 1)
    {
        // Initial stop conversation
        const char *dialogues_phase1[] = {
            "Ryan: Phew! You are still here. Thank you for waiting!",
            "After our first date, so much has changed until now. There were ups and downs, but eventually I was able to call you mine. My Rachell was my Rachell even more. I got to hug you, hold your hand, kiss you, and love you more and more every day.",
            "We pray for us every day, to grow ourselves and love each other more and more, and I look back and cannot see anywhere how that has not happened.",
            "My love for you has grown and grown. My care for you has deepened. My admiration for you has exploded.",
            "While sometimes times may get busy, stressful, and hard, I know we always have each other. We also have what's ahead to look forward to."
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase1[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 2)
    {
        // Walking together dialogue
        const char *dialogues_phase2[] = {
            "Ryan: Come, let's walk together.\n"
            "There's a beautiful garden up ahead I want to show you.",
            "Rachell: :)"
        };
        if (cutscene->current_dialogue < 2)
        {
            dialogue_text = dialogues_phase2[cutscene->current_dialogue];
        }
    }
    // Phase 3: Garden scrolls in (no dialogue)
    else if (cutscene->phase == 4)
    {
        // In the garden dialogue
        const char *dialogues_phase4[] = {
            "Ryan: Look! All the beggies and and all the flowers. Our garden my love.\n"
            "A wonderful land that we can grow and cultivate together.",
            "Rachell: <3",
            "Ryan: I want to stay in this garden with you forever and ever. Taking care of the plants and animals around us.\n"
            "We can plant your tomatoes, I can plant my hot peppers, and we can grow so many beautiful flowers together."
        };
        if (cutscene->current_dialogue < 3)
        {
            dialogue_text = dialogues_phase4[cutscene->current_dialogue];
        }
    }
    else if (cutscene->phase == 5)
    {
        // Walking away dialogue
        const char *dialogues_phase5[] = {
            "Ryan: Let's keep walking my love. There is more and more up ahead. So much more.\n"
            "All the milestones we will go through together. All the memories we will make together.",
            "Rachell: :)"
        };
        if (cutscene->current_dialogue < NUM_LINES_SCENE_8)
        {
            dialogue_text = dialogues_phase5[cutscene->current_dialogue];
        }
    }

    if (dialogue_text != NULL)
    {
        draw_dialogue(terminal_display, dialogue_text,
                     TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    // Render
    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            putchar(terminal_display[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------*/
// CUTSCENE 8: The End                                                                            */
/*------------------------------------------------------------------------------------------------*/

void init_cutscene_the_end(cutscene_status *cutscene, sprite *player)
{
    (void)player; // Unused parameter

    cutscene->frame_timer = 0;
    cutscene->current_dialogue = 0;
    cutscene->dialogue_char_count = 0;
    cutscene->finished = false;
    cutscene->phase = 0;
}
void update_cutscene_the_end(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    cutscene->frame_timer++;
    cutscene->dialogue_char_count += 2;

    if (cutscene->current_dialogue >= 1 && cutscene->frame_timer > seconds_to_frames(30.0f))
    {
        cutscene->finished = true; // End after final message
    }

    int dialogue_index = cutscene->frame_timer / seconds_to_frames(6.0f);

    if (dialogue_index > cutscene->current_dialogue)
    {
        cutscene->current_dialogue = dialogue_index;
        cutscene->dialogue_char_count = 0;
    }
}
void render_cutscene_the_end(cutscene_status *cutscene, sprite *player, background_system *bg)
{
    (void)player; // Unused parameter
    (void)bg;     // Unused parameter
    // Blank screen like the intro
    char terminal_display[TERMINAL_DISPLAY_HEIGHT][TERMINAL_DISPLAY_WIDTH];
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++)
        {
            terminal_display[y][x] = ' ';
        }
    }

    const char *dialogues[] =
    {
        "I want to spend the rest of my life with you my love, and I want you to always be reminded of how much I love you.",
        "No matter if you have your books, your cards, your letters, and now this, I want you to always know that I love you.",
        "Just type in ./mylove to see this again whenever and wherever you want.",
        "I love you, my love.",
        "Forever and always,\n\nRyan",
        " ",
        " "
    };

    if (cutscene->current_dialogue < NUM_LINES_SCENE_8)
    {
        draw_dialogue(terminal_display, dialogues[cutscene->current_dialogue],
                      TERMINAL_DISPLAY_HEIGHT - 30, cutscene->dialogue_char_count);
    }

    clear_terminal_screen();
    for (int y = 0; y < TERMINAL_DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < TERMINAL_DISPLAY_WIDTH; x++) putchar(terminal_display[y][x]);
        putchar('\n');
    }
    fflush(stdout);
}

// End of scenes.c
