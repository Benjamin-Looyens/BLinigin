#pragma once
#include "Prefab.h"
#include "glm/vec2.hpp"
#include <string>

namespace dae {
	class Dart final : public Prefab
	{
	public:
		GameObject* AddToScene(Scene* pSceneToAddTo) override;

		Dart(float speed, glm::vec2 startPos, glm::vec2 movDir, std::string m_SpriteLocation, bool isFromPlayer = true);

		Dart(const Dart& other) = delete;
		Dart(const Dart&& other) = delete;
		Dart& operator=(const Dart& other) = delete;
		Dart& operator=(const Dart&& other) = delete;

	private:
		bool m_IsFromPlayer;
		float m_Speed;
		glm::vec2 m_Startpos; 
		glm::vec2 m_MovDirection;
		std::string m_SpriteLocation;
	};
}
