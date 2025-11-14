# Create the executable project
add_executable(Galaga 
    Galaga.cpp
    
    Dart.cpp
    DartLogic.cpp
    
    DerivedCommands.cpp
    DerivedComponents.cpp
    
    Enemy.cpp
    EnemyComponents.cpp
    EnemyDeath.cpp
    EnemyState.cpp

    GameplayCommands.cpp
    ScoreTracker.cpp
    HighScore.cpp
    Results.cpp

    MainMenu.cpp
    MainMenuState.cpp

    Player.cpp
    PlayerComponents.cpp
    
    StageComponents.cpp
    StageManager.cpp
)

# Add the library (without exposing anything private)
target_link_libraries(Galaga PRIVATE BLiniginLib) 
install(TARGETS Galaga DESTINATION bin)