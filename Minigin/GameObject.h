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

	// TODO : Potentially refactor the concept and template setup here
	template<typename ComponentType>
	concept DerivedComponent = requires(ComponentType component) { std::derived_from<ComponentType, Component>; };

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void SetTransform(const Transform& newTransform) { m_LocalTransform = newTransform; };
		void SetTranslation(float x, float y, float z = 0.0f) { m_LocalTransform.SetTranslation(x, y, z); };
		void SetTranslation(const glm::vec3& newTranslate) { m_LocalTransform.SetTranslation(newTranslate); };

		Transform GetLocalTransform() const { return m_LocalTransform; };
		glm::vec3 GetLocalTranslation() const { return m_LocalTransform.GetLocalTranslation(); };

		void RemoveComponent(size_t componentIndex);
		void PrintComponents() const; // for debug

		void Destroy() { m_Destroyed = true; };
		bool IsDestroyed() { return m_Destroyed; };

		void SetUseParentWorld(const bool newUse) { m_UseParentWorld = newUse; };

#pragma region TemplateFunctions
		template<DerivedComponent ComponentType, typename... TArgs>
		void AddComponent(const TArgs&... args) { m_pComponents.emplace_back(std::make_unique<ComponentType>(this, args...)); };

		template<DerivedComponent ComponentType>
		ComponentType* GetComponent() const {
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

#pragma region ChildObjectFunctions
		void AddChild(std::shared_ptr<GameObject>& pNewChild);
		void RemoveChild(std::shared_ptr<GameObject>& pChildToRemove);
		bool ValidParent(GameObject* pParent);
		int GetChildCount() { return static_cast<int>(m_pChildObjects.size()); };
#pragma endregion ChildObjectFunctions

		GameObject(GameObject* pParent = nullptr);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_LocalTransform{ };
		Transform m_WorldTransform{ };

		bool m_TranslationDirty{ false };
		bool m_RotationDirty{ false };
		bool m_ScaleDirty{ false };

		GameObject* m_pParentObject;

		std::vector<std::unique_ptr<Component>>	 m_pComponents;
		std::vector<std::shared_ptr<GameObject>> m_pChildObjects; // HACK : should I use shared_ptr ?

		bool m_UseParentWorld{ true };

		bool m_Destroyed{ false };
	};
}
