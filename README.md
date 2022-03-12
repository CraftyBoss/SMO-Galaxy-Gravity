# Starlight — SMO Galaxy Gravity

This repo contains the beginnings of a project that will aim to modify the games gravity system to include new gravity areas that can be used in galaxy-themed stages. 

## Prerequisites

- [devkitPro](https://devkitpro.org/) 
- Python 3
- The [Keystone-Engine](https://www.keystone-engine.org/) Python Module

## Configuration and Setup

Before Building, you should make sure that the default things for the project is properly setup. 

For the TCP Logger, you can define `GLOBALDEBUGIP` to set the IP address used for connecting the socket to a server hosted on your local machine. (Note: the port can be easily changed in logger.cpp, its default value is 3080.)

In the makefile, you can set `IP`, `USER`, and `PASS` to their respective values in order to automatically send builds to an FTP server hosted on a modded switch. Then, whenever you run `make send` you'll build the project and immediately send to the switch.

## Prerequisites

- [devkitPro](https://devkitpro.org/) 
- Python 3
- The [Keystone-Engine](https://www.keystone-engine.org/) Python Module

## Building

Build has only been tested on WSL2 running Ubuntu 20.04.1.

Just run:
```
DEVKITPRO={path_to_devkitpro} make
```

On Ubuntu (and other Debian-based systems), devkitPro will be installed to `/opt/devkitpro` by default:

```
DEVKITPRO=/opt/devkitpro/ make
```

## Installing (Atmosphère)

After a successful build, simply transfer the `atmosphere` folder located inside `starlight_patch_100` to the root of your switch's SD card.

---

# Credits
- [OdysseyDecomp](https://github.com/MonsterDruide1/OdysseyDecomp) nerve utility macros
- [OdysseyReversed](https://github.com/shibbo/OdysseyReversed) original decomp repo
- [open-ead](https://github.com/open-ead/sead) sead Headers
- [Bryce](https://github.com/brycewithfiveunderscores/Starlight-SMO-Example/) Original Starlight fork for SMO
- [Mars2030](http://github.com/Mars2032) Created gravity area system

# Starlight (Original README)
An enviroment for linking to Splatoon 2 executable and implementing hooks.

# Starlight Contributors
- [3096](https://github.com/3096)
- [khang06](https://github.com/khang06)
- [OatmealDome](https://github.com/OatmealDome)
- [Random0666](https://github.com/random0666)
- [shadowninja108](https://github.com/shadowninja108)
- [shibbo](https://github.com/shibbo) - Repo based on their work on OdysseyReversed
- [Thog](https://github.com/Thog) - Expertise in how rtld is implemented

# Starlight Credits
- devkitA64
- libnx - switch build rules
