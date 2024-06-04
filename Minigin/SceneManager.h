#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void SetActiveScene(size_t sceneIndex);
		void SetActiveScene(const std::string& sceneName);

		bool IsActiveScene(const Scene* pScene) const;

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_pScenes;

		size_t m_ActiveSceneIndex{};
	};
}

#define BL_SCENE_MANAGER dae::SceneManager::GetInstance()