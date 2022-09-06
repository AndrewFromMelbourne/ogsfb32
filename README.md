# ogsfb32
This repository contains a C++ convenience library for directly manipulating
the Linux Framebuffer on an Odroid Go Super. It was written specifically to
access the 32 bit per pixel framebuffer on the Odroid Go Super.

# libraries

You will need to install libbsd-dev

        sudo apt-get install libbsd-dev

# libogsfb32
The library itself.

# test
A simple test programs

# [osginfo](osginfo/README.md)
A program to display Odroid Go Super specific system information directly on
the framebuffer.

# [Life](life/README.md)
Conway's Game of Life

# [Boxworld](boxworld/README.md)
A version of Boxworld or Sokoban for the Odroid Super Go, written in C++.

# build
```
    cd ogsfb32
    mkdir build
    cd build
    cmake ..
    make
```
