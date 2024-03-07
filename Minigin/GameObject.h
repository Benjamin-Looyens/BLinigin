#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <iostream>
#include <algorithm>

namespace dae
{
	class Texture2D;
	class Component;

	// TODO : Potentially differentiate more the concept and template setup here
	template<typename ComponentType>
	concept DerivedComponent = requires(ComponentType component) { std::derived_from<ComponentType, Component>; };

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y);

		void RemoveComponent(size_t componentIndex);
		void PrintComponents() const; // for debug

		void Destroy() { m_Destroyed = true; };

		Transform GetTransform() const { return m_Transform; };

#pragma region TemplateFunctions
		template<DerivedComponent ComponentType, typename... TArgs>
		void AddComponent(const TArgs&... args) { m_pComponents.emplace_back(std::make_unique<ComponentType>(this, args...)); };

		template<DerivedComponent ComponentType>
		ComponentType* GetComponent() {
			for (auto&& rrComponent : m_pComponents) {
				ComponentType* pTypeCasted{ dynamic_cast<ComponentType*>(rrComponent.get()) };
				
				if (pTypeCasted) return pTypeCasted;
			}
			
			return nullptr;
		}

		template<DerivedComponent ComponentType>
		void RemoveComponent() {
			ComponentType* pComponent{ GetComponent<ComponentType>() };

			if (pComponent) pComponent->Remove();
			else std::cerr << "GameObject does not contain specified component type (" << typeid(ComponentType).name() << ").\n";
		};

		template<DerivedComponent ComponentType>
		bool HasComponent() { return GetComponent<ComponentType>() != nullptr; }
#pragma endregion TemplateFunctions

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform{ };

		std::vector <std::unique_ptr<Component>> m_pComponents;

		bool m_Destroyed{ false };
	};
}
