cmake_minimum_required(VERSION 3.27)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project
project(BLinigin_Projects) # TODO : Change reliance so that Blinigin & Galaga CMake's are seperate

# Add engine subdirectory
add_subdirectory("BLinigin")

# Add project subdirectories
add_subdirectory("Galaga")

# Set up constants
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TARGET_ARCHITECTURE x64 CACHE INTERNAL "Target architecture")
else()
    set(TARGET_ARCHITECTURE x86 CACHE INTERNAL "Target architecture")
endif()
