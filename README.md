# Hmm
Secret WIP (Written in C++). If you know what this is, please don't post about it anywhere! There's a lot I'd still like to do before I share it: It's on Github for version control, and the final version will remain open-source.

# Dependencies
 * devkitARM, which can be installed following the instructions [here](https://devkitpro.org/wiki/Getting_Started).
 * A recent build of [makerom](https://github.com/profi200/Project_CTR) and the latest release of [bannertool](https://github.com/Steveice10/bannertool). These must be added to your PATH.  

# Building
First of all, make sure devkitARM is properly installed - `$DEVKITPRO` and `$DEVKITARM` should be set to `/opt/devkitpro` and `$DEVKITPRO/devkitARM`, respectively.  
After that, open the directory you want to clone the repo into, and execute  
`git clone https://github.com/astronautlevel2/Anemone3DS` (or any other cloning method).  
To install the prerequisite libraries, begin by ensuring devkitPro pacman (and the base install group, `3ds-dev`) is installed, and then install the dkP packages `3ds-jansson`, `3ds-libvorbisidec`, `3ds-libpng`, and `3ds-libarchive` using `[sudo] [dkp-]pacman -S <package-name>`.  

After adding [makerom](https://github.com/profi200/Project_CTR) and [bannertool](https://github.com/Steveice10/buildtools) to your PATH, just enter your directory and run `make`. All built binaries will be in `/out/`.

# Credits
* [Anonymous](https://github.com/MegaPramuk/HCL-3DS) - Created the basis for Portable Homebrew Library (PHL), which I've separated from the rest of the project and adapted for C++
* [astronautlevel2](https://github.com/astronautlevel2) - Makefile, general coding advice, support
* [Carl Birch](https://github.com/carlbirch) - I heavily referenced the coding structure; Entity Component System (ECS) class
* [Frozen Chen](https://github.com/FrozenChen) - Blank logo.bin (3ds)
* [GR3Project](http://nigoro.jp/en/) - Original (2005); [Remake; Sequel](http://la-mulana.com/)
* [Aeon Genesis](http://agtp.romhack.net/project.php?id=lamulana) - Original English Translation (2007)
* [jyosh1](https://github.com/jyosh1) - General coding advice
* [jyosh1](https://github.com/jyosh1) - General coding advice
* [thedax](https://github.com/thedax) - General coding advice
* [Vitor Vilela](https://github.com/VitorVilela7) - General coding advice