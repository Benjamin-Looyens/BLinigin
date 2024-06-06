#pragma once
#include "GameObject.h"
#include "Component.h"
#include "ObserverComponents.h"
#include "Dart.h"

namespace dae {
	class Font;

	class PlayerLogic final : public SubjectComponent
	{
	public:
		void Update() override;
		void LateUpdate() override;

		void Fire();
		void Die();

		PlayerLogic(GameObject* pParent);
		~PlayerLogic() = default;
		PlayerLogic(const PlayerLogic& other) = delete;
		PlayerLogic(const PlayerLogic&& other) = delete;
		PlayerLogic& operator=(const PlayerLogic& other) = delete;
		PlayerLogic& operator=(const PlayerLogic&& other) = delete;

	private:
		int m_DartsFired{ };
	};

	class LivesComponent final : public ObservingSubjectComponent
	{
	public:
		void LoseLife();
		void GainLife();

		int GetLifeCount() const { return m_NumLives; };

		void Render() const override;
		void SetLifeTexture(Texture2D* pNewTexture);

		void OnNotify(const ISubject* pSubject, Event event) override;

		LivesComponent(GameObject* pParent, int lives, glm::vec2 displayPosition = { 0, 0 }, glm::vec2 livesStackDirection = { 1, 0 });
		~LivesComponent() = default;
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

	class ScoreComponent final : public ObservingSubjectComponent
	{
	public:
		int GetScore() const { return m_Score; };
		void AddPoints(int pointsToAdd);

		void Update() override;
		void Render() const override;

		void OnNotify(const ISubject* pSubject, Event event) override;

		ScoreComponent(GameObject* pParent, glm::vec2 displayPosition, dae::Font* pFont, std::string resourceName);
		~ScoreComponent();
		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(const ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(const ScoreComponent&& other) = delete;

	private:
		Font* m_pFont;		

		// Score texture itself:
		Texture2D* m_pTextTexture;
		const std::string m_ResourceFileStr;

		// 1 UP flashing above:
		static Texture2D* s_pOneUpTextTexture;
		static const std::string s_ONE_UP_TEXT;
		static int s_NumScoreCounters;
		static float s_OneUpTimer;
		static bool s_DisplayOneUp;
		static constexpr float s_MAX_ONE_UP_TIMER = 0.3f;

		glm::vec2 m_DisplayPos;

		int m_Score{ };
		bool m_TextDirty{ true };

		int m_NumTimesOneUp{ 0 };
		static constexpr int s_NUM_POINTS_FOR_1UP = 20000;
	};

}