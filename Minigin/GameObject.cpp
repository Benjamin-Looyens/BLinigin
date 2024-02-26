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
	for (auto pComponent : m_Components) 
		if (pComponent != nullptr) pComponent->Update();
}

void dae::GameObject::FixedUpdate()
{

}

void dae::GameObject::Render() const
{
	for (auto pComponent : m_Components)
		if (pComponent != nullptr) pComponent->Render(m_transform); // if component has a rendering section, it will render using game object transform
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::shared_ptr<Component> newComponent)
{
	m_Components.push_back(newComponent);
	
	//auto size{ m_Components.size() };
	//
	//m_Components.resize(size + 1);
	//m_Components[size] = newComponent;
}

void dae::GameObject::RemoveComponent(size_t componentIndex)
{
	m_Components[componentIndex].reset();
}

std::shared_ptr<dae::Component> dae::GameObject::GetComponent(size_t componentIndex)
{
	return m_Components[componentIndex];
}

void dae::GameObject::PrintComponents() const
{
	std::cout << "Components within game object: \n";
	for (size_t index{ }; index < m_Components.size(); ++index)
		if (m_Components[index] != nullptr) std::cout << "[" << index << "] " << m_Components[index]->GetName() << "\n";
}
