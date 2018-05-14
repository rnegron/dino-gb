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
#include <rand.h>
#include "dino.c"
#include "cactus.c"
#include "symbols.c"

/* MACROS START */
#define DINO_X 0x14
#define DINO_Y GRAPHICS_HEIGHT - 17 // Y position is relative to the bottom of the screen

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
#define HURT_DINO_1 0x02
#define HURT_DINO_2 0x03
#define RIGHT_STEP_DINO_1 0x04
#define RIGHT_STEP_DINO_2 0x05
#define LEFT_STEP_DINO_1 0x06
#define LEFT_STEP_DINO_2 0x07
#define DUCK_DINO_BODY 0x10
#define DUCK_DINO_HEAD 0x12
#define DUCK_DINO_TAIL 0x14
#define DUCK_DINO_LEFT 0x16
#define DUCK_DINO_RIGHT 0x1A

// States
#define DINO_IS_IDLE 0
#define DINO_FOOT_RIGHT_DOWN 1
#define DINO_FOOT_LEFT_DOWN 2

#define CACTUS_TILE_COUNT 0x04
#define CACTUS_TILE 0x16
#define CACTUS_1 0x08
#define CACTUS_2 0x09

// UI Symbols
#define SYMBOLS_TILE_COUNT 0x34

// Extra macros
#define FOOT_SWITCH_SPEED 100 // Microseconds

/* MACROS END */


/* GLOBALS START */
// 1 == right foot down, 2 == left foot down
UBYTE dino_state;

// Holds the return value of the joypad() function
UBYTE key_press;

// Reusable counter variable
UBYTE i;
/* GLOBALS START */

/* PROTOTYPES START */
void start_jump();
void play_jump_noise();

void move_dino();
void dino_jump();
void dino_duck();

void move_hazards();

void run_game();
/* PROTOTYPES END */

/* FUNCTIONS START */
void start_jump() {
  // Runs after the player presses the A buton after the game boots
  line(0, GRAPHICS_HEIGHT - 20, GRAPHICS_WIDTH - 1, GRAPHICS_HEIGHT - 20);
  dino_jump();

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

void move_hazards() {}


void run_game() {

  while(1) {
    wait_vbl_done();

    // Handle Dino feet sprite swapping
    move_dino();

    key_press = joypad();
    if (key_press & J_A) {
      dino_jump();
    }

    if (key_press & J_DOWN) {
      wait_vbl_done();
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
    // move_hazards();

    // Check collisions
    // check_collisions();

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

  // Wait for V_BLANK interrupt (screen drawing refresh)
  wait_vbl_done();

  // DEBUG: Draw a line as temporary background
  // The syntax is line(source x, source y, destination x, destination y)
  line(0, GRAPHICS_HEIGHT - 20, 20, GRAPHICS_HEIGHT - 20);

  // Load all the Dino sprites at once
  set_sprite_data(0, DINO_TILE_COUNT, Dino);

  // The dino sprite is comprised of 2 smaller sprites that make one 8x16 sprite
  // Load both parts of the Dino sprite into the first two tile slots
  set_sprite_tile(0, IDLE_DINO_1_TILE);
  set_sprite_tile(1, IDLE_DINO_2_TILE);

  // Place both parts of the Dino sprite in the play area and arrange them side by side
  move_sprite(0, DINO_X,  DINO_Y);
  move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, DINO_Y);

  // Load and place the cactus sprite after the Dino Sprite
  // set_sprite_data(DINO_TILE_COUNT + 1, CACTUS_TILE_COUNT, Cactus);
  // set_sprite_tile(2, CACTUS_TILE);

  // Load and place the UI symbols after the hazard sprites
  // set_sprite_data(DINO_TILE_COUNT + CACTUS_TILE_COUNT + 1, SYMBOLS_TILE_COUNT, Symbols);

  SHOW_SPRITES;

  // Static screen, wait for user input (an A button press)
  waitpad(J_A);

  // The first jump has no sound but scrolls in the rest of the background
  start_jump();

  // DEBUG: Demo mode
  run_game();

}
/* FUNCTIONS END */

/* Code dump

scoreParts[0] = score / 10000;
scoreParts[1] = score / 1000 - scoreParts[0] * 100;
scoreParts[2] = score / 100 - (scoreParts[0] * 1000 + scoreParts[1] * 100);
scoreParts[3] = score - (scoreParts[0] * 10000 + scoreParts[1] * 1000 + scoreParts[2] * 100);
scoreParts[4] = score - (scoreParts[0] * 100000 + scoreParts[1] * 10000 + scoreParts[2] * 1000 + scoreParts[3] * 100)

*/
