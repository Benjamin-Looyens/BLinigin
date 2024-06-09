#pragma once
#include "Component.h"
#include <vector>

namespace dae {
	class GameObject;
	class ScoreTrackerComponent;
	class HighScoreComponent; 
	struct ScoreData;
	class TextComponent;
	
	class ResultsComponent final : public Component
	{
		enum class ResultsSimpleState
		{
			RECEIVING_SCORES,
			RECEIVING_NAMES,
			PRINTING_DATA,
			DONE
		};
	public:
		ResultsComponent(GameObject* pParent, dae::ScoreTrackerComponent* pTracker);
		
		void Update() override;

		void SaveScores();
		void AddHighScoreRenderObjects();

	private:
		dae::ScoreTrackerComponent* m_pScoreTracker;
		dae::HighScoreComponent* m_pHighScoreComp;

		bool m_DisplayingScores = false;
		bool m_ReceivedScores = false;

		std::vector<dae::ScoreData> m_Scores;
		size_t m_CurrentPlayerIndex{};

		static const std::string s_SCORE_FILE;

		dae::ResultsComponent::ResultsSimpleState m_State = dae::ResultsComponent::ResultsSimpleState::RECEIVING_SCORES;

		bool m_InitCurrent = false;
		dae::GameObject* m_pResultsTextObj = nullptr;
		dae::TextComponent* m_pPlayerTextComp = nullptr;
		dae::TextComponent* m_pNameTextComp = nullptr;
		dae::TextComponent* m_pResultsTextComp = nullptr;
		dae::TextComponent* m_pShotsTextComp = nullptr;
		dae::TextComponent* m_pHitsTextComp = nullptr;
		dae::TextComponent* m_pHitMissTextComp = nullptr;

		float m_DelayBeforeInput{};
		static constexpr float s_MAX_DELAY_BEFORE_INPUT{ 2.0f };
	};
}