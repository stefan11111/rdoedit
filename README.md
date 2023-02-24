# Rdoedit (RootDO edit) 
Minimal alternative to sudoedit

This project aims to be a minimal alternative to sudoedit.
Sudoedit by itself is over 700 lines of code by itself, not including all the header files or the fact that it's part of sudo.
There is no reason why such a simple app should be so bloated or come bundled with bloat like sudo.
This readme is likely more verbose that my implementation. 

This app allows you to edit files as root while using your config files for your editor(.nanorc, .vimrc, etc).

Any configuration is made by editing the source code.
The configuration file is rdoedit.h, but any desired features can be patched into rdoedit.c.
After editing rdoedit.h the program must be recompiled.
Doing things this way keeps rdoedit fast, secure and simple.
rdoedit.h is well commented and the code base is very small, so the source code is the documentation.

If you want to allow multiple users or groups to use the app either add the feature yourself or use sudoedit.
If you do add some features, make a pull request. I might merge it.

This app is very minimal. It is under 200 lines of code.

In light of (not so recent) events, I have decided to harden this app by default.
It sacrifices performance slightly, but improves security.
Some time ago, CVE-2023-22809 was found in sudoedit and promptly patched.
By my testing, rdoedit was not vulnerable to this exploit, but I have decided to take some precautions.
This app by default no longer checks the $EDITOR envvar and instead only uses the EDITOR macro defined in rdoedit.h.
It also pseudo-randomly generates temorary filenames instead of using a macro-defined name, which could be vulnerable.

### Installation

You can clone and build rdoedit with the following set of commands (as root if necessary):

```sh
git clone https://github.com/stefan11111/rdoedit.git
cd rdoedit
make
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
