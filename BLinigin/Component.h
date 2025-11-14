#pragma once
#include <string>
#include <memory>
#include <assert.h>

namespace dae {
	class Transform;
	class GameObject;

	class Component
	{
		//friend Component* GameObject::AddComponent(); // TODO : Find maintainable method to creating components *only* within GameObject::AddComponent()

	public:
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const {};

		void Remove() { m_Removed = true; };
		inline bool IsRemoved() const { return m_Removed; };
		inline const GameObject* GetParent() const { return m_pParent; };
		inline const std::string GetName() const { return typeid(*this).name(); };

		//Component() = default;
		Component(GameObject* pParent) {
			m_pParent = pParent;
			assert(pParent && "Component was created with parent = nullptr (parent does not exist).");
		};
		virtual ~Component() = 0;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		bool m_Removed{ false };

		// NOTE : Potentially change the pParent to a private getter rather than protected so that derived components cannot remove their parent for example
		GameObject* m_pParent;
	};

	inline Component::~Component() {};
}