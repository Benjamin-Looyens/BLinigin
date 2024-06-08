#include "ScoreTracker.h"
#include "DerivedEvents.h"
#include "SceneManager.h"
#include "PlayerComponents.h"
#include <algorithm>

dae::ScoreTrackerComponent::ScoreTrackerComponent(GameObject* pParent, std::vector<GameObject*> pPlayers)
	: ObservingComponent(pParent), m_NumAlivePlayers{ static_cast<int>(pPlayers.size()) }
{
	for (auto& pPlayer : pPlayers) {
		m_Players.push_back({ pPlayer, {} });

		pPlayer->GetComponent<dae::PlayerLogic>()->AddObserver(this);
		pPlayer->GetComponent<dae::LivesComponent>()->AddObserver(this);
		pPlayer->GetComponent<dae::ScoreComponent>()->AddObserver(this);
	}
}

void dae::ScoreTrackerComponent::OnNotify(const dae::ISubject* pSubject, dae::Event event)
{
	const dae::PlayerLogic* pPlayerLogic = dynamic_cast<const dae::PlayerLogic*>(pSubject);
	const dae::ScoreComponent* pPlayerScore = dynamic_cast<const dae::ScoreComponent*>(pSubject);

	switch (event) {
	case Event::GAME_OVER:
		m_NumAlivePlayers--;

		if (m_NumAlivePlayers <= 0) {
			for (auto& player : m_Players) {
				auto pPlayer = std::get<GameObject*>(player);

				if (pPlayer) {
					auto pScore = pPlayer->GetComponent<dae::ScoreComponent>();
					std::get<ScoreData>(player).score = pScore->GetScore();
				}
			}

			if (m_Players.size() == 1) BL_SCENE_MANAGER.SetActiveScene("SoloResults");
			else if (m_Players.size() == 2) BL_SCENE_MANAGER.SetActiveScene("CoopResults");
			else {
				// TODO : Handle Versus Results
			}
		}
		break;
	case Event::PLAYER_SHOOT:
		if (pPlayerLogic) {
			auto pPlayerObj = pPlayerLogic->GetParent();

			if (pPlayerObj) {
				auto playerIt = std::find_if(m_Players.begin(), m_Players.end(), [pPlayerObj](const std::pair<GameObject*, dae::ScoreData>& player) 
					{ return player.first == pPlayerObj; });

				if (playerIt != end(m_Players)) {
					std::get<dae::ScoreData>(*playerIt).numShots++;
				}
			}
		}
		break;
	case Event::PLAYER_SHOT_ENEMY:
		if (pPlayerScore) {
			auto pPlayerObj = pPlayerScore->GetParent();

			if (pPlayerObj) {
				auto playerIt = std::find_if(m_Players.begin(), m_Players.end(), [pPlayerObj](const std::pair<GameObject*, dae::ScoreData>& player)
					{ return player.first == pPlayerObj; });

				if (playerIt != end(m_Players)) {
					std::get<dae::ScoreData>(*playerIt).numHits++;
				}
			}
		}
		break;
	}
}

std::vector<dae::ScoreData> dae::ScoreTrackerComponent::GetScores()
{
	std::vector<dae::ScoreData> pReturnScores{};
	pReturnScores.resize(m_Players.size());

	std::transform(m_Players.begin(), m_Players.end(), pReturnScores.begin(),
		[](const std::pair<GameObject*, dae::ScoreData>& player) { return (std::get<ScoreData>(player)); });

	return pReturnScores;
}
