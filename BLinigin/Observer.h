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
		virtual void OnNotify(const dae::ISubject* pSubject, dae::Event event) = 0;
	};

	class ISubject
	{
	public:
		~ISubject() = default;
		
		virtual void AddObserver(dae::IObserver* pObserverToAdd) = 0;
		virtual void RemoveObserver(dae::IObserver* pObserverToRemove) = 0;
		virtual void NotifyObservers(dae::Event& eventToNotify) = 0;
		virtual void NotifyObservers(dae::Event&& eventToNotify) = 0;
	};
}

