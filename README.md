# LearnDirectX

This project consists in a bunch of lybraries used for 3D rendering with DirectX11. They provide a base upon which to 
create your own 3D apps, also coming with some examples and a template to get started.

The project is still a work in progress, so many changes may be added in the future, currently is probably not suited 
for distribution.

## Requirements

- [Visual Studio](https://visualstudio.com) (Is important for the program to be able to unite all the diffent files)
- OS Windows 7 or newer

## Contents

The project currently contains 5 different lybraries, they all have some functions needed for the apps to work properly. 
They are the following:

- **ImGui**: These are some external dependencies used only for the widgets. For more information on what imGui is and
  how to use refer to its [repository](https://github.com/ocornut/imgui).
- **dxerr**: This is just a library that includes some functions for error detection in our program.
- **Math**: This library contains a bunch of math constructions that will be used by the rest of dependencies to do
  their calculations. It includes 2D and 3D vectors, 3x3 matrices, complex numbers and quaternions.
- **Image**: This library contains two important constructions for our applications, the color class, for color handling
  and with some useful predefined colors, and the Image class, which is just an array of colors and allows for easy texture
  implementation and image edition. For the loading and saving of image files it uses
  [FreeImage](https://github.com/jpvanoosten/FreeImage).
- **WindowCreation**: This is the main library of the solution and is the one that we will be including in our applications,
  here the DirectX11 and Win32 tools are implemented and provides the tools for window creation and graphics creation.

Due to the fact that the project is still in progress no more information about its usage will be provided. If you are 
looking for a way to implement a 3D renderer for mathematical purposes in C++ this might be the library for you, for further 
information please contact me directly.
