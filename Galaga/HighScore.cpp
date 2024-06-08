#include "HighScore.h"
#include "GameObject.h"
#include <algorithm>
#include "ResourceManager.h"
#include <json.hpp>
#include "Renderer.h"
#include "TextureComponents.h"
#include "Scene.h"

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
			// TODO : Handle error logic
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

void dae::HighScoreComponent::ReloadScores(const std::string& scoresFile)
{
	m_pLoadedScores.clear();
	BL_RESOURCE.UnLoadJSON(scoresFile);
	LoadScores(scoresFile);
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
					[](const std::unique_ptr<ScoreData>& pUniqueScore) { return pUniqueScore.get(); });

	// Sort raw pointers
	std::sort(pHighestScores.begin(), pHighestScores.end(),
				[](dae::ScoreData* pFirst, dae::ScoreData* pSecond) { return pFirst->score > pSecond->score; });

	pHighestScores.resize(5);

	return pHighestScores;
}

std::vector<dae::ScoreData*> dae::HighScoreComponent::GetAllScores() const
{
	std::vector<dae::ScoreData*> pRawScores{};
	
	if (m_pLoadedScores.size() > 0) {
		pRawScores.resize(m_pLoadedScores.size());

		std::transform(m_pLoadedScores.begin(), m_pLoadedScores.end(), pRawScores.begin(),
			[](const std::unique_ptr<ScoreData>& pUniqueScore) { return pUniqueScore.get(); });
	}
	
	return pRawScores;
}

void dae::HighScoreComponent::SetVisible(bool newVisible)
{
	m_pParent->SetVisible(newVisible);
}

void dae::HighScoreComponent::AddHighScoreListAsChildren(dae::Font* pFontToUse, dae::Scene* pSceneToAddTo)
{
	dae::Scene* pScene = nullptr;
	dae::GameObject* pAbsoluteRoot = nullptr;
	if (pSceneToAddTo == nullptr) {
		pScene = m_pParent->GetContainingScene();
		pAbsoluteRoot = m_pParent;
	}
	else {
		pScene = pSceneToAddTo;
		pAbsoluteRoot = pScene->AddGameObject(); // To guarantee no weird shenanigans with cross-scene game objects & children
	}
	
	glm::vec2 windowSize = BL_RENDERER.GetWindowSize();
	float xWindowSeperation = windowSize.x / 4;
	float yWindowSeperation = windowSize.y / 6;
	float hsLadderElevation = yWindowSeperation * 2;

	pAbsoluteRoot->SetLocalTranslation({ windowSize.x / 2, 0 });

	std::vector<std::string> scoreStrings;
	scoreStrings.resize(5);
	std::vector<std::string> nameStrings;
	nameStrings.resize(5);
	auto pHighScores = Get5HighestScores();

	// Score to string data
	std::transform(pHighScores.begin(), pHighScores.end(), scoreStrings.begin(),
		[](const ScoreData* pScore)
		{
			if (pScore) return std::to_string(pScore->score);
			else return std::string("-----");
		});

	// Initials stored
	std::transform(pHighScores.begin(), pHighScores.end(), nameStrings.begin(),
		[](const ScoreData* pScore)
		{
			if (pScore) return pScore->initials;
			else return std::string("---");
		});

	// Number (1st, 2nd, ...)
	auto pEmptyNumber = pScene->AddGameObject(); // ROOT of the numbers
	pAbsoluteRoot->AddChild(pEmptyNumber);
	pEmptyNumber->SetLocalTranslation({ -xWindowSeperation , hsLadderElevation });

	auto pFirst = pScene->AddGameObject();
	auto pSecond = pScene->AddGameObject();
	auto pThird = pScene->AddGameObject();
	auto pFourth = pScene->AddGameObject();
	auto pFifth = pScene->AddGameObject();

	pFirst->AddComponent<dae::TextComponent>("1ST", pFontToUse, "HighScore1stLabel", true);
	pSecond->AddComponent<dae::TextComponent>("2ND", pFontToUse, "HighScore2ndLabel", true);
	pThird->AddComponent<dae::TextComponent>("3RD", pFontToUse, "HighScore3rdLabel", true);
	pFourth->AddComponent<dae::TextComponent>("4TH", pFontToUse, "HighScore4thLabel", true);
	pFifth->AddComponent<dae::TextComponent>("5TH", pFontToUse, "HighScore5thLabel", true);

	pEmptyNumber->AddChild(pFirst);
	pEmptyNumber->AddChild(pSecond);
	pEmptyNumber->AddChild(pThird);
	pEmptyNumber->AddChild(pFourth);
	pEmptyNumber->AddChild(pFifth);

	pFirst->SetLocalTranslation({ 0, 25.0f * 1 });
	pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
	pThird->SetLocalTranslation({ 0, 25.0f * 3 });
	pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
	pFifth->SetLocalTranslation({ 0, 25.0f * 5 });

	// Score
	auto pScoreRoot = pScene->AddGameObject(); // ROOT of the scores
	pScoreRoot->AddComponent<dae::TextComponent>("SCORE", pFontToUse, "HighScore_ScoreRoot", true);
	pAbsoluteRoot->AddChild(pScoreRoot);
	pScoreRoot->SetLocalTranslation({ 0, hsLadderElevation });

	pFirst = pScene->AddGameObject();
	pSecond = pScene->AddGameObject();
	pThird = pScene->AddGameObject();
	pFourth = pScene->AddGameObject();
	pFifth = pScene->AddGameObject();

	pFirst->AddComponent<dae::TextComponent>(scoreStrings[0], pFontToUse, "HighScore1stScore", true);
	pSecond->AddComponent<dae::TextComponent>(scoreStrings[1], pFontToUse, "HighScore2ndScore", true);
	pThird->AddComponent<dae::TextComponent>(scoreStrings[2], pFontToUse, "HighScore3rdScore", true);
	pFourth->AddComponent<dae::TextComponent>(scoreStrings[3], pFontToUse, "HighScore4thScore", true);
	pFifth->AddComponent<dae::TextComponent>(scoreStrings[4], pFontToUse, "HighScore5thScore", true);

	pScoreRoot->AddChild(pFirst);
	pScoreRoot->AddChild(pSecond);
	pScoreRoot->AddChild(pThird);
	pScoreRoot->AddChild(pFourth);
	pScoreRoot->AddChild(pFifth);

	pFirst->SetLocalTranslation({ 0, 25.0f * 1 });
	pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
	pThird->SetLocalTranslation({ 0, 25.0f * 3 });
	pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
	pFifth->SetLocalTranslation({ 0, 25.0f * 5 });

	// Name
	auto pNameRoot = pScene->AddGameObject(); // ROOT of the names/initials
	pNameRoot->AddComponent<dae::TextComponent>("NAME", pFontToUse, "HighScore_NameRoot", true);
	pAbsoluteRoot->AddChild(pNameRoot);
	pNameRoot->SetLocalTranslation({ xWindowSeperation, hsLadderElevation });

	pFirst = pScene->AddGameObject();
	pSecond = pScene->AddGameObject();
	pThird = pScene->AddGameObject();
	pFourth = pScene->AddGameObject();
	pFifth = pScene->AddGameObject();

	pFirst->AddComponent<dae::TextComponent>(nameStrings[0], pFontToUse, "HighScore1stName", true);
	pSecond->AddComponent<dae::TextComponent>(nameStrings[1], pFontToUse, "HighScore2ndName", true);
	pThird->AddComponent<dae::TextComponent>(nameStrings[2], pFontToUse, "HighScore3rdName", true);
	pFourth->AddComponent<dae::TextComponent>(nameStrings[3], pFontToUse, "HighScore4thName", true);
	pFifth->AddComponent<dae::TextComponent>(nameStrings[4], pFontToUse, "HighScore5thName", true);

	pNameRoot->AddChild(pFirst);
	pNameRoot->AddChild(pSecond);
	pNameRoot->AddChild(pThird);
	pNameRoot->AddChild(pFourth);
	pNameRoot->AddChild(pFifth);

	pFirst->SetLocalTranslation({ 0, 25.0f * 1 });
	pSecond->SetLocalTranslation({ 0, 25.0f * 2 });
	pThird->SetLocalTranslation({ 0, 25.0f * 3 });
	pFourth->SetLocalTranslation({ 0, 25.0f * 4 });
	pFifth->SetLocalTranslation({ 0, 25.0f * 5 });
}

std::ostream& dae::operator<<(std::ostream& os, const ScoreData& score)
{
	os << "Initials: " << score.initials << "\n";
	os << "Score: " << score.score << "\n";
	os << "Number shots: " << score.numShots << "\n";
	os << "Hit/Miss ratio: " << std::fixed << std::setprecision(1) << score.GetHitMissRatio() << "%\n";
	
	return os;
}

float dae::ScoreData::GetHitMissRatio() const
{
	if (numShots == 0) return 100.0f;

	return (static_cast<float>(numHits) / static_cast<float>(numShots)) * 100;
}
