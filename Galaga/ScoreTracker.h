#pragma once
#include "Component.h"
#include "ObserverComponents.h"
#include <vector>

namespace dae {
	class GameObject;
	
	class ScoreTrackerComponent final : public ObservingComponent
	{
	public:
		ScoreTrackerComponent(GameObject* pParent, std::vector<GameObject*> pPlayers);

		void OnNotify(const dae::ISubject* pSubject, dae::Event event) override;

	private:
		std::vector<GameObject*> m_pPlayers;

		int m_NumAlivePlayers;
	};
}

