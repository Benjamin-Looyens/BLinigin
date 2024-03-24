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

	class DataDisplayObserver : public GameObjectObserver
	{
	public:
		DataDisplayObserver(GameObject* pDataContainingObject, GameObject* pDataDisplayObject) : GameObjectObserver(pDataContainingObject), m_pDisplayObject{ pDataDisplayObject } {};

	protected:
		GameObject* GetDisplayObject() { return m_pDisplayObject; };

	private:
		GameObject* m_pDisplayObject;
	};


	class LivesObserver : public DataDisplayObserver
	{
	public:
		LivesObserver(GameObject* pLivesCounterObject, GameObject* pDataDisplayObject);

		virtual void OnNotify(const GameObject* pSubjectObject, Event event) override;
	};

	class ScoreObserver : public DataDisplayObserver
	{
	public:
		ScoreObserver(GameObject* pScoreOwningObject, GameObject* pDataDisplayObject);

		virtual void OnNotify(const GameObject* pSubjectObject, Event event) override;
	};
}

