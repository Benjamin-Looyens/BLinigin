#pragma once
#include "Component.h"
#include "CollisionComponents.h"
#include "ObserverComponents.h"
namespace dae {
	class DartLogic final : public SubjectComponent
	{
	public:
		void Update() override;

		DartLogic(GameObject* pParent, bool isPlayerDart = true);
		DartLogic(const DartLogic& other) = delete;
		DartLogic(const DartLogic&& other) = delete;
		DartLogic& operator=(const DartLogic& other) = delete;
		DartLogic& operator=(const DartLogic&& other) = delete;

	private:
		CollisionComponent* m_pParentCollision;

		bool m_IsPlayerDart;
		float m_ElapsedTime{};
		static constexpr float MAX_LIFETIME{ 2.0f };
	};
}

