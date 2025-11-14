# Create the executable project
add_executable(Galaga "main.cpp")

# Add the library (without exposing anything private)
target_link_libraries(Galaga PRIVATE BLiniginLib) 
install(TARGETS Galaga DESTINATION bin)