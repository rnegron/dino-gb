/*

  ________ _____                       _________________
  ___  __ \___(_)_______ ______        __  ____/___  __ )
  __  / / /__  / __  __ \_  __ \       _  / __  __  __  |
  _  /_/ / _  /  _  / / // /_/ /       / /_/ /  _  /_/ /
  /_____/  /_/   /_/ /_/ \____/        \____/   /_____/

  Demo!


 TODO:
    ! Dino ducking
    - Other sprites
      -- 'Try again' window
      -- Pterodactyl
      -- Collisions
    - Difficulty increase
      -- Score Window
        -- Flash score on 100 interval and perform delta
          scoreParts[0] = score / 10000;
          scoreParts[1] = score / 1000 - scoreParts[0] * 100;
          scoreParts[2] = score / 100 - (scoreParts[0] * 1000 + scoreParts[1] * 100);
          scoreParts[3] = score - (scoreParts[0] * 10000 + scoreParts[1] * 1000 + scoreParts[2] * 100);
          scoreParts[4] = score - (scoreParts[0] * 100000 + scoreParts[1] * 10000 + scoreParts[2] * 1000 + scoreParts[3] * 100)
      -- Score Delta Time on jumps
    - Background
      -- Parallax Scrolling
      -- Night shift after certain score

    - Extras
      -- Battery Saving
        --- Toggle sounds (write 0x00 to NR52_REG for sound OFF, 0x80 for sound ON)
        --- Perform HALT when screen is paused (interrupt to remove HALT)

*/

#include <gb/gb.h>
#include <gb/font.h>
#include <gb/drawing.h>
#include <rand.h>
#include "dino.c"
#include "cactus.c"
#include "symbols.c"

#define DINO_X 20
#define DINO_Y GRAPHICS_HEIGHT - 17

#define DINO_TILE_COUNT 16
#define DINO_SPRITE_X_SIZE 8
#define IDLE_DINO_1_TILE 0
#define IDLE_DINO_2_TILE 2
#define HURT_DINO_1_TILE 4
#define HURT_DINO_2_TILE 6
#define RIGHT_FOOT_DINO_1_TILE 8
#define RIGHT_FOOT_DINO_2_TILE 10
#define LEFT_FOOT_DINO_1_TILE 12
#define LEFT_FOOT_DINO_2_TILE 14
#define DUCK_DINO_1_TILE 16
#define DUCK_DINO_2_TILE 18

// Dino Sprite Indexes
#define IDLE_DINO_1 0
#define IDLE_DINO_2 1
#define HURT_DINO_1 2
#define HURT_DINO_2 3
#define RIGHT_STEP_DINO_1 4
#define RIGHT_STEP_DINO_2 5
#define LEFT_STEP_DINO_1 6
#define LEFT_STEP_DINO_2 7


// States
#define DINO_IS_IDLE 0
#define DINO_FOOT_RIGHT_DOWN 1
#define DINO_FOOT_LEFT_DOWN 2

#define CACTUS_TILE_COUNT 4
#define CACTUS_TILE 16
#define CACTUS_1 8
#define CACTUS_2 9

// UI Symbols
#define SYMBOLS_TILE_COUNT 52

// 1 == right foot down, 2 == left foot down
UBYTE dino_state;

// Holds the return value of the joypad() function
UBYTE key_press;

// Reusable counter variable
UBYTE i;

void start_jump();
void move_dino();
void move_hazards();
void dino_jump();
void run_game();

void start_jump() {
  line(0, GRAPHICS_HEIGHT - 20, GRAPHICS_WIDTH - 1, GRAPHICS_HEIGHT - 20);
  dino_jump();

  // Set dino_state as 2 so that move_dino() changes it to 1 (right foot first)
  dino_state = DINO_FOOT_LEFT_DOWN;
}

void move_dino() {
  switch(dino_state) {

    case DINO_FOOT_LEFT_DOWN:
      set_sprite_tile(0, RIGHT_FOOT_DINO_1_TILE);
      set_sprite_tile(1, RIGHT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_RIGHT_DOWN;
      delay(80);
      break;

    case DINO_FOOT_RIGHT_DOWN:
      set_sprite_tile(0, LEFT_FOOT_DINO_1_TILE);
      set_sprite_tile(1, LEFT_FOOT_DINO_2_TILE);
      dino_state = DINO_FOOT_LEFT_DOWN;
      delay(90);
      break;

    default:
      break;
  }
}

void move_hazards() {}

void dino_jump() {

  // Change the sprite to Idle Dino for jump
  set_sprite_tile(0, IDLE_DINO_1_TILE);
  set_sprite_tile(1, IDLE_DINO_2_TILE);

  // Play the jump sound
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

  // Moving up...
  for (i = DINO_Y; i != DINO_Y - 50; --i) {
    move_sprite(0, DINO_X, i);
    move_sprite(1, DINO_X + DINO_SPRITE_X_SIZE, i);
    delay(8);
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

void run_game() {

  while(1) {
    wait_vbl_done();

    // Handle Dino feet sprite swapping
    move_dino();

    key_press = joypad();
    if (key_press & J_A)     dino_jump();
    // if (key_press & J_DOWN)  dino_duck();

    // Scroll objects to the left
    // move_hazards();

    // Check collisions
    // check_collisions();

    //Scroll the background
    // move_world();

    wait_vbl_done();
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
  set_sprite_data(DINO_TILE_COUNT + 1, CACTUS_TILE_COUNT, Cactus);
  set_sprite_tile(2, CACTUS_TILE);

  // Load and place the UI symbols after the hazard sprites
  set_sprite_data(DINO_TILE_COUNT + CACTUS_TILE_COUNT + 1, SYMBOLS_TILE_COUNT, Symbols);

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
