# Operating System

A command-line operating system based on [Xv6](https://pdos.csail.mit.edu/6.828/2018/xv6.html 'https://pdos.csail.mit.edu/6.828/2018/xv6.html'). The base of the system can be found in the `src/base` directory.

The project was a part of Systems Programming module.

## Features

Each stage was built on top of the previous one. Therefore, a gradual improvement can be seen. Code for each of the stages can be found on the `separate-stages` branch.

### Stage 1

Implementation of [cd](https://ss64.com/bash/cd.html 'https://ss64.com/bash/cd.html')

- `cd` – go to the root directory,
- `cd ..` – go to the directory above,
- `cd [dir]` – go to the specified directory/path.

### Stage 2

Implementation of [ls](https://ss64.com/bash/ls.html 'https://ss64.com/bash/ls.html')

- `ls` – list files and directories in the current working directory (cwd),
- `ls [dir]` – list files and directories from the specified path.

### Stage 3

Implementation of `-l` flag for `ls`

- `ls -l` – list files and directories in the cwd and their details,
- `ls [dir] -l` – list files and directories with details from the specified path.

### Stage 4

Enabling `ls` to work in the root directory.

![Operating system screenshot](https://github.com/amrwc/OS/blob/master/assets/OS-ls.png)

## Setup

### Windows

1. Install [Cygwin](https://cygwin.com/install.html 'https://cygwin.com/install.html').

   When you get to the 'Select Packages' screen, click on the small icon to the right of 'Devel'. The 'txst' to the right of it should change to 'Install' instead of 'Default'. You can now continue with the rest of the installation process, accepting the rest of the defaults.

   Note that Cygwin will take a long time to install, possibly well over an hour, since it pulls all of the tools down over the Internet.

2. Install [ImDisk](http://www.ltr-data.se/opencode.html/#ImDisk 'http://www.ltr-data.se/opencode.html/#ImDisk').
3. Install [Bochs 2.6.9](https://sourceforge.net/projects/bochs/files/bochs/2.6.9 'https://sourceforge.net/projects/bochs/files/bochs/2.6.9').
4. In Cygwin, navigate to the project directory and compile with `make`.

   To access a drive with its letter, use `cd /cygdrive/[drive-letter]`.

5. In Bochs, load the `bochsrc.bxrc` file and start the virtual machine.

   `bochsrc.bxrc` is essentially a configuration for the virtual machine, which also points to the correct disk image.
