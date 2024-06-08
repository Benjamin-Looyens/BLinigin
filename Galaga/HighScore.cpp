#include "HighScore.h"
#include "GameObject.h"
#include <algorithm>
#include "ResourceManager.h"
#include <json.hpp>

dae::HighScoreComponent::HighScoreComponent(dae::GameObject* pParent)
	: Component(pParent)
{}

dae::HighScoreComponent::HighScoreComponent(GameObject * pParent, const std::string & scoresFile)
	: Component(pParent)
{
	LoadScores(scoresFile);
}

void dae::HighScoreComponent::LoadScores(const std::string& scoresFile)
{
	auto pJson = BL_RESOURCE.LoadJSON(scoresFile);

	if (pJson) {
		if (pJson->contains("RegisteredScores") && pJson->at("RegisteredScores").is_array()) {
			auto& scores = pJson->at("RegisteredScores");

			int entryNum{ };

			for (auto& scoreDataRaw : scores) {
				dae::ScoreData parsedScore{};
				bool success = true;
				entryNum++;

				if (scoreDataRaw.contains("Initials") && scoreDataRaw.at("Initials").is_string()) {
					parsedScore.initials = scoreDataRaw.at("Initials");
				}
				else success = false;
				if (scoreDataRaw.contains("Score") && scoreDataRaw.at("Score").is_number_integer()) {
					parsedScore.score = scoreDataRaw.at("Score");
				}
				else success = false;
				if (scoreDataRaw.contains("Shots") && scoreDataRaw.at("Shots").is_number_integer()) {
					parsedScore.numShots = scoreDataRaw.at("Shots");
				}
				else success = false;
				if (scoreDataRaw.contains("Hits") && scoreDataRaw.at("Hits").is_number_integer()) {
					parsedScore.numHits = scoreDataRaw.at("Hits");
				}
				else success = false;

				if (success) {
					m_pLoadedScores.push_back(std::make_unique<dae::ScoreData>(parsedScore));
				}
				else {
#ifndef NDEBUG
					std::cerr << "High score file [" << scoresFile << "] entry at [" << entryNum << "] does not contain correct data!\n";
#endif
				}
			}
			
		}
		else {
#ifndef NDEBUG
			std::cerr << "High score file [" << scoresFile << "] does not contain scores!\n";
#endif
			return;
		}
	}
	else {
		// Create high scores file
	}

#ifndef NDEBUG
	PrintScores();
#endif
}

void dae::HighScoreComponent::PrintScores() const
{
	if (m_pLoadedScores.size() > 0) {
		std::cout << "*** HighScores ***\n";

		for (auto& pScore : m_pLoadedScores) {
			std::cout << *pScore << "\n";
		}
	}
}

std::vector<dae::ScoreData*> dae::HighScoreComponent::Get5HighestScores() const
{
	if (m_pLoadedScores.size() == 0) {
		return std::vector<dae::ScoreData*>{ nullptr, nullptr, nullptr, nullptr, nullptr }; // return nullptr filled vector as no scores are currently loaded
	}

	std::vector<dae::ScoreData*> pHighestScores;
	pHighestScores.resize(m_pLoadedScores.size());

	// Copy over scores
	std::transform(m_pLoadedScores.begin(), m_pLoadedScores.end(), pHighestScores.begin(),
					[](const std::unique_ptr<ScoreData>& uniquePtr) { return uniquePtr.get(); });

	// Sort raw pointers
	std::sort(pHighestScores.begin(), pHighestScores.end(),
				[](dae::ScoreData* a, dae::ScoreData* b) { return a->score > b->score; });

	pHighestScores.resize(5);

	return pHighestScores;
}

void dae::HighScoreComponent::SetVisible(bool newVisible)
{
	m_pParent->SetVisible(newVisible);
};

std::ostream& dae::operator<<(std::ostream& os, const ScoreData& score)
{
	os << "Initials: " << score.initials << "\n";
	os << "Score: " << score.score << "\n";
	os << "Number shots: " << score.numShots << "\n";
	os << "Hit/Miss ratio: " << std::fixed << std::setprecision(1) << score.GetHitMissRatio() << "%\n";
	
	return os;
}
