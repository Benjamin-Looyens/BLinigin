#include "DerivedObservers.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "Events.h"
#include <string>

#pragma region LivesObserver
dae::LivesObserver::LivesObserver(GameObject* pDataContainingObject, GameObject* pDataDisplayObject)
	: DataDisplayObserver(pDataContainingObject, pDataDisplayObject)
{}

void dae::LivesObserver::OnNotify(const GameObject* pSubjectObject, Event event)
{
	pSubjectObject; // I SOMETIMES need pSubjectObject in an observer but not ALWAYS and warning level is not allowing me to not reference a parameter so uhhhhhhhhh

	auto pLivesComp = GetGameObject()->GetComponent<dae::LivesComponent>();
	auto pDataComp = GetDisplayObject()->GetComponent<dae::DisplayDataComponent>();

	switch (event) {
	case Event::PLAYER_DIED:
		pLivesComp->LoseLife();

		break;
	}

	pDataComp->UpdateDisplay(std::to_string(pLivesComp->GetLifeCount()));
}
#pragma endregion LivesObserver

#pragma region ScoreObserver
dae::ScoreObserver::ScoreObserver(GameObject* pScoreOwningObject, GameObject* pDataDisplayObject)
	: DataDisplayObserver(pScoreOwningObject, pDataDisplayObject)
{}

void dae::ScoreObserver::OnNotify(const GameObject* pSubjectObject, Event event)
{
	pSubjectObject; // I SOMETIMES need pSubjectObject in an observer but not ALWAYS and warning level is not allowing me to not reference a parameter so uhhhhhhhhh
	
	auto pScoreComp = GetGameObject()->GetComponent<dae::ScoreComponent>();
	auto pDataComp = GetDisplayObject()->GetComponent<dae::DisplayDataComponent>();

	switch (event) {
	case Event::BIG_ENEMY_DIED:
		pScoreComp->AddPoints(500);
		break;
	case Event::SMALL_ENEMY_DIED:
		pScoreComp->AddPoints(100);
		break;
	}

	pDataComp->UpdateDisplay(std::to_string(pScoreComp->GetScore()));
}
#pragma endregion ScoreObserver