#pragma once
#include "GameObject.h"
#include "Component.h"
#include <glm/vec2.hpp>
#include <SDL_rect.h>

namespace dae {
	class CollisionComponent final : public Component
	{
	public:
		bool DoesOverlap(CollisionComponent* otherCollision) const;
		bool DoesOverlap(GameObject* otherObject) const;
		bool DoesOverlap(SDL_FRect rectangle) const;
		std::vector<GameObject*> GetAllOverlappingObjects();

		SDL_FRect GetRect() const;

		// TODO : Check if need to inherit or not, if not, remove virtual destructor & update
		virtual void Update() override;

		CollisionComponent(GameObject* pParent, glm::vec2 widthHeight);
		virtual ~CollisionComponent() = default; 
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(const CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(const CollisionComponent&& other) = delete;

	private:
		SDL_FRect m_CollisionRect;
	};
}