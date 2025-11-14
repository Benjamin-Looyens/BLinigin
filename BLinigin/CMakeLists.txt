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