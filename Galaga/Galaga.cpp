#include <SDL.h> // For some reason has linker error when not included ?

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "DerivedComponents.h"
#include "Timer.h"
#include "InputManager.h"
#include "DerivedCommands.h"
#include "Defines.h"
#include "Player.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "Enemy.h"
#include "PlayerComponents.h"
#include "StageManager.h"

#include "TEMP_PlayerGlobal.h"

void load()
{
	int screenWidth;
	int screenHeight;
	SDL_GetWindowSize(SDL_GetKeyboardFocus(), &screenWidth, &screenHeight); // HACK : **NEED TO FIND ALTERNATIVE** Window size may not be made if the window is not in focus currently

	auto& scene = BL_SCENE_MANAGER.CreateScene("Demo");

	// Raw Object Initializations
	auto pBackgroundObject = scene.AddGameObject();
	pBackgroundObject->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	
	auto pBackgroundTileR = scene.AddGameObject(); 
	auto pTextureCompR = pBackgroundTileR->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileR->SetLocalTranslation(pTextureCompR->GetTextureWidth(), 0.0f);
	
	auto pBackgroundTileB = scene.AddGameObject();
	pBackgroundTileB->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileB->SetLocalTranslation(0.0f, pTextureCompR->GetTextureHeight());

	auto pBackgroundTileBR = scene.AddGameObject();
	pBackgroundTileBR->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileBR->SetLocalTranslation(pTextureCompR->GetTextureWidth(), pTextureCompR->GetTextureHeight());

	pBackgroundObject->AddChild(pBackgroundTileR);
	pBackgroundObject->AddChild(pBackgroundTileB);
	pBackgroundObject->AddChild(pBackgroundTileBR);

	// Prefabs
	dae::Player playerOne{glm::vec2{ 30, 420 }, glm::vec2{ 10, 10 }, glm::vec2{ 10, 160 }, "GalagaP1.tga", 0, 100.0f, 5, true };
	auto pPlayerOneObject = playerOne.AddToScene(&scene);

	auto textureSize = pPlayerOneObject->GetComponent<dae::TextureComponent>()->GetTextureSize(); // NOTE : Temporary setup
	dae::Player playerTwo{ glm::vec2{ 90, 420 }, glm::vec2{ screenWidth - 10 - textureSize.x, 10 }, glm::vec2{ 10, 230 }, "GalagaP2.tga", 1, 100.0f, 5, false };
	auto pPlayerTwoObject = playerTwo.AddToScene(&scene);

	// Sound
	auto& soundSys = dae::ServiceLocator::GetSoundSystem();

	soundSys.LinkIDToFilePath(1,  "../Data/Sound/01. Stage Intro.mp3");
	soundSys.LinkIDToFilePath(2,  "../Data/Sound/02. Fighter Captured.mp3");
	soundSys.LinkIDToFilePath(3,  "../Data/Sound/03. Fighter Rescued.mp3");
	soundSys.LinkIDToFilePath(4,  "../Data/Sound/04. Captured Fighter Destroyed.mp3");
	soundSys.LinkIDToFilePath(5,  "../Data/Sound/05. Challenging Stage.mp3");
	soundSys.LinkIDToFilePath(6,  "../Data/Sound/06. Challenging Stage Over.mp3");
	soundSys.LinkIDToFilePath(7,  "../Data/Sound/07. Challenging Stage Perfect.mp3");
	soundSys.LinkIDToFilePath(8,  "../Data/Sound/08. 1-Up.mp3");
	soundSys.LinkIDToFilePath(9,  "../Data/Sound/09. Die-Start Up Sound.mp3");
	soundSys.LinkIDToFilePath(10, "../Data/Sound/10. Coin.mp3");
	soundSys.LinkIDToFilePath(11, "../Data/Sound/11. Name Entry.mp3");
	soundSys.LinkIDToFilePath(12, "../Data/Sound/12. 2nd - 5th Name Entry.mp3");
	soundSys.LinkIDToFilePath(13, "../Data/Sound/13. Shoot.mp3");

	soundSys.PlaySound(1, 0.2f);

	std::vector<dae::IObserver*> enemyObservers;
	//enemyObservers.push_back(pPlayerOneObject->GetComponent<dae::ScoreComponent>());
	//enemyObservers.push_back(pPlayerTwoObject->GetComponent<dae::ScoreComponent>());

	dae::StageManager stageManager{ enemyObservers };
	stageManager.AddToScene(&scene);

	// TEMP
	dae::PlayerLocator::RegisterPlayer(pPlayerOneObject);
	dae::PlayerLocator::RegisterPlayer(pPlayerTwoObject);

	pPlayerOneObject->PrintComponents();
}

// GENERAL TODOS
// TODO : Fix shared ptrs everywhere
// TODO : Improve Player/Controller Setup
// TODO : Check every class to make sure if applies rule of 5 correctly
// TODO : Add layering options which guarantees that some Objects/Components are rendered above others
// TODO : Reduce auto usage

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}