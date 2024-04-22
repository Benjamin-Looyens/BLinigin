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
#include "DerivedObservers.h"
#include "Player.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// Raw Object Initializations
	auto pBackgroundObject = scene.AddGameObject();
	pBackgroundObject->AddComponent<dae::TextureComponent>("background.tga");
	
	auto pLogoObject = scene.AddGameObject();
	pLogoObject->AddComponent<dae::TextureComponent>("logo.tga");
	pLogoObject->SetLocalTranslation(216, 180);
	pLogoObject->AddComponent<dae::WaveMovementYComponent>(50.0f, 2.5f);

	auto pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto pTextObject = scene.AddGameObject();
	pTextObject->AddComponent<dae::TextComponent>("Programming 4 Assignment", pFont);
	pTextObject->SetLocalTranslation(80, 20);

	auto pSmallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	auto pInstructions1 = scene.AddGameObject();
	auto pInstructions2 = scene.AddGameObject();
	auto pInstructions3 = scene.AddGameObject();
	pInstructions1->AddChild(pInstructions2);
	pInstructions2->AddChild(pInstructions3);
	pInstructions1->AddComponent<dae::TextComponent>("Gamepad A / Keyboard E -> Big score", pSmallFont);
	pInstructions2->AddComponent<dae::TextComponent>("Gamepad B / Keyboard Q -> Small score", pSmallFont);
	pInstructions3->AddComponent<dae::TextComponent>("Gamepad X / Keyboard Space -> Lose Life", pSmallFont);
	pInstructions1->SetLocalTranslation(10, 300);
	pInstructions2->SetLocalTranslation(0, 30);
	pInstructions3->SetLocalTranslation(0, 30);

	// Prefabs
	dae::Player playerOne{ glm::vec3{ 30, 420, 0 }, glm::vec3{ 10, 130, 0 }, glm::vec3{ 10, 160, 0 }, "GalagaP1.tga", 0, 50.0f, 5, true };
	playerOne.AddToScene(&scene);

	dae::Player playerTwo{ glm::vec3{ 60, 420, 0 }, glm::vec3{ 10, 200, 0 }, glm::vec3{ 10, 230, 0 }, "GalagaP2.tga", 1, 50.0f, 5, false };
	playerTwo.AddToScene(&scene);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}