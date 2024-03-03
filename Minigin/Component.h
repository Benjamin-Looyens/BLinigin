#pragma once
#include <string>
#include <memory>

namespace dae {
	class Transform;
	class GameObject;

	class Component
	{
	public:
		virtual void Update() = 0;
		virtual void Render(const Transform& transform) const = 0;
		std::string GetName() const { return m_ComponentName; };

		void Remove() { m_Removed = true; };
		bool IsRemoved() { return m_Removed; };

		//Component() = default;
		Component(const GameObject* parent) { m_Parent = parent; }; // HACK : Maybe try to use not raw pointer
		virtual ~Component() {};
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		std::string m_ComponentName{ "BaseComponent" };
	
		bool m_Removed{ false };

	private:
		const GameObject* m_Parent;
	};

}

