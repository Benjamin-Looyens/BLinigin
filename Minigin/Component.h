#pragma once
#include <string>

namespace dae {
	class Transform;

	class Component
	{
	public:
		virtual void Update() = 0;
		virtual void Render(const Transform& transform) const = 0;
		std::string GetName() const { return m_ComponentName; };

		Component() = default;
		virtual ~Component() {};
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		std::string m_ComponentName{ "BaseComponent" };
	};

}

