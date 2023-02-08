# Rdoedit (RootDO edit) 
Minimal alternative to sudoedit

This project aims to be a minimal alternative to sudoedit.
Sudoedit by itself is over 700 lines of code by itself, not including all the header files or the fact that it's part of sudo.
There is no reason why such a simple app should be so bloated of come bundled with bloat like sudo.
This readme is likely more verbose that my implementation. 

This app allows you to edit files as root while using your config files for your editor(.nanorc, .vimrc, etc).

Any configuration is made by editing the source code.
The configuration file is rdoedit.h, but any desired features can be patched into rdoedit.c.
After editing rdoedit.h the program must be recompiled.
Doing things this way keeps rdoedit fast, secure and simple.
rdoedit.h is well commented and the code base is very small, so the source code is the documentation.

If you want to allow multiple users or groups to use the app either add the feature yourself or use sudoedit.
If you do add some features, make a pull request. I might merge it.

This app is very minimal. The base app is under 200 lines of code. The noconfig option reduces that to under 100 lines of code.

### Installation

You can clone and build rdoedit with the following set of commands (as root if necessary):

```sh
git clone https://github.com/stefan11111/rdoedit.git
cd rdoedit
make
make install
```
If you just want to edit a file as root using your config files(allow all users to use rdo with no password) you can do the following (as root if necessary) :
 
```sh
git clone https://github.com/stefan11111/rdoedit.git
cd rdoedit
make noconfig
make install
```
Then you're good to go!

To uninstall (run as root if necessary):
```sh
make uninstall
```

For convenience, you can alias rdoedit to rdo -e, sudo -e or anything else in your .bashrc or just rename the binary.
By default it is installed in /usr/local/bin.

### Usage

```sh
rdoedit [files]
```
