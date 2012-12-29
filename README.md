Swordfish
=========

This is Swordfish, the file manager for the Hawaii desktop.

## Dependencies

In order to build and install Swordfish you need Vibe.
The Vibe repository contains all the instructions to bring it up.

## Build

Building Swordfish is a piece of cake.

Assuming you are in the source directory, just create a build directory
and run cmake:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii ..

To do a debug build the last command can be:

    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Debug ..

To do a release build instead it can be:

    cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Release ..

If not passed, the CMAKE_INSTALL_PREFIX parameter defaults to /usr/local.
You have to specify a path that fits your needs, /opt/hawaii is just an example.

Package maintainers would pass *-DCMAKE_INSTALL_PREFIX=/usr*.

The CMAKE_BUILD_TYPE parameter allows the following values:

    Debug: debug build
    Release: release build
    RelWithDebInfo: release build with debugging information

## Installation

It's easy really, it's just a matter of typing:

    make install

from the build directory.
