#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>
#include "Observer.h"
#include "ObserverComponents.h"

// TODO : Check & remove virtual where unnecessary (eg: virtual destructors in final)

namespace dae {
	// Class Forward Declarations
	class Font;
	class Texture2D;
	class IObserver;
	enum class Event;
	class IObserver;
	class ISubject;

	// TODO : Merge Text, Texture components & Move them to Engine Files
	// Text Component
	class TextComponent final : public Component
	{
	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);

		TextComponent(dae::GameObject* pParent, const std::string& text, Font* font, const std::string& internalIdentifier);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		const std::string m_ResourceFile;
		Font* m_pFont; // TODO: unique
		Texture2D* m_pTextTexture; // TODO: unique
	};

	// Texture Component
	class TextureComponent final : public Component
	{
	public:
		virtual void Render() const override;

		void SetTexture(const std::string& filename);
		Texture2D* GetTexture() const { return m_pTexture; };

		TextureComponent(dae::GameObject* pParent, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		float GetTextureWidth() const;
		float GetTextureHeight() const;
		glm::vec2 GetTextureSize() const;

	private:
		Texture2D* m_pTexture; // TODO: Change to unique ptr
	};

	// FPS Component
	class FPSComponent final : public Component
	{
	public:
		virtual void Update() override;

		FPSComponent(dae::GameObject* pParent, float displayUpdateInterval = 0.25f);
		virtual ~FPSComponent() = default;
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
		virtual void Update() override;

		WaveMovementYComponent(GameObject* pParent, float waveHeight = 10.0f, float waveSpeed = 1.0f);
		virtual ~WaveMovementYComponent() = default;
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

	class LivesComponent final : public Component
	{
	public:
		void LoseLife();
		void GainLife();

		int GetLifeCount() const { return m_NumLives; };

		void Render() const override;
		void SetLifeTexture(Texture2D* pNewTexture);

		LivesComponent(GameObject* pParent, int lives, glm::vec2 displayPosition = { 0, 0 }, glm::vec2 livesStackDirection = { 1, 0 });
		virtual ~LivesComponent() = default;
		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(const LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(const LivesComponent&& other) = delete;

	private:
		int m_NumLives;
		
		glm::vec2 m_DisplayPos;
		glm::vec2 m_LivesStackDir;

		Texture2D* m_pLifeTexture;

		static constexpr float s_SPACER_BETWEEN_LIVES{ 5.0f };
	};

	class ScoreComponent final : public Component
	{
	public:
		int GetScore() const { return m_Score; };
		void AddPoints(int pointsToAdd) { m_Score += pointsToAdd; };

		ScoreComponent(GameObject* pParent) : Component(pParent) {};
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(const ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(const ScoreComponent&& other) = delete;

	private:
		int m_Score{ };
	};

	class DisplayDataComponent final : public Component
	{
	public:
		void UpdateDisplay(std::string dataStr);

		DisplayDataComponent(GameObject* pParent, std::string dataPrefix, std::string startData = "");
		virtual ~DisplayDataComponent() = default;
		DisplayDataComponent(const DisplayDataComponent& other) = delete;
		DisplayDataComponent(const DisplayDataComponent&& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent& other) = delete;
		DisplayDataComponent& operator=(const DisplayDataComponent&& other) = delete;

	private:
		dae::TextComponent* m_pTextLocation; // non-owning reference to parent's text component (if it has one)
		std::string m_DataPrefix;
	};

	class LivesObservingComponent final : public ObservingComponent
	{
	public:
		LivesObservingComponent(GameObject* pParent, GameObject* pDisplay);

		virtual void OnNotify(const ISubject* pSubject, Event event) override;

	private:
		GameObject* m_pDisplayObject;
	};

	class ScoreObservingComponent final : public ObservingComponent
	{
	public:
		ScoreObservingComponent(GameObject* pParent, GameObject* pDisplay);

		virtual void OnNotify(const ISubject* pSubject, Event event) override;
	
	private:
		GameObject* m_pDisplayObject;
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
}

