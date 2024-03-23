#pragma once
#include "Events.h"

namespace dae
{
	class GameObject;
	enum class Event;

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() {};
		virtual void OnNotify(const GameObject* observedObject, Event event) = 0;
	};
}

