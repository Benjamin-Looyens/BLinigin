#include "Results.h"
#include "ScoreTracker.h"
#include "GameObject.h"
#include "HighScore.h"
#include <json.hpp>
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "TextureComponents.h"
#include "Renderer.h"
#include <SDL.h>
#include <format>
#include "Timer.h"

const std::string dae::ResultsComponent::s_SCORE_FILE = std::string("HighScores.json");

dae::ResultsComponent::ResultsComponent(GameObject* pParent, dae::ScoreTrackerComponent* pTracker)
	:Component(pParent), m_pScoreTracker{ pTracker }
{
	m_pHighScoreComp = m_pParent->AddComponent<dae::HighScoreComponent>();

	auto windowSize = BL_RENDERER.GetWindowSize();
	auto pFont = BL_RESOURCE.LoadFont("Emulogic.ttf", 20);

	constexpr float xPosResults = -250.0f;
	constexpr float xPosValResults = -150.0f;
	constexpr float ySepResults = 25.0f;

	// Setup results visuals
	auto pScene = m_pParent->GetContainingScene();
	m_pResultsTextObj = pScene->AddGameObject();
	m_pResultsTextObj->SetLocalTranslation({ windowSize.x / 2, windowSize.y / 3 });
	m_pResultsTextObj->AddComponent<dae::TextComponent>("--- RESULTS ---", pFont, "Results_Main", true, SDL_Color{ 255, 0, 0, 255 });

	 auto pPlayerTextObj = pScene->AddGameObject();
	 m_pResultsTextObj->AddChild(pPlayerTextObj);
	 pPlayerTextObj->SetLocalTranslation({ 0, -100.0f });
	 m_pPlayerTextComp = pPlayerTextObj->AddComponent<dae::TextComponent>("Enter Player 1 name", pFont, "Results_Player", true, SDL_Color{ 255, 255, 255, 255 });

	 auto pNameTextObj = pScene->AddGameObject();
	 m_pResultsTextObj->AddChild(pNameTextObj);
	 pNameTextObj->SetLocalTranslation({ 0, -55.0f });
	 m_pNameTextComp = pNameTextObj->AddComponent<dae::TextComponent>("___", pFont, "Results_Name", true, SDL_Color{ 255, 255, 255, 255 });

	// SHOTS
	auto pShotsObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pShotsObj);
	pShotsObj->SetLocalTranslation({ xPosResults, ySepResults });
	pShotsObj->AddComponent<dae::TextComponent>("SHOTS FIRED:", pFont, "Results_Shots", false, SDL_Color{ 255, 255, 0, 255 });

	auto pShotsValObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pShotsValObj);
	pShotsValObj->SetLocalTranslation({ -xPosValResults, ySepResults });
	m_pShotsTextComp = pShotsValObj->AddComponent<dae::TextComponent>("0", pFont, "Results_ShotsVal", true, SDL_Color{ 255, 255, 0, 255 });

	// HITS
	auto pHitsObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pHitsObj);
	pHitsObj->SetLocalTranslation({ xPosResults, ySepResults * 2 });
	pHitsObj->AddComponent<dae::TextComponent>("NUMBER OF HITS:", pFont, "Results_Hits", false, SDL_Color{ 255, 255, 0, 255 });

	auto pHitsValObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pHitsValObj);
	pHitsValObj->SetLocalTranslation({ -xPosValResults, ySepResults * 2 });
	m_pHitsTextComp = pHitsValObj->AddComponent<dae::TextComponent>("0", pFont, "Results_HitsVal", true, SDL_Color{ 255, 255, 0, 255 });

	// HIT MISS RATIO
	auto pHitMissObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pHitMissObj);
	pHitMissObj->SetLocalTranslation({ xPosResults, ySepResults * 3 });
	pHitMissObj->AddComponent<dae::TextComponent>("HIT MISS RATIO:", pFont, "Results_Ratio", false, SDL_Color{ 255, 255, 255, 255 });

	auto pHitMissValObj = pScene->AddGameObject();
	m_pResultsTextObj->AddChild(pHitMissValObj);
	pHitMissValObj->SetLocalTranslation({ -xPosValResults, ySepResults * 3 });
	m_pHitMissTextComp = pHitMissValObj->AddComponent<dae::TextComponent>("100%", pFont, "Results_RatioVal", true, SDL_Color{ 255, 255, 255, 255 });

	

	//pDaeProjectTextObj->AddComponent<dae::TextComponent>("DAE GD PROGRAMMING 4", pFont, "DaeProg4Text", true, SDL_Color{ 255, 255, 255, 255 });
}

void dae::ResultsComponent::Update()
{
	if (m_DelayBeforeInput < s_MAX_DELAY_BEFORE_INPUT) m_DelayBeforeInput += BL_TIMER.GetDeltaTime();

	switch (m_State) {
	case ResultsSimpleState::RECEIVING_SCORES:
		m_Scores = m_pScoreTracker->GetScores();
		m_State = ResultsSimpleState::RECEIVING_NAMES;
		break;
	case ResultsSimpleState::RECEIVING_NAMES: {
		if (!m_InitCurrent) {
			m_pShotsTextComp->SetText(std::to_string(m_Scores[m_CurrentPlayerIndex].numShots));
			m_pHitsTextComp->SetText(std::to_string(m_Scores[m_CurrentPlayerIndex].numHits));
			m_pHitMissTextComp->SetText(std::format("{:.1f}", m_Scores[m_CurrentPlayerIndex].GetHitMissRatio()) + "%");
			m_pPlayerTextComp->SetText(std::format("Enter player {} name", (m_CurrentPlayerIndex + 1)));
			m_pNameTextComp->SetText("___");
			m_InitCurrent = true;
		}
		if (m_DelayBeforeInput >= s_MAX_DELAY_BEFORE_INPUT) {
			const char charTyped = BL_INPUT.GetLatestTyped();

			if (charTyped != '\0') {
				m_Scores[m_CurrentPlayerIndex].initials = m_Scores[m_CurrentPlayerIndex].initials + charTyped;

				std::string initialsStr{ m_pNameTextComp->GetText() };

				for (size_t index{}; index < m_Scores[m_CurrentPlayerIndex].initials.size(); ++index) {
					initialsStr[index] = m_Scores[m_CurrentPlayerIndex].initials[index];
				}

				m_pNameTextComp->SetText(initialsStr);
				true;
			}

			if (m_Scores[m_CurrentPlayerIndex].initials.size() >= 3) {
				if (++m_CurrentPlayerIndex >= m_Scores.size()) {
					m_State = ResultsSimpleState::PRINTING_DATA;
					m_pResultsTextObj->SetVisible(false);
				}
				m_InitCurrent = false;
			}
		}
	}
		break;
	case ResultsSimpleState::PRINTING_DATA:
		SaveScores();
		AddHighScoreRenderObjects();
		m_State = ResultsSimpleState::DONE;
		break;
	}
}

void dae::ResultsComponent::SaveScores()
{
	m_pHighScoreComp->LoadScores(s_SCORE_FILE);

	auto pAllScores = m_pHighScoreComp->GetAllScores();

	std::vector<std::unique_ptr<dae::ScoreData>> pTempUniques; // For some reason, when it is a pointer, it does allow me to transfer initials; without this, it does not allow me to?
	pTempUniques.resize(m_Scores.size());

	for (size_t index{}; index < m_Scores.size(); ++index) {
		pTempUniques[index] = std::make_unique<dae::ScoreData>(m_Scores[index]);
		pAllScores.push_back(pTempUniques[index].get());
	}

	nlohmann::json fullJson{};
	std::vector<nlohmann::json> jsonScores{};
	jsonScores.reserve(pAllScores.size());

	for (auto pScore : pAllScores) {
		std::cout << pScore->initials << "\n";

		nlohmann::json curScoreJson{
			{ "Initials", pScore->initials },
			{ "Score", pScore->score },
			{ "Shots", pScore->numShots },
			{ "Hits", pScore->numHits }
		};
		
		jsonScores.push_back(curScoreJson);
	}

	fullJson["RegisteredScores"] = jsonScores;

	BL_RESOURCE.WriteToJSON(s_SCORE_FILE, fullJson);
	m_pHighScoreComp->ReloadScores(s_SCORE_FILE);
}

void dae::ResultsComponent::AddHighScoreRenderObjects()
{
	if (!m_DisplayingScores) {
		if (m_pHighScoreComp) {
			m_pHighScoreComp->AddHighScoreListAsChildren(BL_RESOURCE.LoadFont("Emulogic.ttf", 20), m_pParent->GetContainingScene());

			m_DisplayingScores = true;
		}
	}
}
