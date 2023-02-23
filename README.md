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

In light of (not so recent) events, I have decided to add a hardened verision of this app and set it as the default build target.
It sacrifices performance slightly, but improves security.
Some time ago, CVE-2023-22809 was found is sudoedit and promptly patched. By my testing, rdoedit was not vulterable to this exploit, but I have decided to take some precautions. I have added the hardened option, which is compiled by default. It does not check the $EDITOR envvar and instead only uses the EDITOR macro defined in rdo.hardened.h. It also pseudo-randomly generates temorary filenames instead of using the same name, which could be vulnerable.

### Installation

You can clone and build rdoedit with the following set of commands (as root if necessary):

```sh
git clone https://github.com/stefan11111/rdoedit.git
cd rdoedit
make
make install
```

This is how you build the unhardened version of this app:

```sh
git clone https://github.com/stefan11111/rdoedit.git
cd rdoedit
make unhardened
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
If you do rename the binary, using rdo -e will no longer work.
This isn't a concern if you weren't using rdo anyway.
By default it is installed in /usr/local/bin.

### Usage

```sh
rdoedit [files]
```
