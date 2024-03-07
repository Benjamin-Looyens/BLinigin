#pragma once
#include <string>
#include <memory>

namespace dae {
	class Transform;
	class GameObject;

	class Component
	{
	public:
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const {};
		std::string GetName() const { return m_ComponentName; };

		void Remove() { m_Removed = true; };
		bool IsRemoved() { return m_Removed; };
		const GameObject* GetParent() const { return m_pParent; };

		//Component() = default;
		Component(const GameObject* parent) { m_pParent = parent; }; // HACK : Maybe try to use not raw pointer
		virtual ~Component() {};
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		std::string m_ComponentName{ "BaseComponent" };
	
		bool m_Removed{ false };

		const GameObject* m_pParent;
	};

}

