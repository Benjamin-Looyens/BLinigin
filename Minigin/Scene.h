#pragma once
#include "GameObject.h"

namespace dae
{
	class SceneManager;

	class Scene final
	{
		friend class SceneManager; 
	public:
		void RemoveAll();

		// TODO : Make it so that only SceneManager can call update, render, etc
		void Update();
		void LateUpdate();
		void Render() const;

		dae::GameObject* AddGameObject();
		void RemoveObject(size_t objectIndex);
		void RemoveObject(dae::GameObject* pObject);

		std::vector<dae::GameObject*> GetAllObjects();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <std::unique_ptr<dae::GameObject>> m_pObjects{};
		std::vector <std::unique_ptr<dae::GameObject>> m_pAddedObjects{};

		static unsigned int m_idCounter; 
	};

}
