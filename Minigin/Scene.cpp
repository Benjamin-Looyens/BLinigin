#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_pObjects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_pObjects.erase(std::remove(m_pObjects.begin(), m_pObjects.end(), object), m_pObjects.end());
}

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
}

void Scene::Render() const
{
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

