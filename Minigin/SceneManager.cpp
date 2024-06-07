#include "SceneManager.h"
#include "GameObject.h"

void dae::SceneManager::Update()
{
	if (m_pScenes.empty()) return;
	
	m_pScenes[m_ActiveSceneIndex]->Update();
}

void dae::SceneManager::LateUpdate()
{
	if (m_pScenes.empty()) return;

	m_pScenes[m_ActiveSceneIndex]->LateUpdate();
}

void dae::SceneManager::Render()
{
	if (m_pScenes.empty()) return;

	m_pScenes[m_ActiveSceneIndex]->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	m_pScenes.push_back(std::move(std::unique_ptr<Scene>(new Scene(name))));
	return *m_pScenes.back();
}

//dae::Scene& dae::SceneManager::CreateScene(const std::string& name, std::function<void(dae::Scene&, glm::vec2&)> initFunc)
//{
//	m_Scenes.push_back({ std::move(std::unique_ptr<Scene>(new Scene(name))), initFunc });
//	
//	return *std::get<std::unique_ptr<dae::Scene>>(m_Scenes[m_Scenes.size() - 1]).get();
//}

void dae::SceneManager::SetActiveScene(size_t sceneIndex)
{
	if (sceneIndex >= m_pScenes.size()) {
#ifndef NDEBUG
		std::cerr << "Requested scene index is out of range!\n";
#endif
		return;
	}
	else {
		m_ActiveSceneIndex = sceneIndex;
	}
}

void dae::SceneManager::SetActiveScene(const std::string& sceneName)
{
	bool success = false;

	for (size_t index{ }; index < m_pScenes.size(); ++index) {
		const auto& pScene = m_pScenes[index];
		if (pScene && pScene->m_Name == sceneName) {
			m_ActiveSceneIndex = index;
			success = true;
			break;
		}
	}

#ifndef NDEBUG
	if (!success) {
		std::cerr << "Requested scene [" << sceneName << "] does not exist!\n";
	}
#endif
}

bool dae::SceneManager::IsActiveScene(const Scene* pScene) const
{
	if (pScene == m_pScenes[m_ActiveSceneIndex].get()) return true;
	return false;
}
