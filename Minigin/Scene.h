#pragma once
#include "SceneManager.h"
//#include "DerivedObservers.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class Observer;

	template<typename ObserverType>
	concept DerivedObserver = requires(ObserverType observer) { std::derived_from<ObserverType, Observer>; };

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

		template<DerivedObserver ObserverType, typename... TArgs>
		ObserverType* AddObserver(const TArgs&... args) {
			m_pObservers.emplace_back(std::make_unique<ObserverType>(args...));

			return dynamic_cast<ObserverType*>(m_pObservers[m_pObservers.size() - 1].get());
		};

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <std::unique_ptr<GameObject>> m_pObjects{};
		std::vector<std::unique_ptr<Observer>> m_pObservers{};

		static unsigned int m_idCounter; 
	};

}
