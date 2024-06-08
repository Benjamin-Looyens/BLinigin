#pragma once
#include "Component.h"
#include <vector>
#include <memory>

namespace dae {
	class GameObject;

	struct ScoreData
	{
		std::string initials;
		int score;
		int numShots;
		int numHits;

		friend std::ostream& operator<<(std::ostream& os, const ScoreData& score);
	};

	class HighScoreDisplayComponent final : public Component
	{
	public:
		HighScoreDisplayComponent(GameObject* pParent);
		HighScoreDisplayComponent(GameObject* pParent, const std::string& scoresFile);

		void LoadScores(const std::string& scoresFile);
		void PrintScores() const;
		std::vector<ScoreData*> Get5HighestScores() const;

		void SetVisible(bool newVisible);

	private:
		std::vector<std::unique_ptr<dae::ScoreData>> m_pLoadedScores;
	};
}