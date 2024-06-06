#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>
#include "Observer.h"
#include "ObserverComponents.h"

namespace dae {
	// Class Forward Declarations
	class Font;
	class Texture2D;
	class IObserver;
	enum class Event;
	class IObserver;
	class ISubject;
	class TextComponent;

	// FPS Component
	class FPSComponent final : public Component
	{
	public:
		void Update() override;

		FPSComponent(dae::GameObject* pParent, float displayUpdateInterval = 0.25f);
		~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

	private:
		dae::TextComponent* m_pTextLocation; // non-owning reference to parent's text component (if it has one)

		float m_CounterInterval;
		float m_Counter{};
	};

	class WaveMovementYComponent final : public Component
	{
	public:
		void Update() override;

		WaveMovementYComponent(GameObject* pParent, float waveHeight = 10.0f, float waveSpeed = 1.0f);
		~WaveMovementYComponent() = default;
		WaveMovementYComponent(const WaveMovementYComponent& other) = delete;
		WaveMovementYComponent(const WaveMovementYComponent&& other) = delete;
		WaveMovementYComponent& operator=(const WaveMovementYComponent& other) = delete;
		WaveMovementYComponent& operator=(const WaveMovementYComponent&& other) = delete;

	private:
		glm::vec2 m_Origin;

		float m_WaveHeight;
		float m_WaveSpeed;

		float m_WaveTime{ 0.0f };
	};

	class DisplayDataComponent final : public Component
	{
	public:
		void UpdateDisplay(std::string dataStr);

		DisplayDataComponent(GameObject* pParent, std::string dataPrefix, std::string startData = "");
		~DisplayDataComponent() = default;
		DisplayDataComponent(const DisplayDataComponent& other) = delete;
		DisplayDataComponent(const DisplayDataComponent&& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent&& other) = delete;

	private:
		dae::TextComponent* m_pTextLocation; // non-owning reference to parent's text component (if it has one)
		std::string m_DataPrefix;
	};

	class FollowMouseComponent final : public Component
	{
	public:
		FollowMouseComponent(GameObject* pParent);

		void Update() override;
	};

	class ConstantMovementComponent final : public Component
	{
	public:
		ConstantMovementComponent(GameObject* pParent, float speed, glm::vec2 movDir);

		void Update() override;

	private:
		float m_Speed;
		glm::vec2 m_MovDir;
	};

	class TimedDestroyComponent final : public Component
	{
	public:
		TimedDestroyComponent(GameObject* pParent, float timeTillDestroy);
		void Update() override;

	private:
		float m_ElapsedTime{};
		float m_TimeTillDestroy;
	};

	class IncrementingScaleComponent final : public Component
	{
	public:
		IncrementingScaleComponent(GameObject* pParent, float incrementPerSecond = 1.0f);

		void Update() override;

	private:
		float m_IncrPerSecond;
	};

}

