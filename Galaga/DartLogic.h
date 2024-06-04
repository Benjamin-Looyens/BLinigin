#pragma once
#include "Component.h"
#include "CollisionComponents.h"
#include "ObserverComponents.h"

namespace dae {
	class DartLogic final : public SubjectComponent
	{
	public:
		void Update() override;

		DartLogic(GameObject* pParent);
		DartLogic(const DartLogic& other) = delete;
		DartLogic(const DartLogic&& other) = delete;
		DartLogic& operator=(const DartLogic& other) = delete;
		DartLogic& operator=(const DartLogic&& other) = delete;

	private:
		CollisionComponent* m_pParentCollision;

		float m_ElapsedTime{};
		static constexpr float MAX_LIFETIME{ 10.0f };
	};
}

