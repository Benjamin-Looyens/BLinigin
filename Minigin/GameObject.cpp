#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
//#include "DerivedComponents.h"
#include <iostream>
#include <algorithm>
#include "Scene.h"

dae::GameObject::GameObject(Scene* pScene, GameObject* pParentObject)
	: m_pContainingScene{ pScene }, m_pParentObject { pParentObject }
{}

dae::GameObject::~GameObject() = default;

void dae::GameObject::SetPositionDirty()
{
	m_PositionDirty = true;

	for (auto& childObject : m_pChildObjects)
		childObject->SetPositionDirty();
}

void dae::GameObject::SetScaleDirty()
{
	m_ScaleDirty = true;

	for (auto& childObject : m_pChildObjects)
		childObject->SetScaleDirty();
}

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

	// TODO : Refine component removal system & also fix it cause seems kinda broken
	// Remove components marked for remove
	const size_t curSize{ m_pComponents.size() };

	for (size_t index{ }; index < curSize; ++index) {
		const size_t trueIndex{ (curSize - index) - 1 };
		auto& pComponent = m_pComponents[trueIndex];
		
		if (pComponent->IsRemoved()) {
#ifndef NDEBUG
			std::cout << "Component [" << trueIndex << "] " << pComponent->GetName() << " removed.\n";
#endif 
			m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), pComponent), m_pComponents.end());
		}
	}
}

void dae::GameObject::Render() const
{
	for (auto& pComponent : m_pComponents)
		if (pComponent != nullptr) pComponent->Render(); // if component has a rendering section, it will render using game object transform
}

void dae::GameObject::SetLocalTranslation(const float x, const float y)
{
	m_LocalTransform.SetTranslation(x, y);
	
	SetPositionDirty();
}

void dae::GameObject::AddLocalTranslation(const glm::vec2& add)
{
	m_LocalTransform.AddTranslation(add);

	SetPositionDirty();
}

void dae::GameObject::SetLocalScale(const float x, const float y)
{
	m_LocalTransform.SetScale(x, y);

	SetScaleDirty();
}

dae::Transform dae::GameObject::GetWorldTransform()
{
	if (m_PositionDirty) UpdateWorldPosition();
	if (m_ScaleDirty) UpdateWorldScale();
	
	return m_WorldTransform;
}

glm::vec2 dae::GameObject::GetWorldPosition()
{
	if (m_PositionDirty) UpdateWorldPosition();
	
	return m_WorldTransform.GetTranslation();
}

glm::vec2 dae::GameObject::GetWorldScale()
{
	if (m_ScaleDirty) UpdateWorldScale();

	return m_WorldTransform.GetScale();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionDirty) {
		auto& localPos = m_LocalTransform.GetTranslation();

		if (m_pParentObject == nullptr) m_WorldTransform.SetTranslation(localPos);
		else m_WorldTransform.SetTranslation(m_pParentObject->GetWorldTransform().GetTranslation() + localPos);
	}

	m_PositionDirty = false;
}

void dae::GameObject::UpdateWorldScale()
{
	if (m_ScaleDirty) {
		auto& localScale = m_LocalTransform.GetScale();

		if (m_pParentObject == nullptr) m_WorldTransform.SetScale(localScale);
		else {
			auto& parentScale = m_pParentObject->GetWorldTransform().GetScale();
			glm::vec2 newScale{ localScale };

			if (parentScale.x != 0.0f) newScale.x *= parentScale.x;
			if (parentScale.y != 0.0f) newScale.y *= parentScale.y;

			m_WorldTransform.SetScale(newScale);
		}
	}

	m_ScaleDirty = false;
}

void dae::GameObject::RemoveComponent(size_t componentIndex)
{
	m_pComponents[componentIndex]->Remove();
}

void dae::GameObject::PrintComponents() const
{
#ifndef NDEBUG
	std::cout << "Components within game object: \n";

	if (m_pComponents.size() > 0) {
		for (size_t index{ }; index < m_pComponents.size(); ++index)
			//if (m_pComponents[index] != nullptr) std::cout << "[" << index << "] " << typeid(m_pComponents[index].get()).name() << "\n";
			if (m_pComponents[index] != nullptr) std::cout << "[" << index << "] " << m_pComponents[index]->GetName() << "\n";
	}
	else {
		std::cout << "None\n";
	}
#endif 
}

void dae::GameObject::AddChild(GameObject* pNewChild)
{
	if (pNewChild == nullptr) {
		std::cerr << "Child that is being attempted to add is nullptr!\n";
		return;
	}
	if (!pNewChild->ValidParent(this)) {
		std::cerr << "Child that is being attempted to add is not valid!\n";
		return;
	}
	
	// Dirty pattern (position, scale, rotation)
	auto currentPos = GetWorldPosition();
	auto newChildCurrentPos = pNewChild->GetWorldPosition();
	pNewChild->SetLocalTranslation(newChildCurrentPos - currentPos);

	// Remove from prior parent
	if (pNewChild->HasParent())
		pNewChild->m_pParentObject->RemoveChild(pNewChild);
	
	// newChild is not owning of self, thus is allowed to be a child
	m_pChildObjects.push_back(pNewChild);
	pNewChild->m_pParentObject = this;

#ifndef NDEBUG
	std::cout << "Child object has been added.\n";
#endif 
}

void dae::GameObject::RemoveChild(GameObject* pChildToRemove)
{
	if (pChildToRemove == nullptr) {
		std::cerr << "Child that is being attempted to remove is nullptr!\n";
		return;
	}

	// Check it *is* one of the children
	if (std::find(begin(m_pChildObjects), end(m_pChildObjects), pChildToRemove) == end(m_pChildObjects)) {
		std::cerr << "Child that is being attempted to be removed is not within the child vector!\n";
		return;
	}

	// Update position (keep pos)
	pChildToRemove->SetLocalTranslation(pChildToRemove->GetWorldPosition());
	pChildToRemove->m_PositionDirty = true;

	// Remove from children vector
	m_pChildObjects.erase(std::remove(begin(m_pChildObjects), end(m_pChildObjects), pChildToRemove));

	// Remove self as parent
	pChildToRemove->m_pParentObject = nullptr;

#ifndef NDEBUG
	std::cout << "Child object has been removed.\n";
#endif 
}

bool dae::GameObject::ValidParent(GameObject* pParent)
{
	if (this == pParent) return false;

	//auto pParentShared = std::make_shared<GameObject>(pParent); // I dont like this but cant think of any other way to do it 

	// Check within current child objects
 	bool withinChildVector{ std::find(begin(m_pChildObjects), end(m_pChildObjects), pParent) != end(m_pChildObjects) };
	if (withinChildVector) return false;

	// Recursively check all the child objects to make sure original parent is not within the children objects
	bool validSoFar{ true }; 
	for (const auto& pChild : m_pChildObjects) {
		if (!validSoFar) break;

		validSoFar = pChild->ValidParent(pParent);
	}

	return validSoFar;
}

void dae::GameObject::SeperateFromParent()
{
	if (m_pParentObject == nullptr) {
		std::cerr << "No parent to seperate from.\n";
		return;
	}

	// HACK : This implementation is pretty scuffed
	for (auto& object : m_pParentObject->m_pChildObjects) {
		if (object == this) {
			m_pParentObject->RemoveChild(object);
		}
	}
}
