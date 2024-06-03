#pragma once
#include "Events.h"
#include <vector>

namespace dae
{
	class GameObject;
	class ISubject;

	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(const ISubject* pSubject, Event event) = 0;
	};

	class ISubject
	{
	public:
		~ISubject() = default;
		
		virtual void AddObserver(IObserver* pObserverToAdd) = 0;
		virtual void RemoveObserver(IObserver* pObserverToRemove) = 0;
		virtual void NotifyObservers(Event& eventToNotify) = 0;
		virtual void NotifyObservers(Event&& eventToNotify) = 0;
	};
}

