#include "DerivedObservers.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "Events.h"

#pragma region LivesObserver
dae::LivesObserver::LivesObserver(GameObject* pLivesCounterObject)
	: GameObjectObserver(pLivesCounterObject)
{}

void dae::LivesObserver::OnNotify(const GameObject* pSubjectObject, Event event)
{
	pSubjectObject; // I SOMETIMES need pSubjectObject in an observer but not ALWAYS and warning level is not allowing me to not reference a parameter so uhhhhhhhhh

	switch (event) {
	case Event::PLAYER_DIED:
		auto component = GetGameObject()->GetComponent<dae::LivesCounterComponent>();

		component->LoseLife();
		break;
	}
}
#pragma endregion LivesObserver