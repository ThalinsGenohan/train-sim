# CMakeList.txt : CMake project for train-sim, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)

# Add source to this project's executable.
add_executable (train-sim "src/train-sim.cpp" "include/Tile.h" "src/Tile.cpp" "include/Block.h" "src/Block.cpp" "include/Chunk.h" "src/Chunk.cpp" "include/World.h" "src/World.cpp" "include/Game.h" "src/Game.cpp" "include/Options.h" "include/Input.h" "src/Input.cpp" "src/pch.cpp")
target_include_directories(train-sim PUBLIC "include")
target_precompile_headers(train-sim PUBLIC "include/pch.h")

# TODO: Add tests and install targets if needed.
set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
target_link_options(train-sim PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

set(CMAKE_PREFIX_PATH "lib")
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif (WIN32)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(train-sim sfml-graphics sfml-system -static)

add_custom_command(
   TARGET train-sim POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
           ${CMAKE_CURRENT_SOURCE_DIR}/assets
           ${CMAKE_CURRENT_BINARY_DIR}/assets
)
