#include "Results.h"
#include "ScoreTracker.h"
#include "GameObject.h"
#include "HighScore.h"
#include <json.hpp>
#include "ResourceManager.h"

const std::string dae::ResultsComponent::s_SCORE_FILE = std::string("HighScores.json");

dae::ResultsComponent::ResultsComponent(GameObject* pParent, dae::ScoreTrackerComponent* pTracker)
	:Component(pParent), m_pScoreTracker{ pTracker }
{
	m_pHighScoreComp = m_pParent->AddComponent<dae::HighScoreComponent>();
}

void dae::ResultsComponent::Update()
{
	if (!m_SavedResults) {
		SaveScores();

		m_SavedResults = true;
	}
}

void dae::ResultsComponent::SaveScores()
{
	auto scores = m_pScoreTracker->GetScores();
	m_pHighScoreComp->LoadScores(s_SCORE_FILE);

	auto pAllScores = m_pHighScoreComp->GetAllScores();

	for (auto score : scores) {
		score.initials = "TMP"; // TODO : REPLACE TEMP NAME SETUP
		pAllScores.push_back(&score);
	}

	nlohmann::json fullJson{};
	std::vector<nlohmann::json> jsonScores{};
	jsonScores.reserve(pAllScores.size());

	for (auto pScore : pAllScores) {
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
		

		m_DisplayingScores = true;
	}
}
