#pragma once
#include <vector>

namespace dae {
	class GameObject;
	class Scene;

	class Prefab
	{
	public:
		virtual GameObject* AddToScene(Scene* pSceneToAddTo) = 0;

		Prefab() = default;
		virtual ~Prefab() = default;
		
		// TODO : Verify rule of 5 here
		//Prefab(const Prefab& other) = delete;
		//Prefab(const Prefab&& other) = delete;
		//Prefab& operator=(const Prefab& other) = delete;
		//Prefab& operator=(const Prefab&& other) = delete;
	};
}