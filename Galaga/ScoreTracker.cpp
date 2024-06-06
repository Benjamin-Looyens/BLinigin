#include "ScoreTracker.h"
#include "DerivedEvents.h"
#include "SceneManager.h"

dae::ScoreTrackerComponent::ScoreTrackerComponent(GameObject* pParent, std::vector<GameObject*> pPlayers)
	: ObservingComponent(pParent), m_pPlayers{ pPlayers }, m_NumAlivePlayers{ static_cast<int>(pPlayers.size()) }
{}

void dae::ScoreTrackerComponent::OnNotify(const dae::ISubject*, dae::Event event)
{
	switch (event) {
	case Event::GAME_OVER:
		m_NumAlivePlayers--;

		if (m_NumAlivePlayers <= 0) {
			// TODO : Implement True game over logic

			BL_SCENE_MANAGER.SetActiveScene("Results");
		}
		break;
	}
}
