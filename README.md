# Mesen-X

Mesen-X is a fork of Mesen, which is a cross-platform NES/Famicom emulator for Windows & Linux built in C++ and C#.
The original Mesen project has paused development, and this fork is meant to collect patches contributed by the community.

Changes include:
* Support for newer versions of Mono, for running Mesen on Linux
* ESPM homebrew expansion audio support
* HD Pack improvements
* Replaced VRC7 synth code with Nuked-OPLL
* Debugger improvements, for [Alchemy65](https://github.com/AlchemicRaker/alchemy65)

[Original website (https://www.mesen.ca)](https://www.mesen.ca)  
[Documentation](https://novasquirrel.github.io/Mesen-X/index.html)

You should probably use [Mesen2](https://mesen.ca/) instead if you are able to.

## Development Builds

Development builds of the latest commit are available from GitHub actions. For stable release builds, see the **Releases** section below.

**Warning:** These are development builds and may be ***unstable***. Using them may also increase the chances of your settings being corrupted, or having issues when upgrading to the next official release. Additionally, these builds are currently not optimized via PGO and will typically run 20-30% slower than the official release builds.

Follow either of these, select the top-most entry in the list, and then click the link under "Artifacts" near the bottom.

Windows: [![Build status](https://github.com/NovaSquirrel/Mesen/actions/workflows/win-build.yml/badge.svg)](https://github.com/NovaSquirrel/Mesen/actions/workflows/win-build.yml)

Linux: [![Build status](https://github.com/NovaSquirrel/Mesen/actions/workflows/linux-build.yml/badge.svg)](https://github.com/NovaSquirrel/Mesen/actions/workflows/linux-build.yml)

## Releases

### Windows

The latest versions can be found on [the releases tab](https://github.com/NovaSquirrel/Mesen-X/releases)

### Ubuntu

There's a separate Linux release available on each release in the releases tab. You should be able to use that in most cases if you are using Ubuntu.

The Linux version is a standard .NET executable file and requires Mono to run - you may need to configure your environment to allow it to automatically run .exe files through Mono, or manually run Mesen by using mono (e.g: "mono Mesen.exe").

The following packages need to be installed to run Mesen:

* mono-complete
* libsdl2-2.0
* gnome-themes-standard

**Note:** **Mono 5.18 or higher is recommended**, some older versions of Mono (e.g 4.2.2) have some stability and performance issues which can cause crashes and slow down the UI.
The default Mono version in Ubuntu 18.04 is 4.6.2 (which also causes some layout issues in Mesen).  To install the latest version of Mono, follow the instructions here: https://www.mono-project.com/download/stable/#download-lin

## Roadmap

Things that ***may or may not*** be added in the future, in no particular order:

* Support for more UNIF boards and more NES/Famicom input devices
* Shaders
* TAS editor
* Support for new homebrew mappers
* More Lua features

## Compiling

See [COMPILING.md](COMPILING.md)

## License

Mesen is available under the GPL V3 license.  Full text here: <http://www.gnu.org/licenses/gpl-3.0.en.html>

Copyright (C) 2014-2019 M. Bibaud

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
