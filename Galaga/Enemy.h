#pragma once
#include "Prefab.h"
#include "glm/vec2.hpp"
#include <string>
#include "SoundSystem.h"

namespace dae {
	class IObserver;

	struct EnemyData
	{
		glm::vec2 startPos;
		std::string spriteLocation;
		float movSpeed;
	};

	class Enemy final : public Prefab
	{
	public:
		Enemy(glm::vec2& startPos, std::string& spriteLocation, float& spawnMovSpeed, float& patrolSpeed, std::vector<IObserver*>& observers, float& animFPS, int& animFrames, int& health, sound_id& damageSoundID, sound_id& deathSoundID);
		Enemy(glm::vec2&& startPos, std::string&& spriteLocation, float&& spawnMovSpeed, float&& patrolSpeed, std::vector<IObserver*>& observers, float&& animFPS, int&& animFrames, int&& health, sound_id&& damageSoundID, sound_id&& deathSoundID);

		GameObject* AddToScene(Scene* pSceneToAddTo) override;

		void SetObservers(std::vector<IObserver*> newObservers) { m_Observers = newObservers; };
		std::vector<IObserver*> GetObservers() const { return m_Observers; };

		// TODO : Verify rule of 5 here & in base class
		//Enemy(const Enemy& other) = delete;
		//Enemy(const Enemy&& other) = delete;
		//Enemy& operator=(const Enemy& other) = delete;
		//Enemy& operator=(const Enemy&& other) = delete;

	private:
		glm::vec2 m_StartPos;
		std::string m_SpriteLocation;
		float m_SpawnMovSpeed;
		float m_PatrolMovSpeed;
		std::vector<IObserver*> m_Observers; 

		float m_AnimFPS;
		int m_AnimFrames;
		int m_Health;

		sound_id m_DamageSound;
		sound_id m_DeathSound;
	};
}


