# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/).

## [To-Do]
  - Clock based timing, instead of delay()
  - Pterodactyl sprites
  - Collisions
    - Collisions with Pterodactyl
    - Collision from above
  - Difficulty increase
    - Night shift after certain score
    - Scroll speed increases as the score gets higher 
  - Score Window
    - Hi Score tracking
  - Parallax Scrolling background
  - Extras
    - Battery Saving
      - Toggle sounds (write 0x00 to NR52_REG for sound OFF, 0x80 for sound ON)
      - Perform HALT when screen is paused (interrupt to remove HALT)


## [0.1.1] - 2018-07-10
### Added
  - Sound effect plays whenever the score arrives at a multiple of 100.
  - Score sprites flash whenever the score arrives at a multiple of 100.
  - Added Visual Studio Code folder with tasks to `.gitignore`
  - Build step to Makefile
  - Put comments in all the blank comment lines
### Changed
  - CHANGELOG.md had the years set as 2017, should be 2018.
  - Changed the last part of the sound function names (from `_noise` to _`sound`).
  - Removed some unnecesary registers from the sound functions.
  - Extended README.md

## [0.1.0] - 2018-06-01
### Added
  - Game Over window after collision.
  - Game Over sound effect.
  - Restart after game over by hitting A on the joypad.

### Changed
  - Moved the score sprites further up the screen.
  - Separated UI symbols into *Digits* and *Letters*.
  - Changed the load order of sprites into VRAM.
  - Changed hexadecimal macros due to changing the load order.

## [0.0.4] - 2018-06-01
### Added
  - Loading and scrolling Hazards (Cactus)
  - Basic rectangle collision checks

### Changed
  - Changed `int` declarations to `UBYTE` declarations

## [0.0.3] - 2018-05-28
### Added
  - Basic score keeping by sprite swapping numbers.

### Fixed
  - line() call before `wait_vbl_done()` was affecting VRAM. Deleted all `line()` calls.

## [0.0.2] - 2018-05-13
### Added
  - Rough Dino ducking animation while holding DOWN on the joypad.

### Changed
  - Jumping height is affected by how long the A button is pressed.
  - Hexadecimal notation for VRAM macros instead of decimal notation.


## [0.0.1] - 2018-05-13
### Added
  - Initial commit.
