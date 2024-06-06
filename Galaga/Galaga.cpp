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
#include "TextureComponents.h"
#include "Renderer.h"
#include "ScoreTracker.h"

#include "TEMP_PlayerGlobal.h"

void InitTitle(dae::Scene& titleScene, glm::ivec2& windowSize);
void InitSolo(dae::Scene& soloScene, glm::ivec2& windowSize);
void InitVersus(dae::Scene& versusScene, glm::ivec2& windowSize);
void InitCoop(dae::Scene& coopScene, glm::ivec2& windowSize);
void InitResults(dae::Scene& resultsScene, glm::ivec2& windowSize);
void InitSound();

void load()
{
	glm::ivec2 windowSize{ BL_RENDERER.GetWindowSize() };
	
	auto& title		= BL_SCENE_MANAGER.CreateScene("Title");
	auto& solo		= BL_SCENE_MANAGER.CreateScene("SoloGalaga");
	auto& versus	= BL_SCENE_MANAGER.CreateScene("VersusGalaga");
	auto& coop		= BL_SCENE_MANAGER.CreateScene("CoopGalaga");
	auto& results	= BL_SCENE_MANAGER.CreateScene("Results");
	
	InitSound();
	InitTitle(title, windowSize);
	InitSolo(solo, windowSize);
	InitCoop(coop, windowSize);
	InitVersus(versus, windowSize);
	InitResults(results, windowSize);

	//BL_SCENE_MANAGER.SetActiveScene("Title");
	BL_SCENE_MANAGER.SetActiveScene("CoopGalaga");
}

// GENERAL TODOS
// TODO : Improve Player/Controller Setup
// TODO : Check every class to make sure if applies rule of 5 correctly
// TODO : (Maybe) Add layering options which guarantees that some Objects/Components are rendered above others
// TODO : Reduce auto usage
// TODO : (Maybe) Write explanations/summaries for functions
// TODO : Check where getters and setters are necessary
// TODO : Add debug info for when a file is missing 

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}

void InitTitle(dae::Scene& titleScene, glm::ivec2& windowSize)
{
	titleScene;
	windowSize;
}

void InitSolo(dae::Scene& soloScene, glm::ivec2& windowSize)
{
	soloScene;
	windowSize;
}

void InitVersus(dae::Scene& versusScene, glm::ivec2& windowSize)
{
	versusScene;
	windowSize;
}

void InitCoop(dae::Scene& coopScene, glm::ivec2& windowSize)
{
	// Raw Object Initializations
	auto pBackgroundObject = coopScene.AddGameObject();
	pBackgroundObject->AddComponent<dae::TextureComponent>("GalagaBackground.png");

	auto pBackgroundTileR = coopScene.AddGameObject();
	auto pTextureCompR = pBackgroundTileR->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileR->SetLocalTranslation(pTextureCompR->GetTextureWidth(), 0.0f);

	auto pBackgroundTileB = coopScene.AddGameObject();
	pBackgroundTileB->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileB->SetLocalTranslation(0.0f, pTextureCompR->GetTextureHeight());

	auto pBackgroundTileBR = coopScene.AddGameObject();
	pBackgroundTileBR->AddComponent<dae::TextureComponent>("GalagaBackground.png");
	pBackgroundTileBR->SetLocalTranslation(pTextureCompR->GetTextureWidth(), pTextureCompR->GetTextureHeight());

	// TODO : Make better background tiling system
	pBackgroundObject->AddChild(pBackgroundTileR);
	pBackgroundObject->AddChild(pBackgroundTileB);
	pBackgroundObject->AddChild(pBackgroundTileBR);

	// Prefabs
	dae::GameObject* pPlayerOneObject = nullptr;
	dae::GameObject* pPlayerTwoObject = nullptr;

	float playerElevation{ static_cast<float>(windowSize.y - 100) };
	//float midScreen{ static_cast<float>(windowSize.x / 2) };
	constexpr glm::vec2 scoreInset{ 100, 35 };

	dae::Player playerOne{	glm::vec2{ 150, playerElevation },			// Player pos
							glm::vec2{ 10, windowSize.y - 26 },			// Lives counter pos
							glm::vec2{ scoreInset.x, scoreInset.y },	// Score counter pos
							"GalagaP1.tga",								// Sprite
							0, 100.0f, 3, dae::InputType::KEYBOARD };	// Index, speed, lives, input type

	pPlayerOneObject = playerOne.AddToScene(&coopScene);

	auto textureSize = pPlayerOneObject->GetComponent<dae::TextureComponent>()->GetTextureSize(); // NOTE : Temporary setup
	float p2Offset = (textureSize.x * pPlayerOneObject->GetWorldScale().x);

	dae::Player playerTwo{	glm::vec2{ windowSize.x - 150 - p2Offset, playerElevation },		// Player pos
							glm::vec2{ windowSize.x - 10 - textureSize.x, windowSize.y - 26 },	// Lives counter pos
							glm::vec2{ windowSize.x - scoreInset.x, scoreInset.y },				// Score counter pos
							"GalagaP2.tga",														// Sprite
							1, 100.0f, 3, dae::InputType::GAMEPAD };							// Index, speed, lives, input type

	pPlayerTwoObject = playerTwo.AddToScene(&coopScene);

	std::vector<dae::IObserver*> enemyObservers;
	//enemyObservers.push_back(pPlayerOneObject->GetComponent<dae::ScoreComponent>());
	//enemyObservers.push_back(pPlayerTwoObject->GetComponent<dae::ScoreComponent>());

	dae::StageManager stageManager{ enemyObservers };
	stageManager.AddToScene(&coopScene);

	std::vector<dae::GameObject*> players{ pPlayerOneObject, pPlayerTwoObject };

	auto pScoreTrackerObject = coopScene.AddGameObject();
	auto pScoreTrackerComp = pScoreTrackerObject->AddComponent<dae::ScoreTrackerComponent>(players);
	
	pPlayerOneObject->GetComponent<dae::PlayerLogic>()->AddObserver(pScoreTrackerComp);
	pPlayerTwoObject->GetComponent<dae::PlayerLogic>()->AddObserver(pScoreTrackerComp);
	pPlayerOneObject->GetComponent<dae::LivesComponent>()->AddObserver(pScoreTrackerComp);
	pPlayerTwoObject->GetComponent<dae::LivesComponent>()->AddObserver(pScoreTrackerComp);

	// TEMP
	dae::PlayerLocator::RegisterPlayer(pPlayerOneObject);
	dae::PlayerLocator::RegisterPlayer(pPlayerTwoObject);
}

void InitResults(dae::Scene& resultsScene, glm::ivec2& windowSize)
{
	resultsScene;
	windowSize;
}

void InitSound()
{
	// Sound
	auto& soundSys = BL_SOUND;

	soundSys.LinkIDToFilePath(1,  "Sound/01. Stage Intro.mp3");
	soundSys.LinkIDToFilePath(2,  "Sound/02. Fighter Captured.mp3");
	soundSys.LinkIDToFilePath(3,  "Sound/03. Fighter Rescued.mp3");
	soundSys.LinkIDToFilePath(4,  "Sound/04. Captured Fighter Destroyed.mp3");
	soundSys.LinkIDToFilePath(5,  "Sound/05. Challenging Stage.mp3");
	soundSys.LinkIDToFilePath(6,  "Sound/06. Challenging Stage Over.mp3");
	soundSys.LinkIDToFilePath(7,  "Sound/07. Challenging Stage Perfect.mp3");
	soundSys.LinkIDToFilePath(8,  "Sound/08. 1-Up.mp3");
	soundSys.LinkIDToFilePath(9,  "Sound/09. Die-Start Up Sound.mp3");
	soundSys.LinkIDToFilePath(10, "Sound/10. Coin.mp3");
	soundSys.LinkIDToFilePath(11, "Sound/11. Name Entry.mp3");
	soundSys.LinkIDToFilePath(12, "Sound/12. 2nd - 5th Name Entry.mp3");
	soundSys.LinkIDToFilePath(13, "Sound/13. Shoot.mp3");
	soundSys.LinkIDToFilePath(14, "Sound/BossDamage.mp3");
	soundSys.LinkIDToFilePath(15, "Sound/BossDeath.mp3");
	soundSys.LinkIDToFilePath(16, "Sound/BeeDeath.mp3");
	soundSys.LinkIDToFilePath(17, "Sound/ButterflyDeath.mp3");
}
