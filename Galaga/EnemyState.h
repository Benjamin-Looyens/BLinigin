#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "State.h"

namespace dae {
	class EnemyLogic;

	using EnemyState = State<EnemyLogic>;

	class EnemySpawnInState final : public EnemyState
	{
		// Bezier Movement based on: https://www.youtube.com/watch?v=qoIYkCwWLkA
		// By "Ather Omar" (Youtube channel, May 1st, 2017)
		class BezierPath final
		{
			struct Bezier {
				glm::vec2 startPoint;
				glm::vec2 startOffset;

				glm::vec2 endOffset;
				glm::vec2 endPoint;

				glm::vec2 CalcPosOnCurve(float t)
				{
					float tSqr{ t * t };
					float tCub{ tSqr * t };

					float u{ 1.0f - t };
					float uSqr{ u * u };
					float uCub{ uSqr * u };

					glm::vec2 point{ (uCub * startPoint) + (3 * uSqr * t * startOffset) + (3 * u * tSqr * endOffset) + (tCub * endPoint) };
					point.x = round(point.x);
					point.y = round(point.y);

					return point;
				}
			};

		public:
			void AddCurve(Bezier curve, int samples) {
				m_Curves.push_back(curve);
				m_Samples.push_back(samples);
			}

			void Sample(std::vector<glm::vec2>* pSampledPath) {
				for (size_t index{}; index < m_Curves.size(); ++index) {
					for (float t{ }; t <= 1.0f; t += (1.0f / static_cast<float>(m_Samples[index]))) {
						pSampledPath->push_back(m_Curves[index].CalcPosOnCurve(t));
					}
				}
			}

		private:
			std::vector<Bezier> m_Curves;
			std::vector<int> m_Samples;
		};

	public:
		EnemySpawnInState();
		EnemySpawnInState(std::vector<glm::vec2> path, glm::vec2 startLocation);
		~EnemySpawnInState() override = default;

		void Update(EnemyLogic& enemy) override;
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:
		std::vector<glm::vec2> m_Path{};
		glm::vec2 m_StartLocation{};

		int m_CurrentWayPoint;
		static constexpr float EPSILON{ 5.0f };
	};

	class EnemyPatrolState final : public EnemyState
	{
	public:
		~EnemyPatrolState() override = default;

		void Update(EnemyLogic& enemy) override;
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:
		float m_ElapsedTime{};
		float m_MaxTime{ 5.0f }; // TODO : Define value in constructor (or OnEnter)
		static constexpr int s_PERCENT_CHANCE_TO_KAMIKAZE{ 9 };
	};

	class EnemyKamikazeState final : public EnemyState
	{
	public:
		~EnemyKamikazeState() override = default;

		void Update(EnemyLogic& enemy) override;
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:
		glm::vec2 m_KamikazePosition{};
		glm::vec2 m_StartPosition{};

		bool m_NotDone{ true };
		float m_DoneTimer{};
		float m_MaxDoneTimer{ 1.0f };

		float m_ShootTimer{};

		constexpr static float s_TIME_TO_SHOOT{ 0.5f };
		constexpr static float EPSILON{ 5.0f };
	};

	class EnemyReturnToPatrolState final : public EnemyState
	{
	public: 
		~EnemyReturnToPatrolState() override = default;

		void Update(EnemyLogic& enemy) override;
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:

	};

	class EnemyDeathState final : public EnemyState
	{
	public:
		~EnemyDeathState() override = default;

		void Update(EnemyLogic& enemy) override;
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:
		float m_ElapsedTime{};
		constexpr static float DEATH_TIMER{ 0.0f };
	};
}