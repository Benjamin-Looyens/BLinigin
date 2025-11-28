#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "Component.h"

namespace dae
{
	class Texture2D;
	class Scene;

	template<typename ComponentType>
	concept DerivedComponent = requires(ComponentType component) { std::derived_from<ComponentType, Component>; };

	class GameObject final
	{
	public:
		void Update();
		void LateUpdate();
		void Render() const;

		void SetTransform(const Transform& newTransform) { m_LocalTransform = newTransform; };
		void SetLocalTranslation(const float x, const float y);
		void SetLocalTranslation(const glm::vec2& newTranslate) { SetLocalTranslation(newTranslate.x, newTranslate.y); };
		void AddLocalTranslation(const glm::vec2& add);

		void SetLocalScale(const float x, const float y);
		inline void SetLocalScale(glm::vec2 newScale) { SetLocalScale(newScale.x, newScale.y); };
		inline void MultiplyScale(const float multiplier) { SetLocalScale(GetLocalScale() * multiplier); };

		inline glm::vec2 GetLocalPosition() const { return m_LocalTransform.GetTranslation(); };
		inline glm::vec2 GetLocalScale() const { return m_LocalTransform.GetScale(); };

		inline dae::Transform GetLocalTransform() const { return m_LocalTransform; };
		dae::Transform GetWorldTransform();

		glm::vec2 GetWorldPosition();
		glm::vec2 GetWorldScale();

		void UpdateWorldPosition();
		void UpdateWorldScale();

		void RemoveComponent(size_t componentIndex);
		void PrintComponents() const; // for debug

		void Destroy();
		void DestroyWithChildren();
		bool IsDestroyed() const { return m_Destroyed; };

		inline bool IsEnabled() const { return m_Enabled; };
		inline void SetEnabled(bool newEnabled) { m_Enabled = newEnabled; };

		bool IsVisible() const;
		inline void SetVisible(bool newVisible) { m_Visible = newVisible; };

		// Makes the GameObject use the parent as the origin where it moves from; If it has no parent, this value will be ignored
		inline void SetUseParentAsOrigin(const bool newUse) { m_ParentAsOrigin = newUse; }; 
		inline bool GetUseParentAsOrigin() const { return m_ParentAsOrigin; }; 
		// Makes the GameObject use the parent scale as the base scale; If it has no parent, this value will be ignored
		inline void SetApplyParentScaleOnScale(const bool newApply) { m_ApplyParentScale = newApply; };
		inline bool GetApplyParentScaleOnScale() const { return m_ApplyParentScale; };
		// Makes the GameObject apply the parent's scale to their local translation; If it has no parent, this value will be ignored
		inline void SetApplyParentScaleOnTranslation(const bool newApply) { m_ApplyParentScaleOnTranslation = newApply; };
		inline bool GetApplyParentScaleOnTranslation() const { return m_ApplyParentScaleOnTranslation; };

		dae::GameObject* GetChildAtIndex(size_t index) { if (m_pChildObjects.size() <= index) return nullptr; return m_pChildObjects[index]; };

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

		void AddChild(dae::GameObject* pNewChild);
		void RemoveChild(dae::GameObject* pChildToRemove);
		bool ValidParent(dae::GameObject* pParent);

		void SeperateFromParent();
		inline int GetChildCount() const { return static_cast<int>(m_pChildObjects.size()); };
		inline bool HasParent() const { return (m_pParentObject != nullptr); };

		inline dae::Scene* GetContainingScene() const { return m_pContainingScene; };

		GameObject(Scene* pScene, dae::GameObject* pParent = nullptr);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void SetPositionDirty();
		void SetScaleDirty();
		void SetTransformDirty();

		dae::Transform m_LocalTransform{ glm::vec2{}, glm::vec2{}, glm::vec2{ 1.0f, 1.0f } };
		dae::Transform m_WorldTransform{ };

		bool m_PositionDirty{ true };
		bool m_ScaleDirty{ true };

		dae::GameObject* m_pParentObject{};

		std::vector<std::unique_ptr<dae::Component>> m_pComponents{};
		std::vector<dae::GameObject*> m_pChildObjects{};

		bool m_ParentAsOrigin{ true };
		bool m_ApplyParentScale{ true };
		bool m_ApplyParentScaleOnTranslation{ true };

		bool m_Destroyed{ false };
		bool m_Enabled{ true };
		bool m_Visible{ true };

		dae::Scene* m_pContainingScene{};
	};
}
