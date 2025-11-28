#pragma once
#include "Component.h"
#include <vector>
#include <memory>

namespace dae {
	class GameObject;
	class Font;
	class Scene;

	struct ScoreData
	{
		std::string initials;
		int score;
		int numShots;
		int numHits;

		friend std::ostream& operator<<(std::ostream& os, const ScoreData& score);

		float GetHitMissRatio() const;
	};

	class HighScoreComponent final : public Component
	{
	public:
		HighScoreComponent(GameObject* pParent);
		HighScoreComponent(GameObject* pParent, const std::string& scoresFile);

		void LoadScores(const std::string& scoresFile);
		void ReloadScores(const std::string& scoresFile);
		void PrintScores() const;
		std::vector<ScoreData*> Get5HighestScores() const;
		std::vector<ScoreData*> GetAllScores() const;

		void SetVisible(bool newVisible);
		void AddHighScoreListAsChildren(dae::Font* pFontToUse, dae::Scene* pSceneToAddTo = nullptr);

	private:
		std::vector<std::unique_ptr<dae::ScoreData>> m_pLoadedScores{};
		
		bool m_HasAddedChildren = false;
	};
}