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

		Prefab(const Prefab& other) = delete;
		Prefab(const Prefab&& other) = delete;
		Prefab& operator=(const Prefab& other) = delete;
		Prefab& operator=(const Prefab&& other) = delete;
	};
}