cmake_minimum_required(VERSION 3.13)
project(PROJ_PC)

# My default command
# cmake -G "MinGW Makefiles" -S . -B cmake-build

# Check if the operating system is Windows
if(NOT WIN32)
    message(FATAL_ERROR "This project can only be compiled on Windows because it uses a Windows-specific UART library. Please use a Windows system.")
endif()

# Project settings
set(CMAKE_CXX_STANDARD 17)          # Use C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Enforce the use of C++17
set(CMAKE_CXX_EXTENSIONS OFF)       # Disable compiler-specific extensions

# Include the "inc" directory for header files
include_directories(inc)

# Collect all .cpp files in the "src" folder and all .hpp files in the "inc" folder
file(GLOB SRC_FILES src/*.cpp)
file(GLOB INCLUDE_FILES inc/*.hpp)

# Create the executable target
add_executable(JPO_PC ${SRC_FILES} ${INCLUDE_FILES})

# Add compiler options
if(MSVC) # For Visual Studio (Windows)
    target_compile_options(JPO_PC PRIVATE /W4)  # Enable warning level 4
else() # For GCC/Clang (Linux, macOS)
    target_compile_options(JPO_PC PRIVATE -Wall -Wextra -pedantic) # Enable warnings and treat them as errors
endif()