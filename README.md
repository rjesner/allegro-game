# Allegro Game

In this simple C++/CMake project, the goal is to create a basic 2D game using the Allegro library. It creates a grid template for RPG games with the correct pixel coordinates and player animations.

## Requisites

- Windows 10
- CMake >= 3.16

This project comes with a prebuilt/binary version of the Allegro library for x64 Windows. If you want to build Allegro yourself, the requisites are:

- MinGW-w64 8.1.0 posix-dwarf
- Allegro 5.25

## Usage

Just build the project using the following CMake commands.

```bash
$> cd allegro-game
$> mkdir build
$> cd build
$> cmake ..
$> cmake -G "MinGW Makefiles" ..
```