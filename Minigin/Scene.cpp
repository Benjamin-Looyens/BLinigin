#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::RemoveAll()
{
	m_pObjects.clear();
}

void Scene::Update()
{
	for (auto& object : m_pObjects)
	{
		object->Update();
	}
}

void Scene::FixedUpdate()
{
	for (auto& object : m_pObjects)
	{
		object->FixedUpdate();
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& object : m_pObjects)
	{
		object->LateUpdate();
	}

	// NOTE : Maybe move removal System to own function most likely
	const size_t curSize{ m_pObjects.size() };

	for (size_t index{ }; index < curSize; ++index) {
		const size_t trueIndex{ (curSize - index) - 1 };
		auto& pObject = m_pObjects[trueIndex];

		if (pObject->IsDestroyed()) {
			std::cout << "Component " << trueIndex << " removed.\n";
			m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pObject), m_pObjects.end());
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

GameObject* dae::Scene::AddGameObject()
{
	m_pObjects.push_back(std::make_unique<GameObject>());

	return m_pObjects[m_pObjects.size() - 1].get();
}

void dae::Scene::RemoveObject(size_t objectIndex)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), m_pObjects[objectIndex]), m_pObjects.end());
}

void dae::Scene::RemoveObject(GameObject* pObject)
{
	for (auto& pContainerObject : m_pObjects) {
		if (pContainerObject.get() == pObject) {
			m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pContainerObject), m_pObjects.end());
			break;
		}
	}
}

