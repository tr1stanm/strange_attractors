# strange_attractors

This is a program for simulation of 3D strange attractor models. It includes controls for manipulation of the attractors in 3D space via rotation, zooming, adjustment of camera depth, panning, and various color schemes.

It also includes functionality for saving videos and screenshots.

This program is run from the command line but aside from confirmation at the beginning of the program of user preference on exporting a video file or not, all controls are given to the program via the keyboard when interfacing with the program window.

The attractors currently supported with this software are:

- Lorenz Attractor
- Halvorsen Attractor
- Dadras Attractor
- Langford/Aizawa Attractor
- Three-Scroll Unified System (it's a little buggy though)
- Rabinovich-Fabrikant Attractor
- Rossler Attractor
- Sprott-Linz Attractor
- Sprott B Attractor
- Arneodo Attractor

## Getting Started

### Dependencies

SDL3 (https://wiki.libsdl.org/SDL3/FrontPage)

CMake (https://cmake.org/)

GNU Scientific Library (https://www.gnu.org/software/gsl/)

FFMPEG (https://ffmpeg.org/)

If you have homebrew, apt, or a similar package manager, these tools can all be installed using those.

This project has only been compiled and tested on Mac, but afaik SDL has pretty decent cross-compilation support. I'd be curious to hear if it works well or at all on other platforms.

### Installing

Once the dependencies have been installed, navigate to the project directory in your terminal and run the command:

`cmake -B build`

followed by:

`cmake --build build`


This should generate an executable ("dynsys") in your build folder. Run the file by double clicking it, or from the command line with 

`./build/dynsys` 

If you should decide to record video, the video file will be saved as "anim.mp4" in a folder marked "anims" in the project directory folder.

## Keyboard Controls

This program uses keyboard controls to change the attractor being displayed.

- W / S: Change X rotation
- A / D: Change Y rotation
- Z / X: Change Z rotation
- Q / E: Change overall rotation (rotation scale multiplier)
- Up / Down: Change depth of field
- Left / Right: Change image scale
- Space: Stop rotation
- I / K / J / L: Pan image
- O / P: Change color scheme
- < / >: Change current attractor being displayed
- C: Take a screenshot (saves to screenshots folder in project directory)
- The number keys (1-0) can also be used to switch to any of the 10 different attractors more quickly if desired.

## Screenshots and Examples

Here are some screenshots of various dynamic systems generated using this program utilizing different color schemes.

![frame_000756](https://github.com/user-attachments/assets/aef1fcd9-b625-4678-aec2-29294878df33)

![frame_001237](https://github.com/user-attachments/assets/173a18b8-8297-484b-b46c-e602a88f5bf7)

![frame_001598](https://github.com/user-attachments/assets/d7774096-294e-4919-be28-5df343398b53)

![frame_001609](https://github.com/user-attachments/assets/62ed0b3c-4043-4f98-a183-f0857c22bd8b)

![frame_001707](https://github.com/user-attachments/assets/b7d78273-35c2-4bc9-95f5-ab632d6e6386)

# Author

All code and testing for this project not mentioned as part of the dependencies was written by Tristan Miller.

Please credit me if you refer to it somewhere.
