#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "DerivedComponents.h"
#include <iostream>
#include <algorithm>

dae::GameObject::GameObject(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
{}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& pComponent : m_pComponents) 
		if (pComponent != nullptr) pComponent->Update();
}

void dae::GameObject::FixedUpdate()
{

}

void dae::GameObject::LateUpdate()
{
	for (auto& pComponent : m_pComponents) {
		if (pComponent == nullptr) continue;
		pComponent->LateUpdate();
	}

	// TODO : Move Removal System to own function most likely
	const size_t curSize{ m_pComponents.size() };

	for (size_t index{ }; index < curSize; ++index) {
		const size_t trueIndex{ (curSize - index) - 1 };
		auto& pComponent = m_pComponents[trueIndex];
		
		if (pComponent->IsRemoved()) {
			std::cout << "Component " << trueIndex << " removed.\n";
			m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), pComponent), m_pComponents.end());
		}
	}
}

void dae::GameObject::Render() const
{
	for (auto& pComponent : m_pComponents)
		if (pComponent != nullptr) pComponent->Render(); // if component has a rendering section, it will render using game object transform
}

glm::vec3 dae::GameObject::GetWorldPosition()
{
	if (m_PositionDirty) UpdateWorldPosition();
	
	return m_WorldTransform.GetTranslation();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionDirty) {
		auto& worldPos = m_WorldTransform.GetTranslation();
		auto& localPos = m_LocalTransform.GetTranslation();

		if (m_pParentObject == nullptr) m_WorldTransform.SetTranslation(localPos);
		else m_WorldTransform.SetTranslation(worldPos + localPos);
	}

	m_PositionDirty = false;
}

void dae::GameObject::RemoveComponent(size_t componentIndex)
{
	m_pComponents[componentIndex]->Remove();
}

void dae::GameObject::PrintComponents() const
{
	std::cout << "Components within game object: \n";

	if (m_pComponents.size() > 0) {
		for (size_t index{ }; index < m_pComponents.size(); ++index)
			if (m_pComponents[index] != nullptr) std::cout << "[" << index << "] " << m_pComponents[index]->GetName() << "\n";
	}
	else {
		std::cout << "None\n";
	}
}

// TODO : Test AddChild() (only base adding, no transforming position data)
void dae::GameObject::AddChild(std::shared_ptr<GameObject>& pNewChild)
{
	if (pNewChild == nullptr) {
		std::cerr << "Child that is being attempted to add is nullptr!\n";
		return;
	}
	if (!pNewChild->ValidParent(this)) {
		std::cerr << "Child that is being attempted to add is not valid!\n";
		return;
	}
	
	// TODO : Implement rest of AddChild() (position tranformation data)
	// Dirty pattern (position, scale, rotation)
	

	// Remove from prior parent
	if (pNewChild->m_pParentObject)
		pNewChild->m_pParentObject->m_pChildObjects.erase(std::remove(m_pChildObjects.begin(), m_pChildObjects.end(), pNewChild), m_pChildObjects.end());
	
	// newChild is not owning of self, thus is allowed to be a child
	m_pChildObjects.push_back(pNewChild);
	pNewChild->m_pParentObject = this;

	std::cout << "Child object has been added.\n";
}

// TODO : Test RemoveChild() (only base removing, no transforming position data)
void dae::GameObject::RemoveChild(std::shared_ptr<GameObject>& pChildToRemove)
{
	if (pChildToRemove == nullptr) {
		std::cerr << "Child that is being attempted to remove is nullptr!\n";
		return;
	}

	// TODO : Implement rest of RemoveChild() (Position transformation data)
	
	std::cout << "child use count: " << pChildToRemove.use_count() << "\n";

	// Check it *is* one of the children
	if (std::find(begin(m_pChildObjects), end(m_pChildObjects), pChildToRemove) == end(m_pChildObjects)) {
		std::cerr << "Child that is being attempted to be removed is not within the child vector!\n";
		return;
	}

	// Update position (keep pos)
	

	// Remove from children vector
	m_pChildObjects.erase(std::remove(begin(m_pChildObjects), end(m_pChildObjects), pChildToRemove));

	// Remove self as parent
	pChildToRemove->m_pParentObject = nullptr;

	std::cout << "Child object has been removed.\n";
}

// TODO : Test ValidParent()
bool dae::GameObject::ValidParent(GameObject* pParent)
{
	if (this == pParent) return false;

	auto pParentShared = std::make_shared<GameObject>(pParent); // I dont like this but cant think of any other way to do it 

	// Check within current child objects
 	bool withinChildVector{ std::find(begin(m_pChildObjects), end(m_pChildObjects), pParentShared) != end(m_pChildObjects) };
	if (withinChildVector) return false;

	// Recursively check all the child objects to make sure original parent is not within the children objects
	bool validSoFar{ true }; 
	for (const auto& pChild : m_pChildObjects) {
		if (!validSoFar) break;

		validSoFar = pChild->ValidParent(pParent);
	}

	return validSoFar;
}
