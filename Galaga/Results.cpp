#include "Results.h"
#include "ScoreTracker.h"
#include "GameObject.h"
#include "HighScore.h"
#include <json.hpp>
#include "ResourceManager.h"
#include "InputManager.h"

const std::string dae::ResultsComponent::s_SCORE_FILE = std::string("HighScores.json");

dae::ResultsComponent::ResultsComponent(GameObject* pParent, dae::ScoreTrackerComponent* pTracker)
	:Component(pParent), m_pScoreTracker{ pTracker }
{
	m_pHighScoreComp = m_pParent->AddComponent<dae::HighScoreComponent>();
}

void dae::ResultsComponent::Update()
{
	switch (m_State) {
	case ResultsSimpleState::RECEIVING_SCORES:
		m_Scores = m_pScoreTracker->GetScores();
		m_State = ResultsSimpleState::RECEIVING_NAMES;
		break;
	case ResultsSimpleState::RECEIVING_NAMES: {
		const char charTyped = BL_INPUT.GetLatestTyped();

		if (charTyped != '\0') {
			m_Scores[m_CurrentPlayerIndex].initials = m_Scores[m_CurrentPlayerIndex].initials + charTyped;
			true;
		}

		if (m_Scores[m_CurrentPlayerIndex].initials.size() >= 3) {
			if (++m_CurrentPlayerIndex >= m_Scores.size()) {
				m_State = ResultsSimpleState::PRINTING_DATA;
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
