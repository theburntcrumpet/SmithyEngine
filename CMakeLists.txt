# Minimum CMake version required. 3.16+ needed for better find_package support
# and modern CMake features like target-based commands.
cmake_minimum_required(VERSION 3.16)

# Define the project name, version, and language.
# PROJECT_NAME variable is set to "MyGame" and can be used throughout.
project(MyGame VERSION 1.0.0 LANGUAGES CXX)

# Use C++17 standard. REQUIRED means it will fail if the compiler doesn't support it.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json in the build directory.
# This is used by IDEs and tools like clangd for code intelligence.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Control where built files end up:
# - Executables go to build/bin/
# - Shared libraries (.so/.dll) go to build/lib/
# - Static libraries (.a/.lib) go to build/lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find SDL2 on the system. REQUIRED means CMake will error if not found.
# This sets variables like SDL2_INCLUDE_DIRS and SDL2_LIBRARIES.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Automatically find all source files under src/.
# GLOB_RECURSE searches subdirectories too (e.g., src/entities/*.cpp).
# CONFIGURE_DEPENDS makes CMake check for new/removed files on each build.
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    src/*.hpp
    src/*.h
)

# Create the executable target from our source files.
# Headers are included so they show up in IDE project views.
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Tell the compiler where to find header files.
# PRIVATE means these paths are only used when building this target,
# not by anything that links against it.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

# Link SDL2 libraries to our executable.
# On Linux this links libSDL2.so, on Windows it links SDL2.lib.
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)

# Platform-specific configuration for Windows builds
if(WIN32)
    # In Release builds, don't show a console window behind the game.
    # WIN32_EXECUTABLE makes it a GUI application instead of a console app.
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

    # Automatically copy SDL2.dll next to the .exe after building.
    # Windows needs DLLs in the same directory as the executable (or in PATH).
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Enable compiler warnings to catch potential bugs.
# MSVC (Visual Studio) uses /W4, GCC/Clang use -Wall -Wextra -Wpedantic.
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================================
# Testing with Catch2
# ============================================================================

# FetchContent allows downloading dependencies at configure time
include(FetchContent)

# Declare Catch2 as a dependency - it will be downloaded automatically
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

# Make Catch2 available (downloads if needed, then adds to build)
FetchContent_MakeAvailable(Catch2)

# Find all test source files
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    tests/*.cpp
)

# Only create test target if we have test files
if(TEST_SOURCES)
    # Create test executable
    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

    # Test executable needs access to our source headers
    target_include_directories(${PROJECT_NAME}_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # Link Catch2 to the test executable
    target_link_libraries(${PROJECT_NAME}_tests PRIVATE Catch2::Catch2WithMain)

    # Apply same warning flags to tests
    if(MSVC)
        target_compile_options(${PROJECT_NAME}_tests PRIVATE /W4)
    else()
        target_compile_options(${PROJECT_NAME}_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    # Enable CTest integration
    include(CTest)
    include(Catch)
    catch_discover_tests(${PROJECT_NAME}_tests)
endif()
