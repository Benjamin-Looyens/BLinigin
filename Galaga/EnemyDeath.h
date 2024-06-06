#pragma once
#include "Prefab.h"

namespace dae {
	class EnemyDeath final : public Prefab
	{
	public:
		// TODO : probably customize with parameters & reuse for player
		GameObject* AddToScene(Scene* pSceneToAddTo) override;

	private:
	};
}