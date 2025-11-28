# 3rdParty libraries
add_subdirectory("3rdParty")

# Generate library
add_library(BLiniginLib STATIC 
    BLinigin.cpp 
    
    GameObject.cpp
    
    CollisionComponents.cpp
    ObserverComponents.cpp
    TextureComponents.cpp
    
    Texture2D.cpp
    Gamepad.cpp
    Font.cpp
    Timer.cpp
    Transform.cpp
    
    Scene.cpp
    SceneManager.cpp
    InputManager.cpp
    ResourceManager.cpp
    ServiceLocator.cpp
    SoundSystem_Impl.cpp
    Renderer.cpp
)
set_property(TARGET BLiniginLib PROPERTY CXX_STANDARD 20)

# Make sure everything in this directory is included
target_include_directories(BLiniginLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}") 

# Link 3rdParty
target_link_libraries(BLiniginLib PUBLIC
    glm
    nlohmann_json
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
)

# DLL Copying Function (NEEDS TO BE CALLED WITHIN THE EXECUTABLE PROJECT)
function(BLINIGIN_COPY_OUTPUT_DLLS target)
    add_custom_command(TARGET ${target} POST_BUILD
        # Create the output directory if necessary
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target}>

        # Copy SDL2 dlls
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/BLinigin/3rdParty/SDL2/lib/${TARGET_ARCHITECTURE}/SDL2.dll"
            "$<TARGET_FILE_DIR:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/BLinigin/3rdParty/SDL2_image/lib/${TARGET_ARCHITECTURE}/SDL2_image.dll"
            "$<TARGET_FILE_DIR:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/BLinigin/3rdParty/SDL2_mixer/lib/${TARGET_ARCHITECTURE}/SDL2_mixer.dll"
            "$<TARGET_FILE_DIR:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/BLinigin/3rdParty/SDL2_ttf/lib/${TARGET_ARCHITECTURE}/SDL2_ttf.dll"
            "$<TARGET_FILE_DIR:${target}>"
    )
endfunction()

# Data (Textures, Fonts, Saves, etc) Copying Function (NEEDS TO BE CALLED WITHIN THE EXECUTABLE PROJECT)
function(BLINIGIN_COPY_DATA target)
    add_custom_command(TARGET ${target} POST_BUILD
        # Create the output directory if necessary
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/Data"

        # Copy entire folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Data"
            "$<TARGET_FILE_DIR:${target}>/../Data"
    )
endfunction()
