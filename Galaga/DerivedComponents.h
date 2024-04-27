#pragma once
#include "Component.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>

namespace dae {
	// Class Forward Declarations
	class Font;
	class Texture2D;
	class Observer;
	enum class Event;
	class Observer;

	// Text Component
	class TextComponent : public Component
	{
	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);

		TextComponent(dae::GameObject* pParent, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;
	};

	// Texture Component
	class TextureComponent : public Component
	{
	public:
		virtual void Render() const override;

		void SetTexture(const std::string& filename);

		TextureComponent(dae::GameObject* pParent, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};

	// FPS Component
	class FPSComponent : public Component
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

	class WaveMovementYComponent : public Component
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
		glm::vec3 m_Origin;

		float m_WaveHeight;
		float m_WaveSpeed;

		float m_WaveTime{ 0.0f };
	};

	class SubjectComponent : public Component
	{
	public:
		void AddObserver(Observer* pObserverToAdd);
		void RemoveObserver(Observer* pObserverToRemove);

		void NotifyObservers(Event& eventToNotify);

		SubjectComponent(GameObject* pParent);
		virtual ~SubjectComponent() = default;
		SubjectComponent(const SubjectComponent& other) = delete;
		SubjectComponent(const SubjectComponent&& other) = delete;
		SubjectComponent& operator=(const SubjectComponent& other) = delete;
		SubjectComponent& operator=(const SubjectComponent&& other) = delete;

	private:
		std::vector<Observer*> m_pObservers;
	};
	
	template<typename ObserverType>
	concept DerivedObserver = requires(ObserverType observer) { std::derived_from<ObserverType, Observer>; };
	 
	template<DerivedObserver ObserverType>
	class ObservingComponent : public Component
	{
	public:
		template<typename... TArgs>
		ObservingComponent(GameObject* pParent, const TArgs&... args) : Component(pParent)
		{
			//m_pObserver = std::make_unique<ObserverType>(std::forward<TArgs>(args)...);
			m_pInternalObserver = std::make_unique<ObserverType>(args...);
		}
		virtual ~ObservingComponent() = default;
		ObservingComponent(const ObservingComponent& other) = delete;
		ObservingComponent(const ObservingComponent&& other) = delete;
		ObservingComponent& operator=(const ObservingComponent& other) = delete;
		ObservingComponent& operator=(const ObservingComponent&& other) = delete;

		ObserverType* GetRawPointer() const { return m_pInternalObserver.get(); };

	private:
		std::unique_ptr<ObserverType> m_pInternalObserver;
	};

	class LivesComponent : public Component
	{
	public:
		void LoseLife();
		void GainLife();

		int GetLifeCount() { return m_NumLives; };

		LivesComponent(GameObject* pParent, int lives);
		virtual ~LivesComponent() = default;
		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(const LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(const LivesComponent&& other) = delete;

	private:
		int m_NumLives;
	};

	class ScoreComponent : public Component
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

	class DisplayDataComponent : public Component
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
}

