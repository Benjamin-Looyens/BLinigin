#pragma once
#include <memory>
#include "Transform.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class Component;

	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void SetPosition(float x, float y);

		void AddComponent(std::shared_ptr<Component> newComponent);
		void RemoveComponent(size_t componentIndex);
		std::shared_ptr<Component> GetComponent(size_t componentIndex);
		void PrintComponents() const; // for debug

		void Destroy() { m_Destroyed = true; };

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{ };

		std::vector <std::shared_ptr<Component>> m_Components;

		bool m_Destroyed{ false };
	};
}
