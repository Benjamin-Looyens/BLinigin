#include "DerivedObservers.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "DerivedEvents.h"
#include <string>
#include "ServiceLocator.h"

#pragma region LivesObserver
dae::LivesObserver::LivesObserver(GameObject* pDataContainingObject, GameObject* pDataDisplayObject)
	: DataDisplayObserver(pDataContainingObject, pDataDisplayObject)
{}

void dae::LivesObserver::OnNotify(const GameObject*, Event event)
{
	auto pLivesComp = GetGameObject()->GetComponent<dae::LivesComponent>();
	auto pDataComp = GetDisplayObject()->GetComponent<dae::DisplayDataComponent>();
	int prevLives = pLivesComp->GetLifeCount();

	switch (event) {
	case Event::PLAYER_DIED:
		pLivesComp->LoseLife();
		ServiceLocator::GetSoundSystem().PlaySound(9, 0.4f);
		break;
	}

	if (prevLives != pLivesComp->GetLifeCount())
		pDataComp->UpdateDisplay(std::to_string(pLivesComp->GetLifeCount()));
}
#pragma endregion LivesObserver

#pragma region ScoreObserver
dae::ScoreObserver::ScoreObserver(GameObject* pScoreOwningObject, GameObject* pDataDisplayObject)
	: DataDisplayObserver(pScoreOwningObject, pDataDisplayObject)
{}

void dae::ScoreObserver::OnNotify(const GameObject*, Event event)
{
	auto pScoreComp = GetGameObject()->GetComponent<dae::ScoreComponent>();
	auto pDataComp = GetDisplayObject()->GetComponent<dae::DisplayDataComponent>();
	int prevScore = pScoreComp->GetScore();

	switch (event) {
	case Event::BIG_ENEMY_DIED:
		pScoreComp->AddPoints(500);
		ServiceLocator::GetSoundSystem().PlaySound(8, 0.4f);
		break;
	case Event::SMALL_ENEMY_DIED:
		pScoreComp->AddPoints(100);
		ServiceLocator::GetSoundSystem().PlaySound(10, 0.4f);
		break;
	}

	if (prevScore != pScoreComp->GetScore())
		pDataComp->UpdateDisplay(std::to_string(pScoreComp->GetScore()));
}
#pragma endregion ScoreObserver