#include "DerivedObservers.h"
#include "GameObject.h"
#include "DerivedComponents.h"
#include "Events.h"

#pragma region LivesObserver
dae::LivesObserver::LivesObserver(GameObject* pLivesCounterObject)
	: GameObjectObserver(pLivesCounterObject)
{}

void dae::LivesObserver::OnNotify(const GameObject* pObservedObject, Event& event)
{
	if (event == Event::PLAYER_DIED) {
		auto component = GetGameObject()->GetComponent<dae::LivesCounterComponent>();

		component->LoseLife();
		
		pObservedObject->HasParent();
	}
}
#pragma endregion LivesObserver