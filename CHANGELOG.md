# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/).

## [To-Do]
  - Pterodactyl sprites
  - Collisions
    - Collisions with Pterodactyl
    - Collision from above
  - Difficulty increase
    - Night shift after certain score
  - All the sound effects
    - Score checkpoint sound
  - Score Window
    - Hi Score tracking
    - Flash window after 100
  - Parallax Scrolling background
  - Extras
    - Battery Saving
      - Toggle sounds (write 0x00 to NR52_REG for sound OFF, 0x80 for sound ON)
      - Perform HALT when screen is paused (interrupt to remove HALT)

## [0.1.0] - 2017-06-01
### Added
  - Game Over window after collision.
  - Game Over sound effect.
  - Restart after game over by hitting A on the joypad.

### Changed
  - Moved the score sprites further up the screen.
  - Separated UI symbols into *Digits* and *Letters*.
  - Changed the load order of sprites into VRAM.
  - Changed hexadecimal macros due to changing the load order.

## [0.0.4] - 2017-06-01
### Added
  - Loading and scrolling Hazards (Cactus)
  - Basic rectangle collision checks

### Changed
  - Changed `int` declarations to `UBYTE` declarations

## [0.0.3] - 2017-05-28
### Added
  - Basic score keeping by sprite swapping numbers.

### Fixed
  - line() call before `wait_vbl_done()` was affecting VRAM. Deleted all `line()` calls.

## [0.0.2] - 2017-05-13
### Added
  - Rough Dino ducking animation while holding DOWN on the joypad.

### Changed
  - Jumping height is affected by how long the A button is pressed.
  - Hexadecimal notation for VRAM macros instead of decimal notation.


## [0.0.1] - 2017-05-13
### Added
  - Initial commit.
