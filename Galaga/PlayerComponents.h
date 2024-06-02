#pragma once
#include "GameObject.h"
#include "Component.h"
#include "Observer.h"
#include "Dart.h"

namespace dae {
	class PlayerLogic final : public Component
	{
	public:
		void Update() override;
		void LateUpdate() override;

		void Fire();

		PlayerLogic(GameObject* pParent);

		PlayerLogic(const PlayerLogic& other) = delete;
		PlayerLogic(const PlayerLogic&& other) = delete;
		PlayerLogic& operator=(const PlayerLogic& other) = delete;
		PlayerLogic& operator=(const PlayerLogic&& other) = delete;

	private:
		std::vector<GameObject*> m_pDarts;
	};
}
