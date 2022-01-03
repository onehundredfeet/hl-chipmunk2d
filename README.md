# hl-chipmunk2d

THIS PROJECT IS IN PROGRESS - DO NOT CLONE, IT DOES NOT WORK


## Build and Install
##### Note: Most of the following steps are common on windows, linux and mac. The only difference is in the build & install step.

### 1. Prerequisites:
1.1. Install haxe: (: https://haxe.org/download)  
1.2. Install hashlink: (https://hashlink.haxe.org )  
1.3. Install heaps: (For linux: https://heaps.io/documentation/installation.html )  
1.4. Install the modified webidl from git:   
```sh
haxelib git hl-idl https://github.com/onehundredfeet/hl-idl.git
```

### 2. Build
Open new terminal in this directory.  
```sh
haxelib dev hl-chipmunk2d hl-chipmunk2d
```

This tells haxe to look for the library 'hl-chipmunk2d' in the directory 'hl-chipmunk2d'.  The 'dev' keyword tells haxe that the library is local and will be directly referenced instead of being installed to the library cache.

2.2 Clone chipmunk2d c++ sources as a submodule in hl-chipmunk2d
From the hl-chipmunk2d root, run

```sh
git submodule update --init --recursive
```

2.3 Generate the binding cpp file.

Change directory into the hl-chipmunk2d root.

.Open new terminal under and run 
```sh
make genhl
```

This will generate src/chipmunk2d.cpp from chipmunk2d/chipmunk2d.idl


2.4 Generate cmake and build.  
**Note: At this step there are differences between platforms**  
    Create new dir : HL-chipmunk2d-ROOT/build  
    Open new terminal in the directory HL-chipmunk2d-ROOT/build and run:  

**For linux:**  
```sh
    cmake .. #Add -GNinja for ninja support
      -DCHIPMUNK2D_SRC_DIR="ext/chipmunk2d" # DEFAULT "ext/chipmunk2d" This is the /src from step 2
      -DHL_INCLUDE_DIR="<path-to-hashlink headers>" # DEFAULT "/usr/local/include" Path to where hashlink headers (hl.h, ...) are located.
      -DHL_LIB_DIR="<path-to-hashlink>" # DEFAULT "/usr/local/lib" Path to where hashlink binaries (libhl.so, ...) are located. 
      -HDLL_DESTINATION="<final-install-destination>" # DEFAULT "/usr/local/lib" The path of all *hdll binaries, 
```
```sh
    make
```
```sh
    make install
```

**For windows:**  

```sh
    cmake .. -A x64 -G "Visual Studio 15 2017" 
    -DCHIPMUNK2D_SRC_DIR="<path-to-bullet3>/src" # This is the /src from step 2
      -DHL_INCLUDE_DIR="<path-to-hashlink>/include"
      -DHL_LIB_DIR="<path-to-hashlink>"  # Path to where hashlink binaries (libhl.lib, ...) are located
      -HDLL_DESTINATION="final/install/destination" # Usually <path-to-hashlink>
```

* Open and build build/Project.sln  
* Copy ./build/Debug/bullet.hdll to ../path/to/HashLink/hdll/location/ # Just in case Visual Studio install step fails.  

**For UNIX (Mac / Linux):**  
* Note: Specify the hashlink directories on your system.  On Mac, brew defaults /usr/local (lib/include).

Ninja is optional.  You can use make just by removing -GNinja.

```sh
 cmake -GNinja .. -DCHIPMUNK2D_SRC_DIR="ext/chipmunk2d/chipmunk2d" -DHL_INCLUDE_DIR="/usr/local/include" -DHL_LIB_DIR="/usr/local/lib" -DDETOUR_SRC_DIR="ext/chipmunk2d/Detour" -DHDLL_DESTINATION="/usr/local/lib"
```
```sh
    make
```
```sh
    make install
```

### 3. Run samples
```sh
haxe compile.hxml
hl bin/example.hl
```
