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
	class Scene;

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
		void SetLocalTranslation(const float x, const float y);
		void SetLocalTranslation(const glm::vec2& newTranslate) { SetLocalTranslation(newTranslate.x, newTranslate.y); };
		void AddLocalTranslation(const glm::vec2& add);

		void SetLocalScale(const float x, const float y);
		void SetLocalScale(glm::vec2 newScale) { SetLocalScale(newScale.x, newScale.y); };
		void MultiplyScale(const float multiplier) { m_LocalTransform.MultiplyScale(multiplier); };

		glm::vec2 GetLocalPosition() const { return m_LocalTransform.GetTranslation(); };
		glm::vec2 GetLocalScale() const { return m_LocalTransform.GetScale(); };

		Transform GetLocalTransform() const { return m_LocalTransform; };
		Transform GetWorldTransform();

		glm::vec2 GetWorldPosition();
		glm::vec2 GetWorldScale();

		void UpdateWorldPosition();
		void UpdateWorldScale();

		void RemoveComponent(size_t componentIndex);
		void PrintComponents() const; // for debug

		void Destroy() { m_Destroyed = true; };
		bool IsDestroyed() { return m_Destroyed; };

		void SetUseParentWorld(const bool newUse) { m_UseParentWorld = newUse; };

		GameObject* GetChildAtIndex(size_t index) { if (m_pChildObjects.size() <= index) return nullptr; return m_pChildObjects[index]; };

#pragma region TemplateFunctions
		template<DerivedComponent ComponentType, typename... TArgs>
		ComponentType* AddComponent(const TArgs&... args) {
			m_pComponents.emplace_back(std::make_unique<ComponentType>(this, args...));

			return dynamic_cast<ComponentType*>(m_pComponents[m_pComponents.size() - 1].get());
		};

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

		void AddChild(GameObject* pNewChild);
		void RemoveChild(GameObject* pChildToRemove);
		bool ValidParent(GameObject* pParent);
		void SeperateFromParent();
		int GetChildCount() const { return static_cast<int>(m_pChildObjects.size()); };
		bool HasParent() const { return (m_pParentObject != nullptr); };
		dae::Scene* GetContainingScene() { return m_pContainingScene; };

		GameObject(Scene* pScene, GameObject* pParent = nullptr);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void SetPositionDirty();
		void SetScaleDirty();

		Transform m_LocalTransform{ };
		Transform m_WorldTransform{ };

		bool m_PositionDirty{ true };
		bool m_ScaleDirty{ true };

		GameObject* m_pParentObject;

		std::vector<std::unique_ptr<Component>>	 m_pComponents;
		std::vector<GameObject*> m_pChildObjects;

		bool m_UseParentWorld{ true };

		bool m_Destroyed{ false };

		dae::Scene* m_pContainingScene;
	};
}
