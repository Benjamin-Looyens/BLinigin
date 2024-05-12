#pragma once
#include "Component.h"
#include "ObserverComponents.h"
#include "EnemyState.h"
#include <memory>

namespace dae {
	class GameObject;

	class EnemyLogic final : public Component
	{
	public:
		EnemyLogic(GameObject* pParent, float speed);
		void Update() override;
		void LateUpdate() override;
		
		void SetState(std::unique_ptr<EnemyState> pNewState);
		void SetLocation(glm::vec3 pos);
		glm::vec2 GetLocation();
		float GetSpeed() const { return m_Speed; };

	private:
		std::unique_ptr<EnemyState> m_pState;
		std::unique_ptr<EnemyState> m_pSwitchState;

		float m_Speed;
	};
}