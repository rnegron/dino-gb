/*

  ________ _____                       _________________
  ___  __ \___(_)_______ ______        __  ____/___  __ )
  __  / / /__  / __  __ \_  __ \       _  / __  __  __  |
  _  /_/ / _  /  _  / / // /_/ /       / /_/ /  _  /_/ /
  /_____/  /_/   /_/ /_/ \____/        \____/   /_____/

  Demo!

*/

#include <gb/gb.h>
#include <gb/font.h>
#include <gb/drawing.h>
#include <gb/hardware.h>
#include <rand.h>
#include "dino.c"
#include "background.c"
#include "symbols.c"
#include "hazards.c"

/* MACROS START */
#define DINO_TILE_COUNT 0x1C
#define DINO_SPRITE_X_SIZE 0x08
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

// Dino Sprite Indexes
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

// States
#define DINO_IS_IDLE 0
#define DINO_FOOT_RIGHT_DOWN 1
#define DINO_FOOT_LEFT_DOWN 2

// UI Symbols
#define SYMBOLS_TILE_COUNT 0x33

// Number Tiles
#define ZERO_1  0x20
#define ZERO_2  0x22
#define ONE_1   0x24
#define ONE_2   0x26
#define TWO_1   0x28
#define TWO_2   0x2A
#define THREE_1 0x2C
#define THREE_2 0x2E
#define FOUR_1  0x30
#define FOUR_2  0x32
#define FIVE_1  0x34
#define FIVE_2  0x36
#define SIX_1   0x38
#define SIX_2   0x3A
#define SEVEN_1 0x3C
#define SEVEN_2 0x3E
#define EIGHT_1 0x40
#define EIGHT_2 0x42
#define NINE_1  0x44
#define NINE_2  0x46

// Hazards
#define HAZARDS_TILE_COUNT 0x04
#define CACTUS_TILE 0x4F

// Hazard Sprite Indexes
#define CACTUS_1 0x50
#define CACTUS_2 0x52


// Number Sprite Indexes
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
#define SCORE_DIGITS 0x05

// Score Coordinates
#define SCORE_X 0x58
#define SCORE_Y 0x30

// Extra macros
#define FOOT_SWITCH_SPEED 100 // Microseconds

/* MACROS END */


/* GLOBALS START */
UBYTE DINO_X, DINO_Y;
UBYTE dino_state;  // 1 == right foot down, 2 == left foot down
UBYTE key_press;  // Holds the return value of the joypad() function
UBYTE i;  // Reusable counter variable
UBYTE digit_left, digit_right; // Left and right sprite index of a digit
UBYTE CACTUS_X, CACTUS_Y;
/* GLOBALS END */

/* PROTOTYPES START */
void setup_ui();
void start_jump();
void play_jump_noise();
void play_death_noise();
void move_dino();
void dino_jump();
void dino_duck();
void move_hazards();
void update_score(UBYTE);
UBYTE check_collisions(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void restart();
void run_game();
/* PROTOTYPES END */

/* FUNCTIONS START */
void setup_ui() {

  // Draw starting score
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

void start_jump() {
  // Runs after the player presses the A buton after the game boots
  dino_jump();

  // Start background
  SHOW_BKG;
  wait_vbl_done();

  // Start score display
  setup_ui();

  // Set dino_state as 2 so that move_dino() changes it to 1 (right foot first)
  dino_state = DINO_FOOT_LEFT_DOWN;
}

void play_jump_noise() {
  NR10_REG = 0x79;
  NR11_REG = 0x8A;
  NR12_REG = 0xA1;
  NR13_REG = 0xE0;
  NR14_REG = 0xC6;
  NR21_REG = 0x91;
  NR22_REG = 0x84;
  NR23_REG = 0xD7;
  NR24_REG = 0x86;
  NR30_REG = 0x80;
  NR31_REG = 0x00;
  NR32_REG = 0x20;
  NR33_REG = 0xD6;
  NR34_REG = 0x86;
  NR41_REG = 0x3A;
  NR42_REG = 0xA1;
  NR43_REG = 0x00;
  NR44_REG = 0xC0;
}

void play_death_noise() {
  NR10_REG = 0x70;
  NR11_REG = 0x8A;
  NR12_REG = 0x44;
  NR13_REG = 0xF4;
  NR14_REG = 0x81;
  NR21_REG = 0x81;
  NR22_REG = 0x84;
  NR23_REG = 0xD7;
  NR24_REG = 0x86;
  NR30_REG = 0x80;
  NR31_REG = 0x00;
  NR32_REG = 0x20;
  NR33_REG = 0xD6;
  NR34_REG = 0x86;
  NR41_REG = 0x3A;
  NR42_REG = 0xA1;
  NR43_REG = 0x00;
  NR44_REG = 0xC0;

  delay(100);

  NR10_REG = 0x70;
  NR11_REG = 0x8A;
  NR12_REG = 0x44;
  NR13_REG = 0xC8;
  NR14_REG = 0x80;
  NR21_REG = 0x81;
  NR22_REG = 0x84;
  NR23_REG = 0xD7;
  NR24_REG = 0x86;
  NR30_REG = 0x80;
  NR31_REG = 0x00;
  NR32_REG = 0x20;
  NR33_REG = 0xD6;
  NR34_REG = 0x86;
  NR41_REG = 0x3A;
  NR42_REG = 0xA1;
  NR43_REG = 0x00;
  NR44_REG = 0xC0;
}

void move_dino() {

  // Switches the sprites on the Dino to make
  // a running animation, alternating feet
  switch(dino_state) {

    case DINO_FOOT_LEFT_DOWN:
      set_sprite_tile(0, RIGHT_FOOT_DINO_1_TILE);
      set_sprite_tile(1, RIGHT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_RIGHT_DOWN;
      delay(FOOT_SWITCH_SPEED);
      break;

    case DINO_FOOT_RIGHT_DOWN:
      set_sprite_tile(0, LEFT_FOOT_DINO_1_TILE);
      set_sprite_tile(1, LEFT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_LEFT_DOWN;
      delay(FOOT_SWITCH_SPEED);
      break;

    default:
      break;
  }
}

void dino_jump() {

  // Change the sprite to Idle Dino for jump
  set_sprite_tile(0, IDLE_DINO_1_TILE);
  set_sprite_tile(1, IDLE_DINO_2_TILE);

  // Play the jump sound
  play_jump_noise();

  // Moving up...
  for (i = DINO_Y; i != DINO_Y - 40; --i) {
    move_sprite(0, DINO_X, i);
    move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, i);

    key_press = joypad();
    delay(8);

    // Check if currently holding the DOWN button for fast descent
    if (key_press & J_DOWN) {
      break;
    }

    // Check if currently holding the jump key after a certain height
    if (i <= (DINO_Y - 25)) {
      if (key_press & J_A) continue;
      else break;
    }
  }

  // Hold it...
  delay(40);

  // Moving down...
  for (i; i != DINO_Y; ++i) {
    move_sprite(0, DINO_X, i);
    move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, i);
    delay(8);
  }
}

void dino_duck() {
  while (key_press & J_DOWN) {
    switch(dino_state) {
      case DINO_FOOT_LEFT_DOWN:
        set_sprite_tile(1, DUCK_DINO_RIGHT);
        dino_state = DINO_FOOT_RIGHT_DOWN;
        delay(FOOT_SWITCH_SPEED);
        break;

      case DINO_FOOT_RIGHT_DOWN:
        set_sprite_tile(1, DUCK_DINO_LEFT);
        dino_state = DINO_FOOT_LEFT_DOWN;
        delay(FOOT_SWITCH_SPEED);
        break;

      default:
        break;
    }

    key_press = joypad();
  }
}

void move_hazards() {
  if (CACTUS_X == 8) CACTUS_X = GRAPHICS_WIDTH;
  move_sprite(20, CACTUS_X, DINO_Y);
  move_sprite(21, CACTUS_X + 8, DINO_Y);
  CACTUS_X -= 5;
}

void update_score(UBYTE digit_pos) {
  // This is a recursive function (!)

  // Store the current digit sprite index, which is comprised of
  // two 8x16 sprites, a left and a right half.
  digit_left  = SCORE_1_1 + ((digit_pos - 1) << 1);
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

  switch(get_sprite_tile(digit_right)) {
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

      if (digit_pos != 1) update_score(--digit_pos);
      // else BUG: Catch overflow here?
      break; // DEBUG Does this break statement "break" tail recursion...?
  }
}

UBYTE check_collisions(UBYTE x_1, UBYTE y_1, UBYTE w_1, UBYTE h_1, UBYTE x_2, UBYTE y_2, UBYTE w_2, UBYTE h_2) {
  if ((x_1 < (x_2 + w_2)) && ((x_1 + w_1) > x_2) && (y_1 < (h_2 + y_2)) && ((y_1 + h_1) > y_2)) return 1;
  else return 0;
}

void restart() {
  // Save current high score
  // Reset difficulty, etc.
}

void run_game() {

  // DEBUG
  CACTUS_X = GRAPHICS_WIDTH - 24;
  CACTUS_Y = DINO_Y;
  set_sprite_tile(20, CACTUS_1);
  set_sprite_tile(21, CACTUS_2);
  move_sprite(20, CACTUS_X, DINO_Y);
  move_sprite(21, CACTUS_X + 8, DINO_Y);

  while(1) {
    wait_vbl_done();

    // Handle Dino feet sprite swapping
    move_dino();
    if (check_collisions(DINO_X, DINO_Y, 8, 8, CACTUS_X, CACTUS_Y, 8, 8) == 1) {
      set_sprite_tile(0, HURT_DINO_1);
      set_sprite_tile(1, HURT_DINO_2);
      play_death_noise();
      waitpad(J_A);
    }

    // Handle updating score from rightmost digit
    update_score(SCORE_DIGITS);

    key_press = joypad();
    if (key_press & J_A) {
      dino_jump();
    }

    if (key_press & J_DOWN) {
      set_sprite_tile(0, DUCK_DINO_TAIL);
      set_sprite_tile(1, DUCK_DINO_LEFT);
      set_sprite_tile(2, DUCK_DINO_BODY);
      set_sprite_tile(3, DUCK_DINO_HEAD);

      move_sprite(0, DINO_X - 8, DINO_Y + 3);
      move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE - 8, DINO_Y + 3);
      move_sprite(2, DINO_X + 8, DINO_Y + 3);
      move_sprite(3, DINO_X + 16, DINO_Y + 3);

      dino_duck();

      // Return to non-ducking running sprites
      set_sprite_tile(0, RIGHT_FOOT_DINO_1_TILE);
      set_sprite_tile(1, RIGHT_FOOT_DINO_2_TILE);
      move_sprite(0, DINO_X,  DINO_Y);
      move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y);

      // Hide the extra loaded sprites
      move_sprite(2, GRAPHICS_WIDTH, 0);
      move_sprite(3, GRAPHICS_WIDTH, 0);
    }

    // Scroll objects to the left
    move_hazards();

    //Scroll the background
    // move_world();

  }
}

int main() {
  // Turn on sound
  NR52_REG = 0x80;

  // Select sound channel 1
  NR51_REG = 0x11;

  // Crank up the volume
  NR50_REG = 0x77;

  // Use two 8x8 sprites stacked on top of one another
  SPRITES_8x16;

  DINO_X = 0x14;
  DINO_Y = GRAPHICS_HEIGHT - 17; // Y position is relative to the bottom of the screen

  // Load all the Dino sprites at once
  set_sprite_data(0, DINO_TILE_COUNT, Dino);

  // Load and place the UI symbols after dino sprites
  set_sprite_data(DINO_TILE_COUNT, SYMBOLS_TILE_COUNT, Symbols);

  // Load and place the hazard sprites after the UI symbols
  set_sprite_data(DINO_TILE_COUNT + SYMBOLS_TILE_COUNT + 1, HAZARDS_TILE_COUNT, Hazards);

  // The dino sprite is comprised of 2 smaller sprites that make one 8x16 sprite
  // Load both parts of the Dino sprite into the first two tile slots
  set_sprite_tile(0, IDLE_DINO_1_TILE);
  set_sprite_tile(1, IDLE_DINO_2_TILE);

  // Place both parts of the Dino sprite in the play area and arrange them side by side
  move_sprite(0, DINO_X,  DINO_Y);
  move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y);

  // Wait for V_BLANK interrupt (screen drawing refresh)
  wait_vbl_done();

  // Writes 0x02U to LCDC_REG (0xFF40)
  SHOW_SPRITES;

  // Static screen, wait for user input (an A button press)
  waitpad(J_A);

  // The first jump has no sound but scrolls in the rest of the background
  start_jump();

  // DEBUG: Demo mode
  run_game();

}
/* FUNCTIONS END */
