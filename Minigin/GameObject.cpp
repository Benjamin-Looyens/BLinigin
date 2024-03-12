#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"
#include "DerivedComponents.h"
#include <iostream>

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
	// TODO : implement late update (destroyed, dirty, etc)
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

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
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
