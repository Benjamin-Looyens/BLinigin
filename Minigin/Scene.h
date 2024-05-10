#pragma once
#include "SceneManager.h"
#include "Observer.h"

namespace dae
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void RemoveAll();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		GameObject* AddGameObject();
		void RemoveObject(size_t objectIndex);
		void RemoveObject(GameObject* pObject);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <std::unique_ptr<GameObject>> m_pObjects{};

		static unsigned int m_idCounter; 
	};

}
