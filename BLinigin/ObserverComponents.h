#pragma once
#include "Observer.h"
#include "Component.h"

namespace dae {
	class GameObject;
	enum class Event;

	class SubjectComponent : public Component, public ISubject
	{
	public:
		SubjectComponent(dae::GameObject* pParent);
		virtual ~SubjectComponent() = 0;

		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(const SubjectComponent&& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(const SubjectComponent&& other) = delete;

		void AddObserver(IObserver* pObserverToAdd) override;
		void RemoveObserver(IObserver* pObserverToRemove) override;
		void NotifyObservers(dae::Event& eventToNotify) override;
		void NotifyObservers(dae::Event&& eventToNotify) override;

	private:
		std::vector<dae::IObserver*> m_pObservers;
	};

	class ObservingComponent : public Component, public IObserver
	{
	public:
		ObservingComponent(dae::GameObject* pParent);
		virtual ~ObservingComponent() = 0;

		ObservingComponent(const ObservingComponent& other) = delete;
		ObservingComponent(const ObservingComponent&& other) = delete;
		ObservingComponent& operator=(const ObservingComponent& other) = delete;
		ObservingComponent& operator=(const ObservingComponent&& other) = delete;
	};

	class ObservingSubjectComponent : public Component, public IObserver, public ISubject
	{
	public:
		ObservingSubjectComponent(dae::GameObject* pParent);
		virtual ~ObservingSubjectComponent() = 0;

		ObservingSubjectComponent(const ObservingSubjectComponent& other) = delete;
		ObservingSubjectComponent(const ObservingSubjectComponent&& other) = delete;
		ObservingSubjectComponent& operator=(const ObservingSubjectComponent& other) = delete;
		ObservingSubjectComponent& operator=(const ObservingSubjectComponent&& other) = delete;

		void AddObserver(IObserver* pObserverToAdd) override;
		void RemoveObserver(IObserver* pObserverToRemove) override;
		void NotifyObservers(dae::Event& eventToNotify) override;
		void NotifyObservers(dae::Event&& eventToNotify) override;

	private:
		std::vector<dae::IObserver*> m_pObservers;
	};
}