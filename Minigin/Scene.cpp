#include "Scene.h"
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
	//const size_t curSize{ m_pObjects.size() };
	//
	//for (size_t index{ }; index < curSize; ++index) {
	//	const size_t trueIndex{ (curSize - index) - 1 };
	//	auto& pObject = m_pObjects[trueIndex];
	//
	//	if (pObject->IsDestroyed()) {
	//		std::cout << "Object " << trueIndex << " removed.\n";
	//		m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), pObject), m_pObjects.end());
	//	}
	//}

	m_pObjects.erase(std::remove_if(begin(m_pObjects), end(m_pObjects), [](const std::unique_ptr<GameObject>& obj) { return obj->IsDestroyed(); }), m_pObjects.end());

	if (!m_pAddedObjects.empty()) {
		for (auto& pObject : m_pAddedObjects) {
			m_pObjects.push_back(std::move(pObject));
		}

		m_pAddedObjects.clear();
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
	m_pAddedObjects.push_back(std::make_unique<GameObject>(this));

	return m_pAddedObjects[m_pAddedObjects.size() - 1].get();
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

std::vector<GameObject*> dae::Scene::GetAllObjects()
{
	std::vector<GameObject*> pRetObjects{};
	pRetObjects.reserve(m_pObjects.size());

	for (auto& pObject : m_pObjects) {
		pRetObjects.push_back(pObject.get());
	}

	return pRetObjects;
}

