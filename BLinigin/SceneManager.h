#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"
#include <functional>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		dae::Scene& CreateScene(const std::string& name);
		//dae::Scene& CreateScene(const std::string& name, std::function<void(dae::Scene&, glm::vec2&)> initFunc);

		void SetActiveScene(size_t sceneIndex);
		void SetActiveScene(const std::string& sceneName);

		bool IsActiveScene(const dae::Scene* pScene) const;

		void Update();
		void LateUpdate();
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<dae::Scene>> m_pScenes;
		//std::vector<std::pair<std::unique_ptr<dae::Scene>, std::function<void(dae::Scene&, glm::vec2&)>>> m_Scenes;

		size_t m_ActiveSceneIndex{};
	};
}

#define BL_SCENE_MANAGER dae::SceneManager::GetInstance()