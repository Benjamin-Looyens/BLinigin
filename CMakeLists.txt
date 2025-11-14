cmake_minimum_required(VERSION 3.27)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add engine subdirectory
add_subdirectory("BLinigin")

# Add project subdirectories
add_subdirectory("Galaga")