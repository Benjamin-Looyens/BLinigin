#include "ObserverComponents.h"
#include <iostream>
#include "GameObject.h"

#pragma region SubjectComponent
dae::SubjectComponent::SubjectComponent(GameObject* pParent)
	: Component(pParent)
{}

dae::SubjectComponent::~SubjectComponent()
{}

void dae::SubjectComponent::AddObserver(IObserver* pObserverToAdd)
{
	m_pObservers.push_back(pObserverToAdd);
}

void dae::SubjectComponent::RemoveObserver(IObserver* pObserverToRemove)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserverToRemove), m_pObservers.end());
}

void dae::SubjectComponent::NotifyObservers(Event& eventToNotify)
{
	for (auto& pObserver : m_pObservers)
		pObserver->OnNotify(dynamic_cast<ISubject*>(this), eventToNotify);
}

void dae::SubjectComponent::NotifyObservers(Event&& eventToNotify)
{
	for (auto& pObserver : m_pObservers)
		pObserver->OnNotify(dynamic_cast<ISubject*>(this), eventToNotify);
}
#pragma endregion SubjectComponent

#pragma region ObservingComponent
dae::ObservingComponent::ObservingComponent(GameObject* pParent)
	: Component(pParent)
{}
dae::ObservingComponent::~ObservingComponent()
{}
#pragma endregion ObservingComponent

#pragma region ObservingSubjectComponent
dae::ObservingSubjectComponent::ObservingSubjectComponent(GameObject* pParent)
	: Component(pParent)
{}

dae::ObservingSubjectComponent::~ObservingSubjectComponent()
{}

void dae::ObservingSubjectComponent::AddObserver(IObserver* pObserverToAdd)
{
	m_pObservers.push_back(pObserverToAdd);
}

void dae::ObservingSubjectComponent::RemoveObserver(IObserver* pObserverToRemove)
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserverToRemove), m_pObservers.end());
}

void dae::ObservingSubjectComponent::NotifyObservers(Event& eventToNotify)
{
	for (auto& pObserver : m_pObservers)
		pObserver->OnNotify(dynamic_cast<ISubject*>(this), eventToNotify);
}

void dae::ObservingSubjectComponent::NotifyObservers(Event&& eventToNotify)
{
	for (auto& pObserver : m_pObservers)
		pObserver->OnNotify(dynamic_cast<ISubject*>(this), eventToNotify);
}
#pragma endregion ObservingSubjectComponent
