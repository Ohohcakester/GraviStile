GraviStile
==========

![GraviStile](../screenshot/screenshot/ss_main.png)

GraviStile is a Puzzle Platformer, based around a simple mechanic of rotating platforms to change the direction of gravity.
Face 24 challenging, carefully crafted levels in this mind-bending platformer!

## Download
* [**Download (Windows)** [640KB]](https://github.com/Ohohcakester/GraviStile/releases/download/2.0/GraviStile_v2_00_Windows.zip)
* [**Download (Linux)** [430KB]](https://github.com/Ohohcakester/GraviStile/releases/download/2.0/GraviStile_v2_00_Linux.gz)
* [**Release Page**](https://github.com/Ohohcakester/GraviStile/releases)


## How to Play
* Rotating a platform changes the direction of gravity.
* You can only rotate the platform you are standing on.
* Reach the door to finish the stage!

## Controls
* **Arrow Keys**: Movement
* **Space**: Jump
* **A, D**: Rotate platform Left / Right

## New in v2.00
- New stages, making a total of 24 stages.
- New mechanics - linked platforms, lasers and switches
- Many bug fixes and other improvements

### Planned
- Level Editor (it exists and works, but not ready for release)


## Screenshots
![platforms](../screenshot/screenshot/ss_grav1.png)
![lasers](../screenshot/screenshot/ss_grav2.png)
![confusion](../screenshot/screenshot/ss_grav3.png)


## Building GraviStile on your Platform

GraviStile uses SFML (Simple and Fast Multimedia Library), which is available here:
- http://www.sfml-dev.org

### Windows

Follow the instructions to configure an SFML project in Visual Studio here.
- http://www.sfml-dev.org/tutorials/2.3/start-vc.php

Alternatively, install the latest version of MinGW and download the compatible SFML library [here](http://www.sfml-dev.org/download/sfml/2.3.2/), then follow the instructions for compiling on Linux.


### Linux

Quickest way:
- Install SFML: `sudo apt-get install libsfml-dev`
- Then run `build.sh`. This builds the game into the _build directory.

Alternatively, you can follow the instructions to compile an SFML app here:
- http://www.sfml-dev.org/tutorials/2.3/start-linux.php
- Note: compile with the flag `-std=c++11` if using g++.
