# Smithy Engine

Originally started as a cross-platform 2D game skeleton using SDL2 and CMake, this has grown into a barebones, header only SDL2 engine.

## Prerequisites

### Linux (Arch-based)

```bash
# Development tools
sudo pacman -S base-devel cmake

# SDL2 and SDL2_image for native Linux builds
sudo pacman -S sdl2 sdl2_image

# For cross-compiling to Windows
sudo pacman -S mingw-w64-gcc mingw-w64-sdl2 mingw-w64-sdl2_image
```

### Linux (Debian/Ubuntu)

```bash
# Development tools
sudo apt install build-essential cmake

# SDL2 and SDL2_image for native Linux builds
sudo apt install libsdl2-dev libsdl2-image-dev

# For cross-compiling to Windows
sudo apt install mingw-w64
# Note: You'll need to manually download SDL2 and SDL2_image development libraries for MinGW
# from https://github.com/libsdl-org/SDL/releases
# and https://github.com/libsdl-org/SDL_image/releases
```

## Building

### Linux (Native)

```bash
# Create build directory and generate build files
mkdir build && cd build
cmake ..

# Compile
make

# Run the game
./bin/MyGame
```

For faster builds, use Ninja instead of Make:

```bash
sudo pacman -S ninja  # or: sudo apt install ninja-build
mkdir build && cd build
cmake -G Ninja ..
ninja
```

### Windows (Cross-compile from Linux)

```bash
# Create a separate build directory for Windows
mkdir build-win && cd build-win

# Configure with the MinGW toolchain file
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-mingw64.cmake ..

# Compile
make

# Copy DLLs next to the executable
cp /usr/x86_64-w64-mingw32/bin/SDL2.dll bin/
cp /usr/x86_64-w64-mingw32/bin/SDL2_image.dll bin/
```

The Windows executable will be at `build-win/bin/MyGame.exe`.

To test on Linux with Wine:

```bash
wine bin/MyGame.exe
```

### Build Types

By default, CMake builds in Debug mode. For optimized Release builds:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Project Structure

```
.
├── CMakeLists.txt              # Main build configuration
├── cmake/
│   └── toolchain-mingw64.cmake # Cross-compilation settings for Windows
└── src/
    ├── main.cpp                # Entry point
    ├── Game.hpp                # Game class header
    └── Game.cpp                # Game loop, input handling, rendering
```

Add new `.cpp` and `.hpp` files anywhere under `src/` and they'll be automatically included in the build.

## Controls

- **WASD** or **Arrow keys**: Move
- **Escape**: Quit

## Next Steps

Common SDL2 extension libraries to add:

- **SDL2_mixer**: Play sound effects and music
- **SDL2_ttf**: Render text with TrueType fonts

To add these, install them (`sdl2_mixer`, `sdl2_ttf` packages) and add to CMakeLists.txt:

```cmake
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
```
