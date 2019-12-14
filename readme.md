<a href="https://repology.org/metapackage/teeworlds/versions">
    <img src="https://repology.org/badge/vertical-allrepos/teeworlds.svg" alt="Packaging status" align="right">
</a>

Teeworlds [![CircleCI](https://circleci.com/gh/teeworlds/teeworlds.svg?style=svg)](https://circleci.com/gh/teeworlds/teeworlds) [![Travis CI Build Status](https://travis-ci.org/teeworlds/teeworlds.svg?branch=master)](https://travis-ci.org/teeworlds/teeworlds) [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/teeworlds/teeworlds?branch=master&svg=true)](https://ci.appveyor.com/project/heinrich5991/teeworlds)
=========

A retro multiplayer shooter
---------------------------

Teeworlds is a free online multiplayer game, available for all major
operating systems. Battle with up to 16 players in a variety of game
modes, including Team Deathmatch and Capture The Flag. You can even
design your own maps!

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software. See license.txt for full license
text including copyright information.

Please visit https://www.teeworlds.com/ for up-to-date information about
the game, including new versions, custom maps and much more.

Originally written by Magnus Auvinen.

--------

Teeworlds supports two build systems: bam and CMake. Below are build instructions for the major operating systems.


Building on Linux or macOS
==========================

Installing dependencies
-----------------------

    # Debian/Ubuntu
    sudo apt install build-essential git libfreetype6-dev libsdl2-dev libpnglite-dev libwavpack-dev python3
    
    # Fedora
    sudo dnf install @development-tools gcc-c++ git freetype-devel mesa-libGLU-devel pnglite-devel python3 SDL2-devel wavpack-devel
    
    # Arch Linux (doesn't have pnglite in its repositories)
    sudo pacman -S --needed base-devel freetype2 git glu python sdl2 wavpack
    
    # macOS
    brew install freetype sdl2

You also need `bam` or `cmake` installed.

Downloading repository
----------------------

    git clone https://github.com/teeworlds/teeworlds --recurse-submodules
    cd teeworlds
    
    # If you already cloned the repository before, use:
    # git submodule update --init

Building 
----------------------

### bam

    bam all

### CMake

    mkdir -p build
    cd build
    cmake ..
    make

On subsequent builds, you only have to repeat the `make` step.

You can then run the client from the build repository with `./teeworlds` and the server with
`./teeworlds_srv`.


Build options
-------------

### bam

By default, `bam` compiles client and server in **debug** mode. You can specify

`conf=release`: Enable release mode and subsequent optimizations

`server`: Disable generation of the client target. Can be useful on
headless servers which don't have graphics libraries like SDL2 installed.


### CMake

The following options can be passed to the `cmake ..` command line (between the
`cmake` and `..`) in the "Building" step above.

`-GNinja`: Use the Ninja build system instead of Make. This automatically
parallizes the build and is generally **faster**. (Needs `sudo apt install
ninja-build` on Debian, `sudo dnf install ninja-build` on Fedora, and `sudo
pacman -S --needed ninja` on Arch Linux.)

`-DDEV=ON`: Enable debug mode and disable some release mechanics. This leads to
**faster** builds.

`-DCLIENT=OFF`: Disable generation of the client target. Can be useful on
headless servers which don't have graphics libraries like SDL2 installed.


Building on Windows with Visual Studio
======================================

Download and install some version of [Microsoft Visual Studio](https://visualstudio.microsoft.com/en/downloads/)] (as of writing, MSVS Community 2017) with the following components:

* Desktop development with C++ (on the main page)
* Python development (on the main page)
* Git for Windows (in Individual Components → Code tools, optional for bam)

### bam

1. Download and unzip [Teeworlds source](https://github.com/teeworlds/teeworlds/releases) or [Teeworlds latest source](https://github.com/teeworlds/teeworlds/archive/master.zip) or use git to clone it
2. Download and unzip [bam](https://github.com/teeworlds/bam/archive/master.zip) to `teeworlds-version\bam`
3. Run the `x64 Native Tools Command Prompt` (or `x86 Native Tools Command Prompt` for x86) from the start menu.
4. Navigate to `teeworlds-version\bam` and run `make_win64_msvc.bat` (or `make_win32_msvc.bat` for x86) to compile bam
5. Navigate to `teeworlds-version` and run `bam\bam conf=release all ` to compile Teeworlds
   - to build the debug version instead, use `conf=debug`
   - 

### CMake

Run Visual Studio. Open the Team Explorer (View → Team Explorer, Ctrl+^,
Ctrl+M). Click Clone (in the Team Explorer, Connect → Local Git Repositories).
Enter `https://github.com/teeworlds/teeworlds` into the first input box. Wait
for the download to complete (terminals might pop up).

Wait until the CMake configuration is done (watch the Output windows at the
bottom).

Select `teeworlds.exe` in the Select Startup Item… combobox next to the green
arrow. Wait for the compilation to finish.

For subsequent builds you only have to click the button with the green arrow
again.


Building on Windows with MinGW and CMake
==============================

Download and install MinGW with at least the following components:

- mingw-developer-toolkit-bin
- mingw32-base-bin
- mingw32-gcc-g++-bin
- msys-base-bin

Also install [Git](https://git-scm.com/downloads) (for downloading the source
code), [Python](https://www.python.org/downloads/) and
[CMake](https://cmake.org/download/).

Open CMake ("CMake (cmake-gui)" in the start menu). Click "Browse Source"
(first line) and select the directory with the Teeworlds source code. Next,
click "Browse Build" and create a subdirectory for the build (e.g. called
"build"). Then click "Configure". Select "MinGW Makefiles" as the generator and
click "Finish". Wait a bit (until the progress bar is full). Then click
"Generate".

You can now build Teeworlds by executing `mingw32-make` in the build directory.
