#pragma once
#include "GameObject.h"
#include "Component.h"
#include "ObserverComponents.h"
#include "Dart.h"

namespace dae {
	class Font;

	class PlayerLogic final : public Component
	{
	public:
		void Update() override;
		void LateUpdate() override;

		void Fire();

		PlayerLogic(GameObject* pParent);

		PlayerLogic(const PlayerLogic& other) = delete;
		PlayerLogic(const PlayerLogic&& other) = delete;
		PlayerLogic& operator=(const PlayerLogic& other) = delete;
		PlayerLogic& operator=(const PlayerLogic&& other) = delete;

	private:
		std::vector<GameObject*> m_pDarts;
	};

	class LivesComponent final : public ObservingComponent
	{
	public:
		void LoseLife();
		void GainLife();

		int GetLifeCount() const { return m_NumLives; };

		void Render() const override;
		void SetLifeTexture(Texture2D* pNewTexture);

		void OnNotify(const ISubject* pSubject, Event event) override;

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

	class ScoreComponent final : public ObservingComponent
	{
	public:
		int GetScore() const { return m_Score; };
		void AddPoints(int pointsToAdd);

		void Update() override;
		void Render() const override;

		void OnNotify(const ISubject* pSubject, Event event) override;

		ScoreComponent(GameObject* pParent, glm::vec2 displayPosition, dae::Font* pFont, std::string resourceName);
		virtual ~ScoreComponent() = default;
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(const ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(const ScoreComponent&& other) = delete;

	private:
		Font* m_pFont;		
		Texture2D* m_pTextTexture;
		const std::string m_ResourceFileStr;
		glm::vec2 m_DisplayPos;

		int m_Score{ };
		bool m_TextDirty{ true };
	};

}