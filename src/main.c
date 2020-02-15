/*

  ________ _____                       _________________
  ___  __ \___(_)_______ ______        __  ____/___  __ )
  __  / / /__  / __  __ \_  __ \       _  / __  __  __  |
  _  /_/ / _  /  _  / / // /_/ /       / /_/ /  _  /_/ /
  /_____/  /_/   /_/ /_/ \____/        \____/   /_____/

  In Progress!

*/

#include <time.h>       // time_t, clock_t, time(), clock()
#include <rand.h>       // rand(), initrand()
#include <gb/gb.h>      // Main GBDK import header
#include <gb/drawing.h> // GRAPHICS_HEIGHT, GRAPHICS_WIDTH

#include "graphics/dino.c"               // Dino sprites
#include "graphics/letters.c"            // Letter sprites 
#include "graphics/digits.c"             // Digit sprites
#include "graphics/hazards.c"            // Cactus sprites
#include "graphics/gameover.c"           // "H", "I", etc. sprites
#include "graphics/background_tiles.c"   // Clouds, ground sprites
#include "graphics/background_map.c"     // Background map file

/* --- MACROS START --- */
// Dino VRAM tile indices
#define IDLE_DINO_1_TILE 0x00U
#define IDLE_DINO_2_TILE 0x02U
#define HURT_DINO_1_TILE 0x04U
#define HURT_DINO_2_TILE 0x06U
#define RIGHT_FOOT_DINO_1_TILE 0x08U
#define RIGHT_FOOT_DINO_2_TILE 0x0AU
#define LEFT_FOOT_DINO_1_TILE 0x0CU
#define LEFT_FOOT_DINO_2_TILE 0X0EU
#define DUCK_DINO_1_TILE 0x10U
#define DUCK_DINO_2_TILE 0x12U
#define DUCK_DINO_3_TILE 0x14U

// Dino Tile Indices
#define IDLE_DINO_1 0x00U
#define IDLE_DINO_2 0x01U
#define HURT_DINO_1 0x04U
#define HURT_DINO_2 0x06U
#define RIGHT_STEP_DINO_1 0x08U
#define RIGHT_STEP_DINO_2 0x0AU
#define LEFT_STEP_DINO_1 0x0CU
#define LEFT_STEP_DINO_2 0x0EU
#define DUCK_DINO_BODY 0x10U
#define DUCK_DINO_HEAD 0x12U
#define DUCK_DINO_TAIL 0x14U
#define DUCK_DINO_LEFT 0x16U
#define DUCK_DINO_RIGHT 0x1AU

// Dino Sprite Indices
#define DINO_1 0x00U
#define DINO_2 0x01U
#define DINO_3 0x03U
#define DINO_4 0x04U

// Digit Tile Indices
#define ZERO_1 0x1CU
#define ZERO_2 0x1EU
#define ONE_1 0x20U
#define ONE_2 0x22U
#define TWO_1 0x24U
#define TWO_2 0x26U
#define THREE_1 0x28U
#define THREE_2 0x2AU
#define FOUR_1 0x2CU
#define FOUR_2 0x2EU
#define FIVE_1 0x30U
#define FIVE_2 0x32U
#define SIX_1 0x34U
#define SIX_2 0x36U
#define SEVEN_1 0x38U
#define SEVEN_2 0x3AU
#define EIGHT_1 0x3CU
#define EIGHT_2 0x3EU
#define NINE_1 0x40U
#define NINE_2 0x42U

// Digit Sprite Indices
#define SCORE_1_1 0x0AU
#define SCORE_1_2 0x0BU
#define SCORE_2_1 0x0CU
#define SCORE_2_2 0x0DU
#define SCORE_3_1 0x0EU
#define SCORE_3_2 0x0FU
#define SCORE_4_1 0x10U
#define SCORE_4_2 0x11U
#define SCORE_5_1 0x12U
#define SCORE_5_2 0x13U

#define HI_SCORE_1_1 0x14U
#define HI_SCORE_1_2 0x15U
#define HI_SCORE_2_1 0x16U
#define HI_SCORE_2_2 0x17U
#define HI_SCORE_3_1 0x18U
#define HI_SCORE_3_2 0x19U
#define HI_SCORE_4_1 0x1AU
#define HI_SCORE_4_2 0x1BU
#define HI_SCORE_5_1 0x1CU
#define HI_SCORE_5_2 0x1DU

// Letter Sprite Indices
#define H_1 0x05U
#define H_2 0x06U
#define I_1 0x07U
#define I_2 0x08U
#define H_1_TILE 0x68U
#define H_2_TILE 0x6AU
#define I_1_TILE 0x6CU
#define I_2_TILE 0x6EU

// Hazard Tile Indices
#define CACTUS_1_TILE 0x44U
#define CACTUS_2_TILE 0x46U

// Hazard Sprite Indices
#define CACTUS_1 0x1EU
#define CACTUS_2 0x1FU

/* Sprite tile sizes */
#define DINO_TILE_COUNT 0x1CU
#define HAZARDS_TILE_COUNT 0x04U
#define LETTERS_TILE_COUNT 0x2CU
#define DIGITS_TILE_COUNT 0x28U
#define BACKGROUND_TILE_COUNT 0x20U

// Positioning and movement macros
#define GRAVITY 0x06U
#define CLOCK_PER_DECISEC CLOCKS_PER_SEC / 10

// X, Y positioning
#define DEFAULT_DINO_X GRAPHICS_WIDTH - 120
#define DEFAULT_DINO_Y GRAPHICS_HEIGHT - 15
#define HI_SCORE_X 0x54U
#define HI_SCORE_Y 0x28U
#define SCORE_X 0x54U
#define SCORE_Y 0x16U
#define GAME_OVER_X 0x28U
#define GAME_OVER_Y 0x32U

// Used for the intro, where only a small patch of ground is visible
#define GROUND_PATCH_1 0x07U
#define GROUND_PATCH_2 0x08U
#define GROUND_PATCH_3 0x09U
#define GROUND_PATCH_1_TILE 0x4BU
#define GROUND_PATCH_2_TILE 0x4DU
#define GROUND_PATCH_3_TILE 0x51U
#define GROUND_PATCH_X GRAPHICS_WIDTH - 141
#define GROUNG_PATCH_Y GRAPHICS_HEIGHT - 16

#define SCORE_DIGITS 0x05U // i.e. 5 digits: 00000

/* Extra macros */
#define DINO_SPRITE_X_SIZE 0x08U
#define DINO_FOOT_RIGHT_DOWN 0x00U
#define DINO_FOOT_LEFT_DOWN 0x01U
#define DINO_JUMPING 0x02U
#define BLANK_TILE 0x48U   // Useful "blank" or invisible tile
/* --- MACROS END --- */

/* --- GLOBALS START --- */
UBYTE dino_x, dino_y;          // Store Dino Sprite coordinates
UBYTE bkg_x;                   // Background X coordinate for scrolling
UBYTE dino_state;              // 0 == right foot down, 1 == left foot down, 2 == jumping
UBYTE button_press;            // Hold the last return value of the joypad() function
UBYTE prev_button_press;       // Hold the next-to-last return value of the joypad() function
UBYTE i;                       // 8 bit reusable counter variable
UBYTE digit_left, digit_right; // Left and right sprite index of a digit in the meter (score)
UBYTE tmp_digit;               // Temp digit used for holding a digit index while calculating score
UBYTE cactus_x, cactus_y;      // Store nearest Cactus hazard coordinates
UBYTE cactus_speed;            // Speed of incoming cactus, controlled by difficulty
UBYTE velocity;                // y-axis velocity vector for jumping
UBYTE speed_drop_coefficient;  // A coefficient for dropping faster from a jump
UBYTE timer;                   // Ticks ever iteration of the main game loop
UBYTE hazards_on_screen;       // Counts the amount of current hazards on screen


BOOLEAN high_score_toggle;     // Toggle for whether or not to show the high score sprites
BOOLEAN playing_score_sound;   // Indicates if the score jingle is playing, as it takes priority

ULWORD j;               // 32 bit reusable variable
ULWORD current_score;   // Store a 32 bit integer representation of the current score
ULWORD high_score;      // Store 32 bit integer representation of the high score

time_t time_game_started;                // Seconds since the CPU has started running
clock_t clock_jump_start, clock_delta;   // For keeping track of jumps and updating the position

// Variables in which to save the score sprite indexes
UBYTE SCORE_1_1_TILE, SCORE_1_2_TILE,
      SCORE_2_1_TILE, SCORE_2_2_TILE,
      SCORE_3_1_TILE, SCORE_3_2_TILE,
      SCORE_4_1_TILE, SCORE_4_2_TILE,
      SCORE_5_1_TILE, SCORE_5_2_TILE;

/* --- GLOBALS END --- */

/* --- PROTOTYPES START --- */
void setup_score_sprites();
void setup_hi_score_sprites();
void update_score(UBYTE);
void set_high_score();
BOOLEAN new_high_score();
UWORD get_multiplier(UBYTE);
ULWORD score_sprite_to_num(UBYTE);
UBYTE num_to_left_score_sprite(UBYTE);
UBYTE num_to_right_score_sprite(UBYTE);
void play_jump_sound();
void play_death_sound();
void play_score_sound();
// void flash_score();
void spawn_hazard();
void move_hazards();
void move_background();
BOOLEAN check_collisions(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void start_jump();
void game_over();
void restart();
void run_game();
void update();
void render();
/* --- PROTOTYPES END --- */

/* --- FUNCTIONS START --- */
void setup_score_sprites()
{
    /* Using sprites to keep a visual score. Changing sprites simulates increasing the score. */

    // Load both parts (because it is 8x16 pixels in total) of the ZERO sprite
    // into all five corresponding digit positions
    set_sprite_tile(SCORE_1_1, ZERO_1);
    set_sprite_tile(SCORE_1_2, ZERO_2);
    set_sprite_tile(SCORE_2_1, ZERO_1);
    set_sprite_tile(SCORE_2_2, ZERO_2);
    set_sprite_tile(SCORE_3_1, ZERO_1);
    set_sprite_tile(SCORE_3_2, ZERO_2);
    set_sprite_tile(SCORE_4_1, ZERO_1);
    set_sprite_tile(SCORE_4_2, ZERO_2);
    set_sprite_tile(SCORE_5_1, ZERO_1);
    set_sprite_tile(SCORE_5_2, ZERO_2);

    // Move the digits into (X, Y) position on the screen,
    // each 8 pixels further right on the X-axis
    move_sprite(SCORE_1_1, SCORE_X, SCORE_Y);
    move_sprite(SCORE_1_2, SCORE_X + 0x08U, SCORE_Y);
    move_sprite(SCORE_2_1, SCORE_X + 0x10U, SCORE_Y);
    move_sprite(SCORE_2_2, SCORE_X + 0x18U, SCORE_Y);
    move_sprite(SCORE_3_1, SCORE_X + 0x20U, SCORE_Y);
    move_sprite(SCORE_3_2, SCORE_X + 0x28U, SCORE_Y);
    move_sprite(SCORE_4_1, SCORE_X + 0x30U, SCORE_Y);
    move_sprite(SCORE_4_2, SCORE_X + 0x38U, SCORE_Y);
    move_sprite(SCORE_5_1, SCORE_X + 0x40U, SCORE_Y);
    move_sprite(SCORE_5_2, SCORE_X + 0x48U, SCORE_Y);
}

void setup_hi_score_sprites()
{
    /* Using sprites to keep a visual score. Changing sprites simulates increasing the score. */

    // Load the both parts of the ZERO sprite (two parts because it is 8x16 pixels in total)
    // into all five corresponding digit positions
    set_sprite_tile(HI_SCORE_1_1, num_to_left_score_sprite((high_score / 10000U) % 10));
    set_sprite_tile(HI_SCORE_1_2, num_to_right_score_sprite((high_score / 10000U) % 10));
    set_sprite_tile(HI_SCORE_2_1, num_to_left_score_sprite((high_score / 1000U) % 10));
    set_sprite_tile(HI_SCORE_2_2, num_to_right_score_sprite((high_score / 1000U) % 10));
    set_sprite_tile(HI_SCORE_3_1, num_to_left_score_sprite((high_score / 100U) % 10));
    set_sprite_tile(HI_SCORE_3_2, num_to_right_score_sprite((high_score / 100U) % 10));
    set_sprite_tile(HI_SCORE_4_1, num_to_left_score_sprite((high_score / 10U) % 10));
    set_sprite_tile(HI_SCORE_4_2, num_to_right_score_sprite((high_score / 10U) % 10));
    set_sprite_tile(HI_SCORE_5_1, num_to_left_score_sprite(high_score % 10U));
    set_sprite_tile(HI_SCORE_5_2, num_to_right_score_sprite(high_score % 10U));

    // The high score sprites use the second, lighter palette
    set_sprite_prop(HI_SCORE_1_1, S_PALETTE);
    set_sprite_prop(HI_SCORE_1_2, S_PALETTE);
    set_sprite_prop(HI_SCORE_2_1, S_PALETTE);
    set_sprite_prop(HI_SCORE_2_2, S_PALETTE);
    set_sprite_prop(HI_SCORE_3_1, S_PALETTE);
    set_sprite_prop(HI_SCORE_3_2, S_PALETTE);
    set_sprite_prop(HI_SCORE_4_1, S_PALETTE);
    set_sprite_prop(HI_SCORE_4_2, S_PALETTE);
    set_sprite_prop(HI_SCORE_5_1, S_PALETTE);
    set_sprite_prop(HI_SCORE_5_2, S_PALETTE);

    // Move the digits into (X, Y) position on the screen,
    // each 8 pixels further right on the X-axis
    move_sprite(HI_SCORE_1_1, HI_SCORE_X, HI_SCORE_Y);
    move_sprite(HI_SCORE_1_2, HI_SCORE_X + 0x08U, HI_SCORE_Y);
    move_sprite(HI_SCORE_2_1, HI_SCORE_X + 0x10U, HI_SCORE_Y);
    move_sprite(HI_SCORE_2_2, HI_SCORE_X + 0x18U, HI_SCORE_Y);
    move_sprite(HI_SCORE_3_1, HI_SCORE_X + 0x20U, HI_SCORE_Y);
    move_sprite(HI_SCORE_3_2, HI_SCORE_X + 0x28U, HI_SCORE_Y);
    move_sprite(HI_SCORE_4_1, HI_SCORE_X + 0x30U, HI_SCORE_Y);
    move_sprite(HI_SCORE_4_2, HI_SCORE_X + 0x38U, HI_SCORE_Y);
    move_sprite(HI_SCORE_5_1, HI_SCORE_X + 0x40U, HI_SCORE_Y);
    move_sprite(HI_SCORE_5_2, HI_SCORE_X + 0x48U, HI_SCORE_Y);
}

void update_score(UBYTE digit_pos)
{
    // Note: This is a recursive function

    // Store the current digit sprite index, which is comprised of
    // two 8x16 sprites, a left and a right half.
    digit_left = SCORE_1_1 + ((digit_pos - 0x01U) << 0x01U);
    digit_right = SCORE_1_2 + ((digit_pos - 0x01U) << 0x01U);

    /*
      Example of the equation for obtaining the index of the left
      side of the 4th digit:
      digit_left = SCORE_1_1   [macro defined as 0x0A]
        + ((digit_pos - 1)     [digit_pos was 4, so we have 3]
        << 1)                  [bit shift left by 1 means multiply by 2, so we have 6]
        ------------------------------------------------
        = 0x0A + 0x06 = 0x10 = SCORE_4_1
    */

    switch (get_sprite_tile(digit_right)) {
    case ZERO_2:
        set_sprite_tile(digit_left, ONE_1);
        set_sprite_tile(digit_right, ONE_2);
        break;
    case ONE_2:
        set_sprite_tile(digit_left, TWO_1);
        set_sprite_tile(digit_right, TWO_2);
        break;
    case TWO_2:
        set_sprite_tile(digit_left, THREE_1);
        set_sprite_tile(digit_right, THREE_2);
        break;
    case THREE_2:
        set_sprite_tile(digit_left, FOUR_1);
        set_sprite_tile(digit_right, FOUR_2);
        break;
    case FOUR_2:
        set_sprite_tile(digit_left, FIVE_1);
        set_sprite_tile(digit_right, FIVE_2);
        break;
    case FIVE_2:
        set_sprite_tile(digit_left, SIX_1);
        set_sprite_tile(digit_right, SIX_2);
        break;
    case SIX_2:
        set_sprite_tile(digit_left, SEVEN_1);
        set_sprite_tile(digit_right, SEVEN_2);
        break;
    case SEVEN_2:
        set_sprite_tile(digit_left, EIGHT_1);
        set_sprite_tile(digit_right, EIGHT_2);
        break;
    case EIGHT_2:
        set_sprite_tile(digit_left, NINE_1);
        set_sprite_tile(digit_right, NINE_2);
        break;
    case NINE_2:
        set_sprite_tile(digit_left, ZERO_1);
        set_sprite_tile(digit_right, ZERO_2);

        // If this position is a carry position, move on to the next digit
        if (digit_pos != 0x01U) {

            // Perform a recursion on the next digit (to the left)
            update_score(--digit_pos);

            // If this digit is a multiple of 100, play a sound and flash the screen
            if (digit_pos == 0x03U) {
                play_score_sound();
                // flash_score();
            }
        }

        // else Catch overflow here?
        break; // DEBUG Does this break statement "break" tail recursion...?
    }
}

void set_high_score()
{
    /* Checks if the current score is indeed the highest. Sets a BOOLEAN */

    high_score = current_score;

    if (!high_score_toggle)
        high_score_toggle = TRUE;
}

BOOLEAN new_high_score()
{
    /*  */
    j = 0x00U;
    current_score = score_sprite_to_num(0x05U);
    return current_score > high_score ? TRUE : FALSE;
}

UWORD get_multiplier(UBYTE digit_pos)
{
    switch (digit_pos) {
    case 0x05U:
        return 1;

    case 0x04U:
        return 10;

    case 0x03U:
        return 100;

    case 0x02U:
        return 1000;

    case 0x01U:
        return 10000;
    }
}

ULWORD score_sprite_to_num(UBYTE digit_pos)
{

    // This tail-recursive function returns a ULWORD (32 bit) representation of the current score
    // Reusing j as an accumulator here, which was reset before the call to this function

    if (digit_pos == 0x00U)
        return j;

    // For an explanation of the formula used here, check out the update_score() function docstring
    digit_right = SCORE_1_2 + ((digit_pos - 0x01U) << 0x01U);

    tmp_digit = get_sprite_tile(digit_right);

    if (tmp_digit == ZERO_2)
        return score_sprite_to_num(--digit_pos);

    else if (tmp_digit == ONE_2) {
        j += (get_multiplier(digit_pos) * 0x01U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == TWO_2) {
        j += (get_multiplier(digit_pos) * 0x02U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == THREE_2) {
        j += (get_multiplier(digit_pos) * 0x03U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == FOUR_2) {
        j += (get_multiplier(digit_pos) * 0x04U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == FIVE_2) {
        j += (get_multiplier(digit_pos) * 0x05U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == SIX_2) {
        j += (get_multiplier(digit_pos) * 0x06U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == SEVEN_2) {
        j += (get_multiplier(digit_pos) * 0x07U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == EIGHT_2) {
        j += (get_multiplier(digit_pos) * 0x08U);
        return score_sprite_to_num(--digit_pos);
    }

    else if (tmp_digit == NINE_2) {
        j += (get_multiplier(digit_pos) * 0x09U);
        return score_sprite_to_num(--digit_pos);
    }
}

UBYTE num_to_left_score_sprite(UBYTE digit)
{
    // This is the left-side-sprite inverse function to score_sprite_to_num()

    switch (digit) {
    case 0x00U:
        return ZERO_1;
    case 0x01U:
        return ONE_1;
    case 0x02U:
        return TWO_1;
    case 0x03U:
        return THREE_1;
    case 0x04U:
        return FOUR_1;
    case 0x05U:
        return FIVE_1;
    case 0x06:
        return SIX_1;
    case 0x07:
        return SEVEN_1;
    case 0x08:
        return EIGHT_1;
    case 0x09:
        return NINE_1;
    }
}

UBYTE num_to_right_score_sprite(UBYTE digit)
{
    // This is the right-side sprite-inverse function to score_sprite_to_num()

    switch (digit) {
    case 0x00:
        return ZERO_2;
    case 0x01U:
        return ONE_2;
    case 0x02U:
        return TWO_2;
    case 0x03U:
        return THREE_2;
    case 0x04U:
        return FOUR_2;
    case 0x05U:
        return FIVE_2;
    case 0x06U:
        return SIX_2;
    case 0x07U:
        return SEVEN_2;
    case 0x08U:
        return EIGHT_2;
    case 0x09U:
        return NINE_2;
    }
}

void play_jump_sound()
{
    NR10_REG = 0x79;
    NR11_REG = 0x8A;
    NR12_REG = 0xA1;

    NR13_REG = 0xE0;
    NR14_REG = 0xC6;
}

void play_death_sound()
{
    NR10_REG = 0x70;
    NR11_REG = 0x8A;
    NR12_REG = 0x44;
    NR13_REG = 0xF4;
    NR14_REG = 0x81;

    delay(100);

    NR10_REG = 0x70;
    NR11_REG = 0x8A;
    NR12_REG = 0x44;
    NR13_REG = 0xC8;
    NR14_REG = 0x80;
}

void play_score_sound()
{
    playing_score_sound = TRUE;

    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0x63;
    NR13_REG = 0x03;
    NR14_REG = 0x87;

    // delay goes here...

    NR10_REG = 0x00;
    NR11_REG = 0x80;
    NR12_REG = 0x63;
    NR13_REG = 0x53;
    NR14_REG = 0x87;

    playing_score_sound = FALSE;
}

// void flash_score()
// {
//   // Save the current score sprites
//   SCORE_1_1_TILE = get_sprite_tile(SCORE_1_1);
//   SCORE_1_2_TILE = get_sprite_tile(SCORE_1_2);
//   SCORE_2_1_TILE = get_sprite_tile(SCORE_2_1);
//   SCORE_2_2_TILE = get_sprite_tile(SCORE_2_2);
//   SCORE_3_1_TILE = get_sprite_tile(SCORE_3_1);
//   SCORE_3_2_TILE = get_sprite_tile(SCORE_3_2);
//   SCORE_4_1_TILE = get_sprite_tile(SCORE_4_1);
//   SCORE_4_2_TILE = get_sprite_tile(SCORE_4_2);
//   SCORE_5_1_TILE = get_sprite_tile(SCORE_5_1);
//   SCORE_5_2_TILE = get_sprite_tile(SCORE_5_2);

//   for (i = 0; i < 3; i++)
//   {
//     set_sprite_tile(SCORE_1_1, BLANK_TILE);
//     set_sprite_tile(SCORE_1_2, BLANK_TILE);
//     set_sprite_tile(SCORE_2_1, BLANK_TILE);
//     set_sprite_tile(SCORE_2_2, BLANK_TILE);
//     set_sprite_tile(SCORE_3_1, BLANK_TILE);
//     set_sprite_tile(SCORE_3_2, BLANK_TILE);
//     set_sprite_tile(SCORE_4_1, BLANK_TILE);
//     set_sprite_tile(SCORE_4_2, BLANK_TILE);
//     set_sprite_tile(SCORE_5_1, BLANK_TILE);
//     set_sprite_tile(SCORE_5_2, BLANK_TILE);

//     delay(500);

//     set_sprite_tile(SCORE_1_1, SCORE_1_1_TILE);
//     set_sprite_tile(SCORE_1_2, SCORE_1_2_TILE);
//     set_sprite_tile(SCORE_2_1, SCORE_2_1_TILE);
//     set_sprite_tile(SCORE_2_2, SCORE_2_2_TILE);
//     set_sprite_tile(SCORE_3_1, SCORE_3_1_TILE);
//     set_sprite_tile(SCORE_3_2, SCORE_3_2_TILE);
//     set_sprite_tile(SCORE_4_1, SCORE_4_1_TILE);
//     set_sprite_tile(SCORE_4_2, SCORE_4_2_TILE);
//     set_sprite_tile(SCORE_5_1, SCORE_5_1_TILE);
//     set_sprite_tile(SCORE_5_2, SCORE_5_2_TILE);

//     delay(500);
//   }

//   // Simulates time passing and score increasing
//   // while the score flashes by moving the sprites ahead
//   set_sprite_tile(SCORE_4_1, TWO_1);
//   set_sprite_tile(SCORE_4_2, TWO_2);
//   set_sprite_tile(SCORE_5_1, FIVE_1);
//   set_sprite_tile(SCORE_5_2, FIVE_2);
// }

void spawn_hazard()
{
    /* */
    set_sprite_tile(CACTUS_1, CACTUS_1_TILE);
    set_sprite_tile(CACTUS_2, CACTUS_2_TILE);

    cactus_x = GRAPHICS_WIDTH - 0x08U;
    cactus_y = DEFAULT_DINO_Y;

    cactus_speed = ((UBYTE)rand() + 0x80U) % 0x03U + 0x02U;
    hazards_on_screen++;
}

void move_hazards()
{
    if (hazards_on_screen == 0x01U) {
        if (cactus_x == 0x00U) {
            hazards_on_screen--;
        }

        move_sprite(CACTUS_1, cactus_x, cactus_y);
        move_sprite(CACTUS_2, cactus_x + 0x08U, cactus_y);
        cactus_x -= cactus_speed;
    }

    if (hazards_on_screen == 0x00U) {
        move_sprite(CACTUS_1, 0x00U, 0x00U);
        move_sprite(CACTUS_2, 0x00U, 0x00U);
    }

    // Else, there are two hazards on screen
    // else {

    // }

}

void move_background()
{
    //
    if (bkg_x > GRAPHICS_WIDTH)
        bkg_x = 0x00U;

    // Simulate the scrolling of the background by moving its x-axis focus on a timer
    if (timer % 0x0AU == 0) {
        ++bkg_x;
        scroll_bkg(bkg_x, 0x00U);
    }
}

BOOLEAN check_collisions(UBYTE x_1, UBYTE y_1, UBYTE w_1, UBYTE h_1,
                         UBYTE x_2, UBYTE y_2, UBYTE w_2, UBYTE h_2)
{

    // Standard rectangle-to-rectangle collision check
    if ((x_1 < (x_2 + w_2)) && ((x_1 + w_1) > x_2) &&
            (y_1 < (h_2 + y_2)) && ((y_1 + h_1) > y_2))
        return 0x01U;
    else
        return 0x00U;
}

void start_jump()
{
    // Runs after the player presses the A buton after the game boots

    // Move up...
    for (i = DEFAULT_DINO_Y; i != DEFAULT_DINO_Y - 0x28U; --i) {
        move_sprite(DINO_1, dino_x, i);
        move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, i);
        delay(5);
    }

    // Hold the jump...
    delay(60);

    // Move down...
    for (i; i != DEFAULT_DINO_Y; ++i) {
        move_sprite(DINO_1, dino_x, i);
        move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, i);
        delay(5);
    }

    // Move the ground sprites out of display for this session of the game
    move_sprite(GROUND_PATCH_1, 0x00, 0x00);
    move_sprite(GROUND_PATCH_2, 0x00, 0x00);
    move_sprite(GROUND_PATCH_3, 0x00, 0x00);

    // Start score display
    setup_score_sprites();

    // Display the background
    SHOW_BKG;

    // Set dino_state as 2 so that move_dino() changes it to 1 (right foot first)
    dino_state = DINO_FOOT_LEFT_DOWN;
}

void game_over()
{
    // Store the current score at game over
    SCORE_1_1_TILE = get_sprite_tile(SCORE_1_1);
    SCORE_1_2_TILE = get_sprite_tile(SCORE_1_2);
    SCORE_2_1_TILE = get_sprite_tile(SCORE_2_1);
    SCORE_2_2_TILE = get_sprite_tile(SCORE_2_2);
    SCORE_3_1_TILE = get_sprite_tile(SCORE_3_1);
    SCORE_3_2_TILE = get_sprite_tile(SCORE_3_2);
    SCORE_4_1_TILE = get_sprite_tile(SCORE_4_1);
    SCORE_4_2_TILE = get_sprite_tile(SCORE_4_2);
    SCORE_5_1_TILE = get_sprite_tile(SCORE_5_1);
    SCORE_5_2_TILE = get_sprite_tile(SCORE_5_2);

    // Switch to the hurt dino sprites
    set_sprite_tile(DINO_1, HURT_DINO_1);
    set_sprite_tile(DINO_2, HURT_DINO_2);

    // Move the game over window to position and display it
    move_win(GAME_OVER_X, GAME_OVER_Y);

    // Display the game over window
    SHOW_WIN;

    play_death_sound();

    // Check to see if there is a new high score
    if (new_high_score()) {
        set_high_score();
        setup_hi_score_sprites();
    }

    // Wait for user to acknowledge game over
    waitpad(J_A);

    // We know this will run with the A button pressed because of the previous line
    // Now we wait for the user to let go of the button so that the Dino doesn't jump on restart!
    waitpadup();

    // Start the game again!
    restart();
}

void restart()
{
    // Hide all graphical objects while we reset them
    HIDE_SPRITES;
    HIDE_WIN;
    HIDE_BKG;

    // Flush the score sprites
    setup_score_sprites();

    // Reset the dino's starting position
    dino_x = DEFAULT_DINO_X;
    dino_y = DEFAULT_DINO_Y;
    dino_state = DINO_FOOT_LEFT_DOWN;

    // Reset the background
    bkg_x = 0x00U;

    // Move the Dino back into starting position with the starting idle sprite
    set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
    set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

    move_sprite(DINO_1, dino_x, dino_y);
    move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

    if (hazards_on_screen != 0x00U) {
        cactus_x = 0x00U;
        cactus_y =  0x00U;
        move_sprite(CACTUS_1, cactus_x, cactus_y);
        move_sprite(CACTUS_2, cactus_x + 8, cactus_y);
        hazards_on_screen = 0x00U;
    }

    // Bring the sprites and background back
    SHOW_SPRITES;
    SHOW_BKG;

    // Store the seconds since the system started
    time_game_started = time(NULL);

    // Start the game loop
    run_game();
}

void run_game()
{
    // Initialize a helpful timer for managing timing logic
    timer = 0x00U;

    while (TRUE) {

        // Increment timer each time through the loop;
        ++timer;

        // Reset the timer after 255 ticks
        if (timer == 0xFFU)
            timer = 0x00U;

        // By checking the previous button press and comparing to the current button press
        // we can confirm that the button has not been held down (a dirty hack)
        // prev_button_press = button_press;
        button_press = joypad();

        // We want to jump if the A button is pressed and only if the Dino was not already jumping
        if (button_press & J_A && dino_state != DINO_JUMPING) {

            // Check if this is the first jump
            if (clock_jump_start == 0xFFFFU)
                clock_jump_start = clock();

            // Initial jump velocity is set as an impulse
            velocity = 0x03U;
            dino_state = DINO_JUMPING;

            // Change the sprite to Idle Dino for jump
            set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
            set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

            // Play the jump sound
            play_jump_sound();

            // Store the time when the Dino starts jumping to use a time delta as
            // a factor in calculating velocity for the jump
            clock_jump_start = clock();
        }

        // Run game update logic
        update();

        // Wait for V_BLANK interrupt
        wait_vbl_done();

        // Run game render logic
        render();
    }
}

void update()
{
    /* Perform particular logic based on the state of the Dino */

    // We use clock() instead of time() for a 100x increase in precision
    clock_delta = clock() - clock_jump_start;

    // DEBUG: Game over on command!
    if (joypad() & J_LEFT)
        game_over();

    // Handle Dino jumping or falling
    if (dino_state == DINO_JUMPING) {
        velocity -= (GRAVITY * (clock_delta / CLOCK_PER_DECISEC) * speed_drop_coefficient);
        dino_y -= velocity;

        // Hit the "floor"
        if (dino_y > DEFAULT_DINO_Y) {
            dino_y = DEFAULT_DINO_Y;
            dino_state = DINO_FOOT_LEFT_DOWN;
            velocity = 0x00U;
            speed_drop_coefficient = 0x01U;
        }

        // Hit max jump height
        if (dino_y < DEFAULT_DINO_Y - 40U) {
            dino_y = DEFAULT_DINO_Y - 40U;
            velocity = -0x02U;
        }
    }

    // Handle Dino collision with hazards (only one for now)
    if (check_collisions(dino_x, dino_y, 0x0CU, 0x0CU, cactus_x, cactus_y, 0x08U, 0x08U) == 0x01U) {
        game_over();
    }

    // There is a chance to spawn a hazard on every 10 ticks of the timer
    // It will trigger if there are less than two hazards on screen and if random number is even
    if (timer % 0x14U == 0x00U && hazards_on_screen != 0x02U && (rand() % 0x02U)) {

        // DEBUG, ONLY WANT 1 HAZARD FOR NOW
        if (hazards_on_screen == 0x00U && (time(NULL) - time_game_started >= 0x02U))
            spawn_hazard();
    }
}

void render()
{
    // Move the Dino if its X or Y position have changed
    move_sprite(DINO_1, dino_x, dino_y);
    move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

    // Simulate the running animation by switching feet on a timer
    if (timer % 0x06U == 0x00U)
        if (dino_state == DINO_FOOT_LEFT_DOWN) {
            set_sprite_tile(DINO_1, RIGHT_FOOT_DINO_1_TILE);
            set_sprite_tile(DINO_2, RIGHT_FOOT_DINO_2_TILE);
            dino_state = DINO_FOOT_RIGHT_DOWN;
        }

        else if (dino_state == DINO_FOOT_RIGHT_DOWN) {
            set_sprite_tile(DINO_1, LEFT_FOOT_DINO_1_TILE);
            set_sprite_tile(DINO_2, LEFT_FOOT_DINO_2_TILE);
            dino_state = DINO_FOOT_LEFT_DOWN;
        }

    // Handle updating score from rightmost digit
    if (timer % 10 == 0)
        update_score(SCORE_DIGITS);

    // Scroll hazards to the left
    if (hazards_on_screen == 0x01U || 0x02U)
        move_hazards();

    // Scroll the background
    move_background();
}


int main()
{

    // Disable interrupts as they are not being used
    disable_interrupts();

    // Hide graphical objects while the game is being set up
    HIDE_WIN;
    HIDE_BKG;
    HIDE_SPRITES;

    // Turn on sound
    NR52_REG = 0x80;

    // Crank up the volume
    NR50_REG = 0x77;

    // Select sound channel 1
    NR51_REG = 0x11;

    // Use two 8x8 sprites stacked on top of one another
    SPRITES_8x16;

    dino_x = DEFAULT_DINO_X; // Set dino_x position (hardcoded) as 20
    dino_y = DEFAULT_DINO_Y; // Y position is relative to the bottom of the screen

    // Load all the Dino sprites at once
    set_sprite_data(0x00U, DINO_TILE_COUNT, Dino);

    // Load and place digit sprites after dino sprites
    set_sprite_data(DINO_TILE_COUNT, DIGITS_TILE_COUNT, Digits);

    // Load and place the hazard sprites after the UI symbols
    set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT, HAZARDS_TILE_COUNT, Hazards);

    // Load some background tiles as sprites in order to place some of them at the start
    set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT + HAZARDS_TILE_COUNT, 0x0BU, BackgroundTiles);

    // The dino sprite is comprised of 2 smaller sprites that make one 8x16 sprite
    // Load both parts of the Dino sprite into the first two tile slots
    set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
    set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

    // Set a small patch of ground as visible to simulate the background before being drawn in
    set_sprite_tile(GROUND_PATCH_1, GROUND_PATCH_1_TILE);
    set_sprite_tile(GROUND_PATCH_2, GROUND_PATCH_2_TILE);
    set_sprite_tile(GROUND_PATCH_3, GROUND_PATCH_3_TILE);

    // Load the tiles used to write GAME OVER on a window
    set_win_data(0x00U, LETTERS_TILE_COUNT, Letters);
    set_win_tiles(0x00U, 0x00U, GameoverWidth, GameoverHeight, Gameover);

    // Set the position of the background X coordinate, load the data and set the tiles
    bkg_x = 0x00U;
    set_bkg_data(LETTERS_TILE_COUNT, 0x20U, BackgroundTiles);
    set_bkg_tiles(bkg_x, 0x00U, BackgroundMapWidth, BackgroundMapHeight, BackgroundMap);

    // Move the ground sprites into position below the Dino
    move_sprite(GROUND_PATCH_1, GROUND_PATCH_X, GROUNG_PATCH_Y);
    move_sprite(GROUND_PATCH_2, GROUND_PATCH_X + 0x08U, GROUNG_PATCH_Y);
    move_sprite(GROUND_PATCH_3, GROUND_PATCH_X + 0x10U, GROUNG_PATCH_Y);

    // Place both parts of the Dino sprite in the play area and arrange them side by side
    move_sprite(DINO_1, dino_x, dino_y);
    move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

    // Hide the high score display at the start
    high_score_toggle = FALSE;

    // For use when holding the DOWN button to drop faster from a jump
    speed_drop_coefficient = 0x01U;

    // No score so far!
    high_score = current_score = 0x00U;

    // No hazards on screen yet
    hazards_on_screen = 0x00U;

    // Initialize this variable to a sentinel in order to use it in the update()
    clock_jump_start = 0xFFFFU; // 65,535 in decimal

    // Turn on the screen and display the loaded sprites
    // Performs a bitwise inclusive OR on LCDC_REG(0xFF40) with 0x80U
    // (Sets bit 7 on the 8-bit LCD Control Register)
    DISPLAY_ON;

    // Performs a bitwise inclusive OR on LCDC_REG(0xFF40) with 0x02U
    // (Sets bit 1 on the 8-bit LCD Control Register)
    SHOW_SPRITES;

    // Wait for V_BLANK interrupt (screen drawing refresh)
    wait_vbl_done();

    // Static screen, wait for user input (an A button press)
    waitpad(J_A);

    // Seed the random number generator
    initrand(DIV_REG);

    // The first jump has no sound but scrolls in the rest of the background
    start_jump();

    // We want to keep track of when the game started to increase difficulty with time
    time_game_started = time(NULL);

    // Go into the main game loop!
    run_game();
}
/* --- FUNCTIONS END ---*/
