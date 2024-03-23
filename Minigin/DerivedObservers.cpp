#include "DerivedObservers.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "Events.h"

#pragma region LivesObserver
dae::LivesObserver::LivesObserver(GameObject* pLivesCounterObject)
	: GameObjectObserver(pLivesCounterObject)
{}

void dae::LivesObserver::OnNotify(const GameObject* pObservedObject, Event event)
{
	// TODO : fix the shit with pObservedObject (idk if i need it or not)
	if (event == Event::PLAYER_DIED) {
		auto component = GetGameObject()->GetComponent<dae::LivesCounterComponent>();

		component->LoseLife();
		
		if (pObservedObject) pObservedObject->HasParent();
	}
}
#pragma endregion LivesObserver