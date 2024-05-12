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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

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
	
	//auto pLogoObject = scene.AddGameObject();
	//pLogoObject->AddComponent<dae::TextureComponent>("logo.tga");
	//pLogoObject->SetLocalTranslation(216, 180);
	//pLogoObject->AddComponent<dae::WaveMovementYComponent>(50.0f, 2.5f);
	//
	//auto pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto pTextObject = scene.AddGameObject();
	//pTextObject->AddComponent<dae::TextComponent>("Programming 4 Assignment", pFont);
	//pTextObject->SetLocalTranslation(80, 20);

	// Prefabs
	dae::Player playerOne{ glm::vec3{ 30, 420, 0 }, glm::vec3{ 10, 130, 0 }, glm::vec3{ 10, 160, 0 }, "GalagaP1.tga", 0, 100.0f, 5, true };
	playerOne.AddToScene(&scene);

	dae::Player playerTwo{ glm::vec3{ 90, 420, 0 }, glm::vec3{ 10, 200, 0 }, glm::vec3{ 10, 230, 0 }, "GalagaP2.tga", 1, 100.0f, 5, false };
	playerTwo.AddToScene(&scene);

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

	dae::Enemy test{ glm::vec3{}, "BasicEnemy.tga", 350.0f, 50.0f };
	test.AddToScene(&scene);
}

// GENERAL TODOS
// TODO : Fix shared ptrs everywhere
// TODO : Vec3 to Vec2 (z component is useless in a 2D engine)
// TODO : Improve Player/Controller Setup

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}