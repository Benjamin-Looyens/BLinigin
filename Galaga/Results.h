#pragma once
#include "Component.h"

namespace dae {
	class GameObject;
	class ScoreTrackerComponent;
	class HighScoreComponent; 
	
	class ResultsComponent final : public Component
	{
	public:
		ResultsComponent(GameObject* pParent, dae::ScoreTrackerComponent* pTracker);
		
		void Update() override;

		void SaveScores();
		void AddHighScoreRenderObjects();

	private:
		dae::ScoreTrackerComponent* m_pScoreTracker;
		dae::HighScoreComponent* m_pHighScoreComp;

		bool m_DisplayingScores;

		bool m_SavedResults = false;

		static const std::string s_SCORE_FILE;
	};
}