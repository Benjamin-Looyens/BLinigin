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
#include "MainMenu.h"
#include "Results.h"
#include "StageComponents.h"

void InitTitle	(dae::Scene& titleScene,	glm::ivec2& windowSize);
void InitSolo	(dae::Scene& soloScene,		glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT);
void InitVersus	(dae::Scene& versusScene,	glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT);
void InitCoop	(dae::Scene& coopScene,		glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT);
void InitResults(dae::Scene& resultsScene,	glm::ivec2& windowSize, dae::ScoreTrackerComponent* pScoreTracker);
void InitSound();
void AddAllModeSharedObj(dae::Scene& scene, glm::ivec2& windowSize);

void load()
{
	glm::ivec2 windowSize{ BL_RENDERER.GetWindowSize() };
	
	auto& title		= BL_SCENE_MANAGER.CreateScene("Title");
	auto& solo		= BL_SCENE_MANAGER.CreateScene("SoloGalaga");
	auto& versus	= BL_SCENE_MANAGER.CreateScene("VersusGalaga");
	auto& coop		= BL_SCENE_MANAGER.CreateScene("CoopGalaga");

	auto& soloResults	= BL_SCENE_MANAGER.CreateScene("SoloResults");
	auto& coopResults	= BL_SCENE_MANAGER.CreateScene("CoopResults");
	auto& versusResults = BL_SCENE_MANAGER.CreateScene("VersusResults");
	
	InitSound();
	InitTitle(title, windowSize);
	dae::ScoreTrackerComponent* pSoloScoreTracker = nullptr;
	dae::ScoreTrackerComponent* pCoopScoreTracker = nullptr;
	dae::ScoreTrackerComponent* pVersusScoreTracker = nullptr;

	BL_INPUT.AddGamepad();
	
	InitSolo(solo, windowSize, pSoloScoreTracker);
	InitCoop(coop, windowSize, pCoopScoreTracker);
	InitVersus(versus, windowSize, pVersusScoreTracker);

	InitResults(soloResults, windowSize, pSoloScoreTracker);
	InitResults(coopResults, windowSize, pCoopScoreTracker);
	InitResults(versusResults, windowSize, pVersusScoreTracker);

	BL_SCENE_MANAGER.SetActiveScene("Title");
}

// GENERAL TODOS
// TODO : Check every class to make sure if applies rule of 5 correctly
// TODO : (Maybe) Add layering options which guarantees that some Objects/Components are rendered above others
// TODO : Reduce auto usage
// TODO : (Maybe) Write explanations/summaries for functions
// TODO : Check where getters and setters are necessary
// TODO : Add debug info for when a file is missing 
// TODO : Look at kill player command and ensure it is in the active scene

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}

void InitTitle(dae::Scene& titleScene, glm::ivec2& windowSize)
{
	auto pMainMenuObj = titleScene.AddGameObject();
	pMainMenuObj->AddComponent<dae::MainMenuComponent>();

	//TextComponent(dae::GameObject* pParent, const std::string& text, dae::Font* font, const std::string& internalIdentifier, bool isCentered = false, SDL_Color color = { 255, 255, 255, 255 });
	auto pFont = BL_RESOURCE.LoadFont("Emulogic.ttf", 20);
	auto pGalagaTextObj = titleScene.AddGameObject();
	pGalagaTextObj->AddComponent<dae::TextComponent>("GALAGA", pFont, "GalagaTitleText", true, SDL_Color{ 0, 255, 255, 255 });
	pGalagaTextObj->SetLocalTranslation({ windowSize.x / 2, 50 });

	auto pDaeProjectTextObj = titleScene.AddGameObject();
	pDaeProjectTextObj->AddComponent<dae::TextComponent>("DAE GD PROGRAMMING 4", pFont, "DaeProg4Text", true, SDL_Color{ 255, 255, 255, 255 });
	pDaeProjectTextObj->SetLocalTranslation({ windowSize.x / 2, 100 });
}

void InitSolo(dae::Scene& soloScene, glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT)
{
	AddAllModeSharedObj(soloScene, windowSize);

	std::vector<dae::IObserver*> enemyObservers;

	dae::StageManager stageManager{ enemyObservers };
	stageManager.AddToScene(&soloScene);

	// Prefabs
	dae::GameObject* pPlayerOneObject = nullptr;

	float playerElevation{ static_cast<float>(windowSize.y - 100) };
	float midScreen{ static_cast<float>(windowSize.x / 2) };
	constexpr glm::vec2 scoreInset{ 100, 35 };

	dae::Player playerOne{	glm::vec2{ midScreen - 16, playerElevation },			// Player pos
							glm::vec2{ 10, windowSize.y - 26 },						// Lives counter pos
							glm::vec2{ scoreInset.x, scoreInset.y },				// Score counter pos
							"GalagaP1.tga",											// Sprite
							0, 100.0f, 3, dae::InputType::KEYBOARD_AND_GAMEPAD };	// Index, speed, lives, input type

	pPlayerOneObject = playerOne.AddToScene(&soloScene);

	std::vector<dae::GameObject*> players{ pPlayerOneObject };

	auto pScoreTrackerObject = soloScene.AddGameObject();
	pScoreTracker_OUT = pScoreTrackerObject->AddComponent<dae::ScoreTrackerComponent>(players);
}

void InitVersus(dae::Scene& versusScene, glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT)
{
	AddAllModeSharedObj(versusScene, windowSize);

	std::vector<dae::IObserver*> enemyObservers;

	dae::StageManager stageManager{ enemyObservers };
	auto pStageManagerObj = stageManager.AddToScene(&versusScene);
	
	// Prefabs
	dae::GameObject* pPlayerOneObject = nullptr;

	float playerElevation{ static_cast<float>(windowSize.y - 100) };
	float midScreen{ static_cast<float>(windowSize.x / 2) };
	constexpr glm::vec2 scoreInset{ 100, 35 };

	dae::Player playerOne{ glm::vec2{ midScreen - 16, playerElevation },			// Player pos
							glm::vec2{ 10, windowSize.y - 26 },						// Lives counter pos
							glm::vec2{ scoreInset.x, scoreInset.y },				// Score counter pos
							"GalagaP1.tga",											// Sprite
							0, 100.0f, 3, dae::InputType::KEYBOARD_AND_GAMEPAD };	// Index, speed, lives, input type

	pPlayerOneObject = playerOne.AddToScene(&versusScene);

	std::vector<dae::GameObject*> players{ pPlayerOneObject };

	auto pScoreTrackerObject = versusScene.AddGameObject();
	pScoreTracker_OUT = pScoreTrackerObject->AddComponent<dae::ScoreTrackerComponent>(players);

	auto pStageLogic = pStageManagerObj->GetComponent<dae::StageManagerLogic>();

	auto pEnemyControlObj = versusScene.AddGameObject();
	pEnemyControlObj->AddComponent<dae::PlayerEnemyControlComponent>(pStageLogic);
}

void InitCoop(dae::Scene& coopScene, glm::ivec2& windowSize, dae::ScoreTrackerComponent*& pScoreTracker_OUT)
{
	AddAllModeSharedObj(coopScene, windowSize);

	std::vector<dae::IObserver*> enemyObservers;

	dae::StageManager stageManager{ enemyObservers };
	stageManager.AddToScene(&coopScene);

	// Prefabs
	dae::GameObject* pPlayerOneObject = nullptr;
	dae::GameObject* pPlayerTwoObject = nullptr;

	float playerElevation{ static_cast<float>(windowSize.y - 100) };
	//float midScreen{ static_cast<float>(windowSize.x / 2) };
	constexpr glm::vec2 scoreInset{ 100, 35 };

	dae::Player playerOne{ glm::vec2{ 150, playerElevation },						// Player pos
						glm::vec2{ 10, windowSize.y - 26 },						// Lives counter pos
						glm::vec2{ scoreInset.x, scoreInset.y },				// Score counter pos
						"GalagaP1.tga",											// Sprite
						0, 100.0f, 3, dae::InputType::KEYBOARD_AND_GAMEPAD };	// Index, speed, lives, input type

	pPlayerOneObject = playerOne.AddToScene(&coopScene);

	auto textureSize = pPlayerOneObject->GetComponent<dae::TextureComponent>()->GetTextureSize(); // NOTE : Temporary setup
	float p2Offset = (textureSize.x * pPlayerOneObject->GetWorldScale().x);

	dae::Player playerTwo{ glm::vec2{ windowSize.x - 150 - p2Offset, playerElevation },		// Player pos
							glm::vec2{ windowSize.x - 10 - textureSize.x, windowSize.y - 26 },	// Lives counter pos
							glm::vec2{ windowSize.x - scoreInset.x, scoreInset.y },				// Score counter pos
							"GalagaP2.tga",														// Sprite
							1, 100.0f, 3, dae::InputType::GAMEPAD };							// Index, speed, lives, input type

	pPlayerTwoObject = playerTwo.AddToScene(&coopScene);

	std::vector<dae::GameObject*> players{ pPlayerOneObject, pPlayerTwoObject };

	auto pScoreTrackerObject = coopScene.AddGameObject();
	pScoreTracker_OUT = pScoreTrackerObject->AddComponent<dae::ScoreTrackerComponent>(players);
}

void InitResults(dae::Scene& resultsScene, glm::ivec2& windowSize, dae::ScoreTrackerComponent* pScoreTracker)
{
	auto pResultsObj = resultsScene.AddGameObject();
	pResultsObj->AddComponent<dae::ResultsComponent>(pScoreTracker);

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

void AddAllModeSharedObj(dae::Scene& scene, glm::ivec2&)
{
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

	// TODO : Make better background tiling system
	pBackgroundObject->AddChild(pBackgroundTileR);
	pBackgroundObject->AddChild(pBackgroundTileB);
	pBackgroundObject->AddChild(pBackgroundTileBR);
}
