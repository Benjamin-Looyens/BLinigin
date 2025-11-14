# Generate library
add_library(BLiniginLib STATIC "BLinigin.cpp")
set_property(TARGET BLiniginLib PROPERTY CXX_STANDARD 20)

# Make sure everything in this directory is included
target_include_directories(BLiniginLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}") 