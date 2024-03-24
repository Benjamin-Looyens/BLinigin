#pragma once
#include "Observer.h"

namespace dae {
	class GameObject;

	class GameObjectObserver : public Observer
	{
	public:
		GameObjectObserver(GameObject* pGameObject) : m_pStoredGameObject{ pGameObject } {};

	protected:
		GameObject* GetGameObject() { return m_pStoredGameObject; };

	private:
		GameObject* m_pStoredGameObject;
	};



	class LivesObserver : public GameObjectObserver
	{
	public:
		LivesObserver(GameObject* pLivesCounterObject);

		virtual void OnNotify(const GameObject* subjectObject, Event event) override;
	};


}

