#pragma once
#include "Component.h"
#include "ObserverComponents.h"
#include <vector>
#include "HighScore.h"

namespace dae {
	class GameObject;
	
	class ScoreTrackerComponent final : public ObservingComponent
	{
	public:
		ScoreTrackerComponent(GameObject* pParent, std::vector<GameObject*> pPlayers);

		void OnNotify(const dae::ISubject* pSubject, dae::Event event) override;
		std::vector<dae::ScoreData> GetScores();

	private:
		std::vector<std::pair<GameObject*, dae::ScoreData>> m_Players;

		int m_NumAlivePlayers;
	};
}

