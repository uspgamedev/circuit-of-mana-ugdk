
Circuit of Mana (for UGDK)
==========================

This is a prototype of our Circuit of Mana project using UGDK.

How to Compile
--------------

### On Linux

You need the following packages:

  ```
  cmake >= 2.8.11
  git >= 1.7.10
  ```

After cloning this repository, go inside its root directory and use the commands:

  ```bash
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  ```

It generates an executable for each POV version of the prototype:

  ```
  src/platformer/circuit-of-mana-platformer
  ```

### On Windows

You will need the following software:

  ```
  CMake 2.8.11 (or higher)
  Visual Studio 2013 (or newer)
  ```

After cloning this repository, run the cmake-gui utlity, point the "Where is the source code"
to the repository root and the "Where to build the binaries" to any directory you wish.
Recommended is a directory named "build".

Then, press "Configure" and choose "Visual Studio 12 2013" (optionally Win64) and then "Generate".
With that done, there should be a Visual Studio solution in the binary directory.
 

