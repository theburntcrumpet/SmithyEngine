# Minimum CMake version required. 3.16+ needed for better find_package support
# and modern CMake features like target-based commands.
cmake_minimum_required(VERSION 3.16)

# Define the project name, version, and language.
project(Smithy VERSION 1.0.0 LANGUAGES CXX)

# Use C++17 standard. REQUIRED means it will fail if the compiler doesn't support it.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json in the build directory.
# This is used by IDEs and tools like clangd for code intelligence.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 on the system. REQUIRED means CMake will error if not found.
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# ============================================================================
# Smithy Engine (header-only library)
# ============================================================================

# Collect all engine headers
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    engine/*.hpp
    engine/*.h
)

# Create an INTERFACE library (header-only)
add_library(smithy INTERFACE)

# Engine include path - consumers include as "engine/Entity.hpp" etc.
target_include_directories(smithy INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
)

# Engine dependencies
target_link_libraries(smithy INTERFACE
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
)

# ============================================================================
# Examples and Tests (only when building Smithy directly)
# ============================================================================

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # Control where built files end up
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    # ========================================================================
    # Examples
    # ========================================================================

    # Basic Game Example
    file(GLOB_RECURSE BASIC_GAME_SOURCES CONFIGURE_DEPENDS
        examples/basic_game/*.cpp
    )
    file(GLOB_RECURSE BASIC_GAME_HEADERS CONFIGURE_DEPENDS
        examples/basic_game/*.hpp
        examples/basic_game/*.h
    )

    add_executable(basic_game ${BASIC_GAME_SOURCES} ${BASIC_GAME_HEADERS})

    target_include_directories(basic_game PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/basic_game
    )

    target_link_libraries(basic_game PRIVATE smithy)

    # Platform-specific configuration for Windows builds
    if(WIN32)
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(basic_game PROPERTIES WIN32_EXECUTABLE TRUE)
        endif()

        add_custom_command(TARGET basic_game POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:basic_game>
        )
    endif()

    # Enable compiler warnings
    if(MSVC)
        target_compile_options(basic_game PRIVATE /W4)
    else()
        target_compile_options(basic_game PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    # Pong Example
    file(GLOB_RECURSE PONG_SOURCES CONFIGURE_DEPENDS
        examples/pong/*.cpp
    )
    file(GLOB_RECURSE PONG_HEADERS CONFIGURE_DEPENDS
        examples/pong/*.hpp
        examples/pong/*.h
    )

    add_executable(pong ${PONG_SOURCES} ${PONG_HEADERS})

    target_include_directories(pong PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/pong
    )

    target_link_libraries(pong PRIVATE smithy)

    if(MSVC)
        target_compile_options(pong PRIVATE /W4)
    else()
        target_compile_options(pong PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    # ========================================================================
    # Testing with Catch2
    # ========================================================================

    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
    )

    FetchContent_MakeAvailable(Catch2)

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        tests/*.cpp
    )

    if(TEST_SOURCES)
        add_executable(smithy_tests ${TEST_SOURCES})

        target_link_libraries(smithy_tests PRIVATE
            smithy
            Catch2::Catch2WithMain
        )

        if(MSVC)
            target_compile_options(smithy_tests PRIVATE /W4)
        else()
            target_compile_options(smithy_tests PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        include(CTest)
        include(Catch)
        catch_discover_tests(smithy_tests)
    endif()
endif()
