/*

  ________ _____                       _________________
  ___  __ \___(_)_______ ______        __  ____/___  __ )
  __  / / /__  / __  __ \_  __ \       _  / __  __  __  |
  _  /_/ / _  /  _  / / // /_/ /       / /_/ /  _  /_/ /
  /_____/  /_/   /_/ /_/ \____/        \____/   /_____/

  In Progress!

*/

#include <time.h>
#include <gb/gb.h>
#include <gb/drawing.h>

#include "dino.c"
#include "letters.c"
#include "digits.c"
#include "hazards.c"
#include "gameover.c"
#include "background_tiles.c"
#include "background_map.c"

/* MACROS START */
// Dino VRAM tile indices
#define IDLE_DINO_1_TILE 0x00
#define IDLE_DINO_2_TILE 0x02
#define HURT_DINO_1_TILE 0x04
#define HURT_DINO_2_TILE 0x06
#define RIGHT_FOOT_DINO_1_TILE 0x08
#define RIGHT_FOOT_DINO_2_TILE 0x0A
#define LEFT_FOOT_DINO_1_TILE 0x0C
#define LEFT_FOOT_DINO_2_TILE 0X0E
#define DUCK_DINO_1_TILE 0x10
#define DUCK_DINO_2_TILE 0x12
#define DUCK_DINO_3_TILE 0x14

// Dino Tile Indices
#define IDLE_DINO_1 0x00
#define IDLE_DINO_2 0x01
#define HURT_DINO_1 0x04
#define HURT_DINO_2 0x06
#define RIGHT_STEP_DINO_1 0x08
#define RIGHT_STEP_DINO_2 0x0A
#define LEFT_STEP_DINO_1 0x0C
#define LEFT_STEP_DINO_2 0x0E
#define DUCK_DINO_BODY 0x10
#define DUCK_DINO_HEAD 0x12
#define DUCK_DINO_TAIL 0x14
#define DUCK_DINO_LEFT 0x16
#define DUCK_DINO_RIGHT 0x1A

// Dino Sprite Indices
#define DINO_1 0x00
#define DINO_2 0x01
#define DINO_3 0x03
#define DINO_4 0x04

// Digit Tiles
#define ZERO_1 0x1C
#define ZERO_2 0x1E
#define ONE_1 0x20
#define ONE_2 0x22
#define TWO_1 0x24
#define TWO_2 0x26
#define THREE_1 0x28
#define THREE_2 0x2A
#define FOUR_1 0x2C
#define FOUR_2 0x2E
#define FIVE_1 0x30
#define FIVE_2 0x32
#define SIX_1 0x34
#define SIX_2 0x36
#define SEVEN_1 0x38
#define SEVEN_2 0x3A
#define EIGHT_1 0x3C
#define EIGHT_2 0x3E
#define NINE_1 0x40
#define NINE_2 0x42

// Digit Sprite Indices
#define SCORE_1_1 0x0A
#define SCORE_1_2 0x0B
#define SCORE_2_1 0x0C
#define SCORE_2_2 0x0D
#define SCORE_3_1 0x0E
#define SCORE_3_2 0x0F
#define SCORE_4_1 0x10
#define SCORE_4_2 0x11
#define SCORE_5_1 0x12
#define SCORE_5_2 0x13

#define HI_SCORE_1_1 0x14
#define HI_SCORE_1_2 0x15
#define HI_SCORE_2_1 0x16
#define HI_SCORE_2_2 0x17
#define HI_SCORE_3_1 0x18
#define HI_SCORE_3_2 0x19
#define HI_SCORE_4_1 0x1A
#define HI_SCORE_4_2 0x1B
#define HI_SCORE_5_1 0x1C
#define HI_SCORE_5_2 0x1D


#define H_1 0x05
#define H_2 0x06
#define I_1 0x07
#define I_2 0x08
#define H_1_TILE 0x68
#define H_2_TILE 0x6A
#define I_1_TILE 0x6C
#define I_2_TILE 0x6E

#define HI_SCORE_X 0x54
#define HI_SCORE_Y 0x28

// Hazards
#define CACTUS_TILE 0x4F

// Hazard Sprite Tiles
#define CACTUS_1_TILE 0x44
#define CACTUS_2_TILE 0x46

// Hazard Sprite Indices
#define CACTUS_1 0x1E
#define CACTUS_2 0x1F

// Extra macros
#define DINO_SPRITE_X_SIZE 0x08
#define FOOT_SWITCH_SPEED 0x64 // 100 in base 10 (Microseconds)
#define DINO_FOOT_RIGHT_DOWN 0x01
#define DINO_FOOT_LEFT_DOWN 0x02
#define DINO_JUMPING 0x03
#define DINO_FALLING 0x04

#define DINO_TILE_COUNT 0x1C
#define HAZARDS_TILE_COUNT 0x04
#define LETTERS_TILE_COUNT 0x2C
#define DIGITS_TILE_COUNT 0x28
#define BACKGROUND_TILE_COUNT 0x20

#define DEFAULT_DINO_X 0x14
#define DEFAULT_DINO_Y GRAPHICS_HEIGHT - 17
#define SCORE_X 0x54
#define SCORE_Y 0x16
#define GAME_OVER_X 0x28
#define GAME_OVER_Y 0x32

// Used for the intro, where only a small patch of ground is visible
#define GROUND_PATCH_1 0x07
#define GROUND_PATCH_2 0x08
#define GROUND_PATCH_3 0x09
#define GROUND_PATCH_1_TILE 0x4B
#define GROUND_PATCH_2_TILE 0x4D
#define GROUND_PATCH_3_TILE 0x51
#define GROUND_PATCH_X 0x13
#define GROUNG_PATCH_Y GRAPHICS_HEIGHT - 16

#define SCORE_DIGITS 0x05 // i.e. 5 digits: 00000
#define BLANK_TILE 0x48   // Useful "blank" or invisible tile
/* MACROS END */

/* GLOBALS START */
UBYTE dino_x, dino_y;          // Store Dino Sprite coordinates
UBYTE bkg_x;                   // Background X coordinate for scrolling
UBYTE dino_state;              // 1 == right foot down, 2 == left foot down, 3 == jumping, 4 == falling
UBYTE key_press;               // Holds the return value of the joypad() function
UBYTE i;                       // Reusable counter variable
UBYTE digit_left, digit_right; // Left and right sprite index of a digit
UBYTE tmp_digit;               // Temporary digit used for holding a digit index while calculating score
UBYTE CACTUS_X, CACTUS_Y;      // Store nearest Cactus hazard coordinates
UBYTE current_score;           // Store an integer representation of the current score
UBYTE high_score;              // Store an integer representation of the high score
BOOLEAN high_score_toggle;

UWORD jump_start_time; //
UWORD time_delta;      //

// Variables in which to save the score sprite indexes
UBYTE SCORE_1_1_TILE, SCORE_1_2_TILE, SCORE_2_1_TILE, SCORE_2_2_TILE,
    SCORE_3_1_TILE, SCORE_3_2_TILE, SCORE_4_1_TILE, SCORE_4_2_TILE,
    SCORE_5_1_TILE, SCORE_5_2_TILE;
/* GLOBALS END */

/* PROTOTYPES START */
void setup_score_sprites();
void setup_hi_score_sprites();
UBYTE get_multiplier(UBYTE);
UBYTE score_sprite_to_num(UBYTE);
UBYTE num_to_left_score_sprite(UBYTE);
UBYTE num_to_right_score_sprite(UBYTE);
void set_high_score();
void start_jump();
void play_jump_sound();
void play_death_sound();
void play_score_sound();
// void flash_score();
void move_hazards();
void move_bkg();
void update_score(UBYTE);
BOOLEAN check_collisions(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void game_over();
void restart();
BOOLEAN new_high_score();
void run_game();
/* PROTOTYPES END */

/* FUNCTIONS START */
void setup_score_sprites()
{
  /* Using sprites to keep a visual score. Changing sprites simulates increasing the score. */

  // Load the both parts of the ZERO sprite (two parts because it is 8x16 pixels in total)
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
  move_sprite(SCORE_1_2, SCORE_X + 8, SCORE_Y);
  move_sprite(SCORE_2_1, SCORE_X + 16, SCORE_Y);
  move_sprite(SCORE_2_2, SCORE_X + 24, SCORE_Y);
  move_sprite(SCORE_3_1, SCORE_X + 32, SCORE_Y);
  move_sprite(SCORE_3_2, SCORE_X + 40, SCORE_Y);
  move_sprite(SCORE_4_1, SCORE_X + 48, SCORE_Y);
  move_sprite(SCORE_4_2, SCORE_X + 56, SCORE_Y);
  move_sprite(SCORE_5_1, SCORE_X + 64, SCORE_Y);
  move_sprite(SCORE_5_2, SCORE_X + 72, SCORE_Y);
}

void setup_hi_score_sprites()
{
  /* Using sprites to keep a visual score. Changing sprites simulates increasing the score. */

  // Load the both parts of the ZERO sprite (two parts because it is 8x16 pixels in total)
  // into all five corresponding digit positions
  set_sprite_tile(HI_SCORE_1_1, num_to_left_score_sprite((high_score / 10000) % 10));
  set_sprite_tile(HI_SCORE_1_2, num_to_right_score_sprite((high_score / 10000) % 10));
  set_sprite_tile(HI_SCORE_2_1, num_to_left_score_sprite((high_score / 1000) % 10));
  set_sprite_tile(HI_SCORE_2_2, num_to_right_score_sprite((high_score / 1000) % 10));
  set_sprite_tile(HI_SCORE_3_1, num_to_left_score_sprite((high_score / 100) % 10));
  set_sprite_tile(HI_SCORE_3_2, num_to_right_score_sprite((high_score / 100) % 10));
  set_sprite_tile(HI_SCORE_4_1, num_to_left_score_sprite((high_score / 10) % 10));
  set_sprite_tile(HI_SCORE_4_2, num_to_right_score_sprite((high_score / 10) % 10));
  set_sprite_tile(HI_SCORE_5_1, num_to_left_score_sprite(high_score % 10));
  set_sprite_tile(HI_SCORE_5_2, num_to_right_score_sprite(high_score % 10));

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
  move_sprite(HI_SCORE_1_2, HI_SCORE_X + 8, HI_SCORE_Y);
  move_sprite(HI_SCORE_2_1, HI_SCORE_X + 16, HI_SCORE_Y);
  move_sprite(HI_SCORE_2_2, HI_SCORE_X + 24, HI_SCORE_Y);
  move_sprite(HI_SCORE_3_1, HI_SCORE_X + 32, HI_SCORE_Y);
  move_sprite(HI_SCORE_3_2, HI_SCORE_X + 40, HI_SCORE_Y);
  move_sprite(HI_SCORE_4_1, HI_SCORE_X + 48, HI_SCORE_Y);
  move_sprite(HI_SCORE_4_2, HI_SCORE_X + 56, HI_SCORE_Y);
  move_sprite(HI_SCORE_5_1, HI_SCORE_X + 64, HI_SCORE_Y);
  move_sprite(HI_SCORE_5_2, HI_SCORE_X + 72, HI_SCORE_Y);
}

UBYTE get_multiplier(UBYTE digit_pos)
{
  switch (digit_pos)
  {
  case 5:
    return 1;

  case 4:
    return 10;

  case 3:
    return 100;

  case 2:
    return 1000;

  case 1:
    return 10000;
  }
}

UBYTE score_sprite_to_num(UBYTE digit_pos)
{

  // This tail-recursive function returns a UBYTE representation of the current score
  // Reusing i as an accumulator here, which was reset before the call to this function

  if (digit_pos == 0)
    return i;

  // For an explanation of the formula used here, check out the update_score() function
  digit_right = SCORE_1_2 + ((digit_pos - 1) << 1);

  tmp_digit = get_sprite_tile(digit_right);

  if (tmp_digit == ZERO_2)
    return score_sprite_to_num(--digit_pos);

  else if (tmp_digit == ONE_2)
  {
    i += (get_multiplier(digit_pos) * 1);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == TWO_2)
  {
    i += (get_multiplier(digit_pos) * 2);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == THREE_2)
  {
    i += (get_multiplier(digit_pos) * 3);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == FOUR_2)
  {
    i += (get_multiplier(digit_pos) * 4);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == FIVE_2)
  {
    i += (get_multiplier(digit_pos) * 5);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == SIX_2)
  {
    i += (get_multiplier(digit_pos) * 6);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == SEVEN_2)
  {
    i += (get_multiplier(digit_pos) * 7);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == EIGHT_2)
  {
    i += (get_multiplier(digit_pos) * 8);
    return score_sprite_to_num(--digit_pos);
  }

  else if (tmp_digit == NINE_2)
  {
    i += (get_multiplier(digit_pos) * 9);
    return score_sprite_to_num(--digit_pos);
  }
}

UBYTE num_to_left_score_sprite(UBYTE digit)
{
  // This is the left-side-sprite inverse function to score_sprite_to_num()

  switch (digit)
  {
  case 0:
    return ZERO_1;
  case 1:
    return ONE_1;
  case 2:
    return TWO_1;
  case 3:
    return THREE_1;
  case 4:
    return FOUR_1;
  case 5:
    return FIVE_1;
  case 6:
    return SIX_1;
  case 7:
    return SEVEN_1;
  case 8:
    return EIGHT_1;
  case 9:
    return NINE_1;
  }
}

UBYTE num_to_right_score_sprite(UBYTE digit)
{
  // This is the right-side sprite-inverse function to score_sprite_to_num()

  switch (digit)
  {
  case 0:
    return ZERO_2;
  case 1:
    return ONE_2;
  case 2:
    return TWO_2;
  case 3:
    return THREE_2;
  case 4:
    return FOUR_2;
  case 5:
    return FIVE_2;
  case 6:
    return SIX_2;
  case 7:
    return SEVEN_2;
  case 8:
    return EIGHT_2;
  case 9:
    return NINE_2;
  }
}

void set_high_score()
{
  high_score = current_score;

  if (!high_score_toggle)
    high_score_toggle = 1;
}

void start_jump()
{
  // Runs after the player presses the A buton after the game boots
  for (i = DEFAULT_DINO_Y; i != DEFAULT_DINO_Y - 40; --i)
  {
    move_sprite(DINO_1, dino_x, i);
    move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, i);
    delay(8);
  }

  delay(60);

  for (i; i != DEFAULT_DINO_Y; ++i)
  {
    move_sprite(DINO_1, dino_x, i);
    move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, i);
    delay(8);
  }

  // Move the ground sprites out of display for this session of the game
  move_sprite(GROUND_PATCH_1, 0x00, 0x00);
  move_sprite(GROUND_PATCH_2, 0x00, 0x00);
  move_sprite(GROUND_PATCH_3, 0x00, 0x00);

  // Start score display
  setup_score_sprites();

  // Set dino_state as 2 so that move_dino() changes it to 1 (right foot first)
  dino_state = DINO_FOOT_LEFT_DOWN;
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
  NR10_REG = 0x00;
  NR11_REG = 0x80;
  NR12_REG = 0x63;
  NR13_REG = 0x03;
  NR14_REG = 0x87;

  delay(100);

  NR10_REG = 0x00;
  NR11_REG = 0x80;
  NR12_REG = 0x63;
  NR13_REG = 0x53;
  NR14_REG = 0x87;
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

void move_hazards()
{
  if (CACTUS_X == 8)
    CACTUS_X = GRAPHICS_WIDTH;
  move_sprite(CACTUS_1, CACTUS_X, CACTUS_Y);
  move_sprite(CACTUS_2, CACTUS_X + 8, CACTUS_Y);
  CACTUS_X -= 8;
}

void move_bkg()
{
  if (bkg_x == GRAPHICS_WIDTH)
    bkg_x = 0;

  ++bkg_x;
  scroll_bkg(bkg_x, 0);
}

void update_score(UBYTE digit_pos)
{
  // Note: This is a recursive function

  // Store the current digit sprite index, which is comprised of
  // two 8x16 sprites, a left and a right half.
  digit_left = SCORE_1_1 + ((digit_pos - 1) << 1);
  digit_right = SCORE_1_2 + ((digit_pos - 1) << 1);

  /*
    Example of the equation for obtaining the index of the left
    side of the 4th digit:
    digit_left = SCORE_1_1   [macro defined as 0x0A]
      + ((digit_pos - 1)     [digit_pos was 4, so we have 3]
      << 1)                  [bit shift left by 1 means multiply by 2, so we have 6]
      ------------------------------------------------
      = 0x0A + 0x06 = 0x10 = SCORE_4_1
  */

  switch (get_sprite_tile(digit_right))
  {
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
    if (digit_pos != 1)
    {

      // Perform a recursion on the next digit (to the left)
      update_score(--digit_pos);

      // If this digit is a multiple of 100, play a sound and flash the screen
      if (digit_pos == 3)
      {
        play_score_sound();
        // flash_score();
      }
    }

    // else BUG: Catch overflow here?
    break; // DEBUG Does this break statement "break" tail recursion...?
  }
}

BOOLEAN check_collisions(UBYTE x_1, UBYTE y_1, UBYTE w_1, UBYTE h_1,
                         UBYTE x_2, UBYTE y_2, UBYTE w_2, UBYTE h_2)
{

  // Standard rectangle-to-rectangle collision check
  if ((x_1 < (x_2 + w_2)) && ((x_1 + w_1) > x_2) &&
      (y_1 < (h_2 + y_2)) && ((y_1 + h_1) > y_2))
    return 1;
  else
    return 0;
}

void game_over()
{
  // Store the score at game over
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
  SHOW_WIN;

  play_death_sound();

  // Check to see if there is a new high score
  if (new_high_score())
  {
    // If this is the first high score, move the high score sprites into position
    if (!high_score_toggle)
    {
      set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT + HAZARDS_TILE_COUNT, LETTERS_TILE_COUNT, Letters);
      // set_sprite_tile(H_1, H_1_TILE);
      // set_sprite_tile(H_2, H_2_TILE);
      // set_sprite_tile(I_1, I_1_TILE);
      // set_sprite_tile(I_2, I_2_TILE);

      // move_sprite(H_1, HI_SCORE_X, HI_SCORE_Y + 0x18);
      // move_sprite(H_2, HI_SCORE_X + 0x08, HI_SCORE_Y + 0x18);
      // move_sprite(I_1, HI_SCORE_X + 0x10, HI_SCORE_Y + 0x18);
      // move_sprite(I_2, HI_SCORE_X + 0x18, HI_SCORE_Y + 0x18);
    }
    set_high_score();
    setup_hi_score_sprites();
  }

  // Wait for user to acknowledge game over
  waitpad(J_A);

  // Start the game again!
  restart();
}

void restart()
{
  /* 
    TODO: Save current high score, reset difficulty, clear sprite tiles, etc.
  */

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
  bkg_x = 0;

  // Move the Dino back into starting position with the starting idle sprite
  set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
  set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

  move_sprite(DINO_1, dino_x, dino_y);
  move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

  SHOW_SPRITES;

  // Start the game loop
  run_game();
}

void run_game()
{

  // Display the background
  SHOW_BKG;

  // DEBUG
  CACTUS_X = GRAPHICS_WIDTH - 24;
  CACTUS_Y = DEFAULT_DINO_Y - 2;

  set_sprite_tile(CACTUS_1, CACTUS_1_TILE);
  set_sprite_tile(CACTUS_2, CACTUS_2_TILE);

  move_sprite(CACTUS_1, CACTUS_X, CACTUS_Y);
  move_sprite(CACTUS_2, CACTUS_X + 8, CACTUS_Y);

  // Main game loop
  while (1)
  {
    // Wait for V_BLANK interrupt (screen drawing refresh)
    wait_vbl_done();

    // Perform particular logic based on the state of the Dino
    switch (dino_state)
    {
    case DINO_FALLING:

      // If the Y position of the Dino is the starting position, the Dino finished falling
      if (dino_y == DEFAULT_DINO_Y)
      {
        dino_state = DINO_FOOT_LEFT_DOWN;
        break;
      }

      // The dino can still keep falling...
      else
      {
        // Get the current key being pressed
        key_press = joypad();

        if ((key_press & J_DOWN) && (dino_y + 3 >= DEFAULT_DINO_Y + 3))
        {
          // Fall quickly
          dino_y += 3;
          move_sprite(DINO_1, dino_x, dino_y);
          move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);
          break;
        }

        else
        {
          // Fall normally
          ++dino_y;
          move_sprite(DINO_1, dino_x, dino_y);
          move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);
          break;
        }
      }

    case DINO_JUMPING:

      // Check if currently holding the DOWN button for fast descent
      key_press = joypad();
      if (key_press & J_DOWN)
      {
        dino_state = DINO_FALLING;
        break;
      }

      // Check if currently holding the jump key after a certain height
      // This is to allow variation in jump height, but still force a minimum jump height
      if (dino_y <= (DEFAULT_DINO_Y - 25))
      {
        if (key_press & J_A)
          continue;
        else
        {
          dino_state = DINO_FALLING;
          break;
        }
      }

      // If the Dino has reached maximum jump height, time to come back down
      if (dino_y == (DEFAULT_DINO_Y - 40))
      {
        dino_state = DINO_FALLING;
        break;
      }

      // The Dino can still jump higher...
      else
      {
        --dino_y;
        move_sprite(DINO_1, dino_x, dino_y);
        move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);
        break;
      }

    // Handle Dino feet sprite swapping
    case DINO_FOOT_LEFT_DOWN:
      set_sprite_tile(DINO_1, RIGHT_FOOT_DINO_1_TILE);
      set_sprite_tile(DINO_2, RIGHT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_RIGHT_DOWN;
      delay(FOOT_SWITCH_SPEED);
      break;

    case DINO_FOOT_RIGHT_DOWN:
      set_sprite_tile(DINO_1, LEFT_FOOT_DINO_1_TILE);
      set_sprite_tile(DINO_2, LEFT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_LEFT_DOWN;
      delay(FOOT_SWITCH_SPEED);
      break;

    default:
      break;
    }

    // Handle Dino collision with hazards
    if (check_collisions(dino_x, dino_y, 12, 12, CACTUS_X, CACTUS_Y, 8, 8) == 1)
    {
      game_over();
    }

    // Handle updating score from rightmost digit
    update_score(SCORE_DIGITS);

    key_press = joypad();

    // Handle dino jumping
    if (key_press & J_A)
    {

      dino_state = DINO_JUMPING;
      jump_start_time = time(NULL);

      // Change the sprite to Idle Dino for jump
      set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
      set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

      // Play the jump sound
      play_jump_sound();
    }

    // if (key_press & J_DOWN) {
    //   // Set ducking Dino running sprites
    //   set_sprite_tile(DINO_1, DUCK_DINO_TAIL);
    //   set_sprite_tile(DINO_2, DUCK_DINO_LEFT);
    //   set_sprite_tile(DINO_3, DUCK_DINO_BODY);
    //   set_sprite_tile(DINO_4, DUCK_DINO_HEAD);

    //   // Move the ducking Dino sprites into place
    //   move_sprite(DINO_1, dino_x - DINO_SPRITE_X_SIZE, dino_y + 3);
    //   move_sprite(DINO_2, dino_x, dino_y + 3);
    //   move_sprite(DINO_3, dino_x + DINO_SPRITE_X_SIZE, dino_y + 3);
    //   move_sprite(DINO_4, dino_x + DINO_SPRITE_X_SIZE + DINO_SPRITE_X_SIZE, dino_y + 3);

    //   dino_duck();

    //   // Return to non-ducking Dino running sprites
    //   set_sprite_tile(DINO_1, RIGHT_FOOT_DINO_1_TILE);
    //   set_sprite_tile(DINO_2, RIGHT_FOOT_DINO_2_TILE);

    //   // Move the non-ducking Dino sprites into place
    //   move_sprite(DINO_1, dino_x, dino_y);
    //   move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

    //   // Hide the extra loaded sprites off screen
    //   move_sprite(DINO_3, GRAPHICS_WIDTH + DINO_SPRITE_X_SIZE, GRAPHICS_HEIGHT);
    //   move_sprite(DINO_4, GRAPHICS_WIDTH + DINO_SPRITE_X_SIZE, GRAPHICS_HEIGHT);
    // }

    // Scroll hazards to the left
    move_hazards();

    // Scroll the background
    move_bkg();
  }
}

BOOLEAN new_high_score()
{
  i = 0;
  current_score = score_sprite_to_num(5);
  return current_score > high_score ? 1 : 0;
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

  dino_x = DEFAULT_DINO_X; // Set dino_x position as 20
  dino_y = DEFAULT_DINO_Y; // Y position is relative to the bottom of the screen

  // Load all the Dino sprites at once
  set_sprite_data(0, DINO_TILE_COUNT, Dino);

  // Load and place digit sprites after dino sprites
  set_sprite_data(DINO_TILE_COUNT, DIGITS_TILE_COUNT, Digits);

  // Load and place the hazard sprites after the UI symbols
  set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT, HAZARDS_TILE_COUNT, Hazards);

  // Load some background tiles as sprites in order to place some of them at the start
  set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT + HAZARDS_TILE_COUNT, 0x0B, BackgroundTiles);

  // The dino sprite is comprised of 2 smaller sprites that make one 8x16 sprite
  // Load both parts of the Dino sprite into the first two tile slots
  set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
  set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

  //
  set_sprite_tile(GROUND_PATCH_1, GROUND_PATCH_1_TILE);
  set_sprite_tile(GROUND_PATCH_2, GROUND_PATCH_2_TILE);
  set_sprite_tile(GROUND_PATCH_3, GROUND_PATCH_3_TILE);

  move_sprite(GROUND_PATCH_1, GROUND_PATCH_X, GROUNG_PATCH_Y);
  move_sprite(GROUND_PATCH_2, GROUND_PATCH_X + 8, GROUNG_PATCH_Y);
  move_sprite(GROUND_PATCH_3, GROUND_PATCH_X + 16, GROUNG_PATCH_Y);

  // Place both parts of the Dino sprite in the play area and arrange them side by side
  move_sprite(DINO_1, dino_x, dino_y);
  move_sprite(DINO_2, dino_x + DINO_SPRITE_X_SIZE, dino_y);

  // Load the tiles used to write GAME OVER on a window
  set_win_data(0, LETTERS_TILE_COUNT, Letters);
  set_win_tiles(0, 0, GameoverWidth, GameoverHeight, Gameover);

  //
  bkg_x = 0;
  set_bkg_data(LETTERS_TILE_COUNT, 32, BackgroundTiles);
  set_bkg_tiles(bkg_x, 0, BackgroundMapWidth, BackgroundMapHeight, BackgroundMap);

  // Hide the high score display at the start
  high_score_toggle = 0;

  // No high score so far!
  high_score = 0;

  // Performs a bitwise inclusive OR on LCDC_REG(0xFF40) with 0x80U
  // (Turns on bit 7 of the 8-bit LCD Control Register)
  DISPLAY_ON;

  // Writes 0x02U to LCDC_REG (0xFF40)
  SHOW_SPRITES;

  // Wait for V_BLANK interrupt (screen drawing refresh)
  wait_vbl_done();

  // Static screen, wait for user input (an A button press)
  waitpad(J_A);

  // The first jump has no sound but scrolls in the rest of the background
  start_jump();

  // DEBUG: Demo mode
  run_game();
}
/* FUNCTIONS END */
