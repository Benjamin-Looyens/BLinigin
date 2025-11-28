# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "glm")

# nlohmann
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE "nlohmann/include/single_include")

# ImGui
#add_library(imgui INTERFACE)
#target_include_directories(imgui INTERFACE imgui)

# SDL2
add_library(SDL2 INTERFACE)
target_include_directories(SDL2 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include")

target_link_libraries(SDL2 INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/${TARGET_ARCHITECTURE}/SDL2.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/${TARGET_ARCHITECTURE}/SDL2main.lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/${TARGET_ARCHITECTURE}/SDL2test.lib"
)

add_library(SDL2_image INTERFACE)
target_include_directories(SDL2_image INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/include")

target_link_libraries(SDL2_image INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_image/lib/${TARGET_ARCHITECTURE}/SDL2_image.lib"
)

add_library(SDL2_mixer INTERFACE)
target_include_directories(SDL2_mixer INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer/include")

target_link_libraries(SDL2_mixer INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer/lib/${TARGET_ARCHITECTURE}/SDL2_mixer.lib"
)

add_library(SDL2_ttf INTERFACE)
target_include_directories(SDL2_ttf INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/include")

target_link_libraries(SDL2_ttf INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf/lib/${TARGET_ARCHITECTURE}/SDL2_ttf.lib"
)


# Steamworks
#add_library(steamworks INTERFACE)
#target_include_directories(steamworks INTERFACE steamworks/public)