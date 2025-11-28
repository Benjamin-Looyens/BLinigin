#pragma once
#include "Prefab.h"
#include "Observer.h"

namespace dae {
	class GameObject;
	class Scene;
	
	class StageManager final : public Prefab
	{
	public:
		StageManager(std::vector<IObserver*>& enemyObservers);

		dae::GameObject* AddToScene(dae::Scene* pSceneToAddTo) override;

	private:
		std::vector<IObserver*> m_EnemyObservers{};
	};
}
