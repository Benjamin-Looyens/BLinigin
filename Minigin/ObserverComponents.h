#pragma once
#include "Observer.h"
#include "Component.h"

namespace dae {
	class GameObject;
	enum class Event;

	class SubjectComponent : public Component, public ISubject
	{
	public:
		SubjectComponent(GameObject* pParent);
		virtual ~SubjectComponent() = default;

		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(const SubjectComponent&& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(const SubjectComponent&& other) = delete;

		void AddObserver(IObserver* pObserverToAdd) override;
		void RemoveObserver(IObserver* pObserverToRemove) override;
		void NotifyObservers(Event& eventToNotify) override;
		void NotifyObservers(Event&& eventToNotify) override;

	private:
		std::vector<IObserver*> m_pObservers;
	};

	class ObservingComponent : public Component, public IObserver
	{
	public:
		ObservingComponent(GameObject* pParent);
		virtual ~ObservingComponent() = default;

		ObservingComponent(const ObservingComponent& other) = delete;
		ObservingComponent(const ObservingComponent&& other) = delete;
		ObservingComponent& operator=(const ObservingComponent& other) = delete;
		ObservingComponent& operator=(const ObservingComponent&& other) = delete;
	};

	class ObservingSubjectComponent : public Component, public IObserver, public ISubject
	{
	public:
		ObservingSubjectComponent(GameObject* pParent);
		virtual ~ObservingSubjectComponent() = default;

		ObservingSubjectComponent(const ObservingSubjectComponent& other) = delete;
		ObservingSubjectComponent(const ObservingSubjectComponent&& other) = delete;
		ObservingSubjectComponent& operator=(const ObservingSubjectComponent& other) = delete;
		ObservingSubjectComponent& operator=(const ObservingSubjectComponent&& other) = delete;

		void AddObserver(IObserver* pObserverToAdd) override;
		void RemoveObserver(IObserver* pObserverToRemove) override;
		void NotifyObservers(Event& eventToNotify) override;
		void NotifyObservers(Event&& eventToNotify) override;

	private:
		std::vector<IObserver*> m_pObservers;
	};
}