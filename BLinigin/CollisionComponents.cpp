#include "CollisionComponents.h"
#include <SDL.h>
#include "Scene.h"

dae::CollisionComponent::CollisionComponent(GameObject* pParent, glm::vec2 widthHeight)
	: dae::Component(pParent)
{
	auto parentPos = pParent->GetWorldPosition();
	m_CollisionRect = { parentPos.x, parentPos.y, widthHeight.x, widthHeight.y };
}

void dae::CollisionComponent::Update()
{
	// HACK : Could be an observer instead
	auto parentPos = m_pParent->GetWorldPosition();

	m_CollisionRect.x = parentPos.x;
	m_CollisionRect.y = parentPos.y;
}

bool dae::CollisionComponent::DoesOverlap(CollisionComponent* pOtherCollision) const
{
	if (pOtherCollision != nullptr) return DoesOverlap(pOtherCollision->GetRect());

	return false;
}

bool dae::CollisionComponent::DoesOverlap(GameObject* pOtherObject) const
{
	auto otherComponent = pOtherObject->GetComponent<dae::CollisionComponent>();
	
	if (otherComponent != nullptr) return DoesOverlap(otherComponent);

	return false;
}

bool dae::CollisionComponent::DoesOverlap(SDL_FRect rectangle) const
{
	SDL_FRect scaledCollision = GetRect();

	return SDL_HasIntersectionF(&rectangle, &scaledCollision);
}

/// <summary>
/// <b>EXPENSIVE</b> Gets all the game objects that have collision component and overlap the current collision component
/// </summary>
/// <returns>A vector of GameObject* containing the overlapping objects</returns>
std::vector<dae::GameObject*> dae::CollisionComponent::GetAllOverlappingObjects()
{
	std::vector<GameObject*> pOverlappingObjects{};
	auto pScene = m_pParent->GetContainingScene();

	auto pAllObjects = pScene->GetAllObjects();
	for (auto& pObject : pAllObjects) {
		if (pObject) {
			auto pCollisionComp = pObject->GetComponent<dae::CollisionComponent>();

			if (pCollisionComp && DoesOverlap(pCollisionComp)) {
				pOverlappingObjects.push_back(pObject);
			}
		}
	}

	return pOverlappingObjects;
}

SDL_FRect dae::CollisionComponent::GetRect() const
{
	auto scale = m_pParent->GetWorldScale();
	SDL_FRect scaledCollision{ m_CollisionRect.x, m_CollisionRect.y, m_CollisionRect.w * scale.x, m_CollisionRect.h * scale.y };

	return scaledCollision;
}
