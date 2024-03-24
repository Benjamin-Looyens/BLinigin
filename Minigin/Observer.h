#pragma once
#include "Events.h"

namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() {};
		virtual void OnNotify(const GameObject* subjectObject, Event event) = 0;
	};
}

