<h1 align="center">Set-P</h1>
<p align="center">Add a path to PATH, straight from your terminal</h3>

Set-P is powered by [CLI11](https://github.com/CLIUtils/CLI11)

## What is Set-P?
Set-P is a command line tool made for windows that allows you to add paths to the global PATH environment variable straight from your command line.

Note: You still will need to refresh the environment in each shell manually, if you have Chocolatey installed, you may use refreshenv

## Usage
Syntax: setp `[OPTIONS] [path]`

Where `path` is the path you want to add to the global PATH environment variable. (If path contains spaces, it must be surrounded by quotation marks)

### Flags
`-f` - Adds the path to the beginning of the PATH environment variable instead of at the end. Useful for overriding other paths
`-u` - Adds path to the current user instead of the Local PC. Does not require Administrator. If this flag is not specified, then Administrator is required.
`-r` - [NYI] Removes the path from the PATH environment variable

## Installation
Download the latest Set-P binary for your architecture (32-bit or x64) at [releases](https://github.com/m-doescode/set-p/releases)

## Building from source

### Prerequisites
1. You must have MYS2 MinGW-w64 installed. You may install it from [msys2.org/#installation](https://www.msys2.org/#installation)
2. You must add `C:\msys64\mingw64\bin` to path, or wherever your MSYS2 installation is located. (I know, ironic, right?)
3. You must have the following packages installed: (You can install a package by using pacman -S <package_name>:
	- make
	- mingw-w64-x86_64-clang
	- lld
(Protip: You can also use `pacman -S make mingw-w64-x86_64-clang lld`)

### Building
1. Open the downloaded source
2. Run `make` (You may add `-r` or `-j16` to increase speed)
3. Once it is finished, go to the build directory
4. Run `setp.exe` or add the folder to path

### Found any bugs?
Report them to me! Open a new [issue](https://github.com/m-doescode/set-p/issues) or create a new thread at [discussions](https://github.com/m-doescode/set-p/discussions)
