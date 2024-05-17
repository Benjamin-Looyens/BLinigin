#include "SceneManager.h"
#include "GameObject.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_pScenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::FixedUpdate()
{
	for (auto& scene : m_pScenes)
	{
		scene->FixedUpdate();
	}
}

void dae::SceneManager::LateUpdate()
{
	for (auto& scene : m_pScenes)
	{
		scene->LateUpdate();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_pScenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	m_pScenes.push_back(std::move(std::unique_ptr<Scene>(new Scene(name))));
	return *m_pScenes.back();
}
