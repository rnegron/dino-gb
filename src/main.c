/*

  ________ _____                       _________________
  ___  __ \___(_)_______ ______        __  ____/___  __ )
  __  / / /__  / __  __ \_  __ \       _  / __  __  __  |
  _  /_/ / _  /  _  / / // /_/ /       / /_/ /  _  /_/ /
  /_____/  /_/   /_/ /_/ \____/        \____/   /_____/

  Demo!

*/

#include <gb/gb.h>
#include <gb/drawing.h>

#include "dino.c"
#include "letters.c"
#include "digits.c"
#include "hazards.c"
#include "gameover.c"

/* MACROS START */
// Dino VRAM tile indices
#define IDLE_DINO_1_TILE       0x00
#define IDLE_DINO_2_TILE       0x02
#define HURT_DINO_1_TILE       0x04
#define HURT_DINO_2_TILE       0x06
#define RIGHT_FOOT_DINO_1_TILE 0x08
#define RIGHT_FOOT_DINO_2_TILE 0x0A
#define LEFT_FOOT_DINO_1_TILE  0x0C
#define LEFT_FOOT_DINO_2_TILE  0X0E
#define DUCK_DINO_1_TILE       0x10
#define DUCK_DINO_2_TILE       0x12
#define DUCK_DINO_3_TILE       0x14

// Dino Tile Indices
#define IDLE_DINO_1       0x00
#define IDLE_DINO_2       0x01
#define HURT_DINO_1       0x04
#define HURT_DINO_2       0x06
#define RIGHT_STEP_DINO_1 0x08
#define RIGHT_STEP_DINO_2 0x0A
#define LEFT_STEP_DINO_1  0x0C
#define LEFT_STEP_DINO_2  0x0E
#define DUCK_DINO_BODY    0x10
#define DUCK_DINO_HEAD    0x12
#define DUCK_DINO_TAIL    0x14
#define DUCK_DINO_LEFT    0x16
#define DUCK_DINO_RIGHT   0x1A

// Dino Sprite Indices
#define DINO_1 0x00
#define DINO_2 0x01
#define DINO_3 0x03
#define DINO_4 0x04

// Digit Tiles
#define ZERO_1  0x1C
#define ZERO_2  0x1E
#define ONE_1   0x20
#define ONE_2   0x22
#define TWO_1   0x24
#define TWO_2   0x26
#define THREE_1 0x28
#define THREE_2 0x2A
#define FOUR_1  0x2C
#define FOUR_2  0x2E
#define FIVE_1  0x30
#define FIVE_2  0x32
#define SIX_1   0x34
#define SIX_2   0x36
#define SEVEN_1 0x38
#define SEVEN_2 0x3A
#define EIGHT_1 0x3C
#define EIGHT_2 0x3E
#define NINE_1  0x40
#define NINE_2  0x42

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

// Hazards
#define CACTUS_TILE 0x4F

// Hazard Sprite Tiles
#define CACTUS_1_TILE 0x44
#define CACTUS_2_TILE 0x46

// Hazard Sprite Indices
#define CACTUS_1 0x1E
#define CACTUS_2 0x1F

// Extra macros
#define DINO_TILE_COUNT      0x1C
#define DINO_SPRITE_X_SIZE   0x08
#define FOOT_SWITCH_SPEED    0x64  // 100 in base 10 (Microseconds)
#define DINO_IS_IDLE         0x00
#define DINO_FOOT_RIGHT_DOWN 0x01
#define DINO_FOOT_LEFT_DOWN  0x02

#define LETTERS_TILE_COUNT   0x2C
#define DIGITS_TILE_COUNT    0x28
#define SCORE_DIGITS         0x05 // i.e. 5 digits: 00000
#define SCORE_X              0x54
#define SCORE_Y              0x16
#define GAME_OVER_X          0x28
#define GAME_OVER_Y          0x28

#define HAZARDS_TILE_COUNT   0x04
#define DINO_JUMP_SPEED      0x08

#define BLANK_TILE           0x48

/* MACROS END */


/* GLOBALS START */
UBYTE DINO_X, DINO_Y;           // Store Dino Sprite coordinates
UBYTE dino_state;               // 1 == right foot down, 2 == left foot down
UBYTE key_press;                // Holds the return value of the joypad() function
UBYTE i;                        // Reusable counter variable
UBYTE digit_left, digit_right;  // Left and right sprite index of a digit
UBYTE CACTUS_X, CACTUS_Y;       // Store nearest Cactus hazard coordinates
UBYTE high_score;               // Store an integer representation of the high score

// Variables in which to save the score sprite indexes
UBYTE SCORE_1_1_TILE, SCORE_1_2_TILE, SCORE_2_1_TILE, SCORE_2_2_TILE, 
      SCORE_3_1_TILE, SCORE_3_2_TILE, SCORE_4_1_TILE, SCORE_4_2_TILE, 
      SCORE_5_1_TILE, SCORE_5_2_TILE;
/* GLOBALS END */

/* PROTOTYPES START */
void setup_score();
// UBYTE score_sprite_to_num();
// void set_high_score();
void start_jump();
void play_jump_sound();
void play_death_sound();
void play_score_sound();
void flash_score();
void move_dino();
void dino_jump();
void dino_duck();
void move_hazards();
void update_score(UBYTE);
BOOLEAN check_collisions(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void game_over();
void restart();
BOOLEAN new_high_score();
void run_game();
/* PROTOTYPES END */

/* FUNCTIONS START */
void setup_score() {
  /* Using sprites to keep a visual score. Changing sprites simulates increasing the score. */

  // Load the both parts of the ZERO sprite (which is 8x16 pixels)
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

// UBYTE score_sprite_to_num() {

// }

// void set_high_score(){

// }

void start_jump() {
  // Runs after the player presses the A buton after the game boots
  dino_jump();

  wait_vbl_done();

  // Start score display
  setup_score();

  // Set dino_state as 2 so that move_dino() changes it to 1 (right foot first)
  dino_state = DINO_FOOT_LEFT_DOWN;
}

void play_jump_sound() {
  NR10_REG = 0x79;
  NR11_REG = 0x8A;
  NR12_REG = 0xA1;
  NR13_REG = 0xE0;
  NR14_REG = 0xC6;
}

void play_death_sound() {
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

void play_score_sound() {
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

void flash_score() {
  // Save the current score sprites
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

  for (i = 0; i < 3; i++) {
    set_sprite_tile(SCORE_1_1, BLANK_TILE);
    set_sprite_tile(SCORE_1_2, BLANK_TILE);
    set_sprite_tile(SCORE_2_1, BLANK_TILE);
    set_sprite_tile(SCORE_2_2, BLANK_TILE);
    set_sprite_tile(SCORE_3_1, BLANK_TILE);
    set_sprite_tile(SCORE_3_2, BLANK_TILE);
    set_sprite_tile(SCORE_4_1, BLANK_TILE);
    set_sprite_tile(SCORE_4_2, BLANK_TILE);
    set_sprite_tile(SCORE_5_1, BLANK_TILE);
    set_sprite_tile(SCORE_5_2, BLANK_TILE);

    delay(500);

    set_sprite_tile(SCORE_1_1, SCORE_1_1_TILE);
    set_sprite_tile(SCORE_1_2, SCORE_1_2_TILE);
    set_sprite_tile(SCORE_2_1, SCORE_2_1_TILE);
    set_sprite_tile(SCORE_2_2, SCORE_2_2_TILE);
    set_sprite_tile(SCORE_3_1, SCORE_3_1_TILE);
    set_sprite_tile(SCORE_3_2, SCORE_3_2_TILE);
    set_sprite_tile(SCORE_4_1, SCORE_4_1_TILE);
    set_sprite_tile(SCORE_4_2, SCORE_4_2_TILE);
    set_sprite_tile(SCORE_5_1, SCORE_5_1_TILE);
    set_sprite_tile(SCORE_5_2, SCORE_5_2_TILE);

    delay(500);
  }

  // Simulates time passing and score increasing 
  // while the score flashes
  set_sprite_tile(SCORE_4_1, TWO_1);
  set_sprite_tile(SCORE_4_2, TWO_2);
  set_sprite_tile(SCORE_5_1, FIVE_1);
  set_sprite_tile(SCORE_5_2, FIVE_2);
}

void move_dino() {

  // Switches the sprites on the Dino to make
  // a running animation, alternating feet
  switch(dino_state) {
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
  }
}

void dino_jump() {

  // Change the sprite to Idle Dino for jump
  set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
  set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

  // Play the jump sound
  play_jump_sound();

  // Moving up...
  for (i = DINO_Y; i != DINO_Y - 40; --i) {
    move_sprite(0, DINO_X, i);
    move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, i);

    key_press = joypad();
    delay(DINO_JUMP_SPEED);

    // Check if currently holding the DOWN button for fast descent
    if (key_press & J_DOWN) {
      break;
    }

    // Check if currently holding the jump key after a certain height
    // This is to allow variation in jump height, but still force a minimum jump height
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
    delay(DINO_JUMP_SPEED);
  }
}

void dino_duck() {
  while (key_press & J_DOWN) {
    switch(dino_state) {
      case DINO_FOOT_LEFT_DOWN:
        set_sprite_tile(DINO_2, DUCK_DINO_RIGHT);
        dino_state = DINO_FOOT_RIGHT_DOWN;
        delay(FOOT_SWITCH_SPEED);
        break;

      case DINO_FOOT_RIGHT_DOWN:
        set_sprite_tile(DINO_2, DUCK_DINO_LEFT);
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
  move_sprite(CACTUS_1, CACTUS_X, CACTUS_Y);
  move_sprite(CACTUS_2, CACTUS_X + 8, CACTUS_Y);
  CACTUS_X -= 8;
}

void update_score(UBYTE digit_pos) {
  // Note: This is a recursive function

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

      // If this position is a carry position, move on to the next digit
      if (digit_pos != 1) {

        // Perform a recursion on the next digit (to the left)
        update_score(--digit_pos);

        // If this digit is a multiple of 100, play a sound and flash the screen
        if (digit_pos == 3) {
          play_score_sound();
          flash_score();
        }
      }

      // else BUG: Catch overflow here?
      break; // DEBUG Does this break statement "break" tail recursion...?
  }
}

BOOLEAN check_collisions(UBYTE x_1, UBYTE y_1, UBYTE w_1, UBYTE h_1,
                         UBYTE x_2, UBYTE y_2, UBYTE w_2, UBYTE h_2) {

  // Standard rectangle-to-rectangle collision check
  if ((x_1 < (x_2 + w_2)) && ((x_1 + w_1) > x_2) &&
      (y_1 < (h_2 + y_2)) && ((y_1 + h_1) > y_2)) return 1;
  else return 0;
}

void game_over() {
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

  play_death_sound();

  wait_vbl_done();

  // Check to see if there is a new high score
  if (new_high_score)

  // Move the game over window to position and display it
  move_win(GAME_OVER_X, GAME_OVER_Y);
  SHOW_WIN;
  SHOW_BKG;

  // Wait for user to acknowledge game over
  waitpad(J_A);

  // Start the game again!
  restart();
}


void restart() {
  /* 
    TODO: Save current high score, reset difficulty, clear sprite tiles, etc.
  */

  // Hide all graphical objects while we reset them
  HIDE_SPRITES;
  HIDE_WIN;
  HIDE_BKG;

  // Flush the score sprites
  setup_score();

  // Reset the dino's starting position
  dino_state = DINO_FOOT_LEFT_DOWN;

  SHOW_SPRITES;

  // Start the game loop
  run_game();
}

void run_game() {

  // DEBUG
  CACTUS_X = GRAPHICS_WIDTH - 24;
  CACTUS_Y = DINO_Y - 2;

  set_sprite_tile(CACTUS_1, CACTUS_1_TILE);
  set_sprite_tile(CACTUS_2, CACTUS_2_TILE);

  move_sprite(CACTUS_1, CACTUS_X, CACTUS_Y);
  move_sprite(CACTUS_2, CACTUS_X + 8, CACTUS_Y);

  while(1) {
    wait_vbl_done();

    // Handle Dino feet sprite swapping
    move_dino();

    // Handle Dino collision with 
    if (check_collisions(DINO_X, DINO_Y, 12, 12, CACTUS_X, CACTUS_Y, 8, 8) == 1) {
      game_over();
    }

    // Handle updating score from rightmost digit
    update_score(SCORE_DIGITS);

    key_press = joypad();
    if (key_press & J_A) {
      dino_jump();
    }

    if (key_press & J_DOWN) {
      // Set ducking Dino running sprites
      set_sprite_tile(DINO_1, DUCK_DINO_TAIL);
      set_sprite_tile(DINO_2, DUCK_DINO_LEFT);
      set_sprite_tile(DINO_3, DUCK_DINO_BODY);
      set_sprite_tile(DINO_4, DUCK_DINO_HEAD);

      // Move the ducking Dino sprites into place
      move_sprite(DINO_1, DINO_X - DINO_SPRITE_X_SIZE, DINO_Y + 3);
      move_sprite(DINO_2, DINO_X, DINO_Y + 3);
      move_sprite(DINO_3, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y + 3);
      move_sprite(DINO_4, DINO_X + DINO_SPRITE_X_SIZE + DINO_SPRITE_X_SIZE, DINO_Y + 3);

      dino_duck();

      // Return to non-ducking Dino running sprites
      set_sprite_tile(DINO_1, RIGHT_FOOT_DINO_1_TILE);
      set_sprite_tile(DINO_2, RIGHT_FOOT_DINO_2_TILE);

      // Move the non-ducking Dino sprites into place
      move_sprite(DINO_1, DINO_X,  DINO_Y);
      move_sprite(DINO_2, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y);

      // Hide the extra loaded sprites off screen
      move_sprite(DINO_3, GRAPHICS_WIDTH + DINO_SPRITE_X_SIZE, GRAPHICS_HEIGHT);
      move_sprite(DINO_4, GRAPHICS_WIDTH + DINO_SPRITE_X_SIZE, GRAPHICS_HEIGHT);
    }

    // Scroll objects to the left
    // move_hazards();

    //Scroll the background
    // move_world();

  }
}

BOOLEAN new_high_score() {

}

int main() {

  // Hide graphical objects while the game is being set up
  HIDE_WIN;
  HIDE_BKG;
  HIDE_SPRITES;

  // Performs a bitwise inclusive OR on LCDC_REG(0xFF40) with 0x80U
  // (Turns on bit 7 of the 8-bit LCD Control Register)
  DISPLAY_ON;

  // Disable interrupts as they are not being used
  disable_interrupts();

  // Turn on sound
  NR52_REG = 0x80;

  // Select sound channel 1
  NR51_REG = 0x11;

  // Crank up the volume
  NR50_REG = 0x77;

  // Use two 8x8 sprites stacked on top of one another
  SPRITES_8x16;

  DINO_X = 0x14;                 // Set DINO_X position as 20
  DINO_Y = GRAPHICS_HEIGHT - 17; // Y position is relative to the bottom of the screen

  // Load all the Dino sprites at once
  set_sprite_data(0, DINO_TILE_COUNT, Dino);

  // Load and place digit sprites after dino sprites
  set_sprite_data(DINO_TILE_COUNT, DIGITS_TILE_COUNT, Digits);

  // Load and place the hazard sprites after the UI symbols
  set_sprite_data(DINO_TILE_COUNT + DIGITS_TILE_COUNT, HAZARDS_TILE_COUNT, Hazards);

  // The dino sprite is comprised of 2 smaller sprites that make one 8x16 sprite
  // Load both parts of the Dino sprite into the first two tile slots
  set_sprite_tile(DINO_1, IDLE_DINO_1_TILE);
  set_sprite_tile(DINO_2, IDLE_DINO_2_TILE);

  // Place both parts of the Dino sprite in the play area and arrange them side by side
  move_sprite(DINO_1, DINO_X,  DINO_Y);
  move_sprite(DINO_2, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y);

  // Load the tiles used to write GAME OVER on a window
  set_win_data(0, LETTERS_TILE_COUNT, Letters);
  set_win_tiles(0, 0, GameoverWidth, GameoverHeight, Gameover);


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
