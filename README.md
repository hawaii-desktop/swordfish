Swordfish
=========

[![GitHub release](https://img.shields.io/github/release/hawaii-desktop/swordfish.svg)](https://github.com/hawaii-desktop/swordfish)
[![GitHub issues](https://img.shields.io/github/issues/hawaii-desktop/swordfish.svg)](https://github.com/hawaii-desktop/swordfish/issues)
[![IRC Network](https://img.shields.io/badge/irc-freenode-blue.svg "IRC Freenode")](https://webchat.freenode.net/?channels=hawaii-desktop)

This is Swordfish, the file manager for the Hawaii desktop environment.

## Dependencies

Qt >= 5.6 with at least the following modules is required:

* [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
* [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
* [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

The following modules and their dependencies are required:

* [ECM >= 1.4.0](http://quickgit.kde.org/?p=extra-cmake-modules.git)

## Build

Building Swordfish is a piece of cake.

Assuming you are in the source directory, just create a build directory
and run cmake:

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii ..
```

To do a debug build the last command can be:

```sh
cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Debug ..
```

To do a release build instead it can be:

```sh
cmake -DCMAKE_INSTALL_PREFIX=/opt/hawaii -DCMAKE_BUILD_TYPE=Release ..
```

If not passed, the `CMAKE_INSTALL_PREFIX` parameter defaults to /usr/local.
You have to specify a path that fits your needs, /opt/hawaii is just an example.

Package maintainers would pass `-DCMAKE_INSTALL_PREFIX=/usr`.

The `CMAKE_BUILD_TYPE` parameter allows the following values:

* **Debug:** debug build
* **Release:** release build
* **RelWithDebInfo:** release build with debugging information

## Installation

It's really easy, it's just a matter of typing:

```sh
make install
```

from the build directory.

# Licensing

## QtQuick Plugins

Licensed under the terms of the GNU Lesser General Public License version 3
as published by the Free Software Foundation and appearing in the file
LICENSE.LGPLv3 included with this software.

## Application

Licensed under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation and appearing in the file
LICENSE.GPL2 included with this software, or (at your option) any later
version.
