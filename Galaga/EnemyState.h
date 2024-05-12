#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace dae {
	class EnemyLogic;

	class EnemyState
	{
	public:
		virtual ~EnemyState() {};
		virtual void Update(EnemyLogic& enemy) = 0;
		virtual void OnEnter(EnemyLogic& enemy) = 0;
		virtual void OnExit(EnemyLogic& enemy) = 0;
	};

	class SpawnIn final : public EnemyState
	{
		struct Bezier {
			glm::vec2 p0;
			glm::vec2 p1;

			glm::vec2 p2;
			glm::vec2 p3;

			glm::vec2 CalcPosOnCurve(float t)
			{
				float tSqr{ t * t };
				float tCub{ tSqr * t };

				float u{ 1.0f - t };
				float uSqr{ u * u };
				float uCub{ uSqr * u };

				//glm::vec2 point{ (p0.x * uCub, p0.y * uCub) + (3 * uSqr * t * p1) };
				glm::vec2 point{ (uCub * p0) + (3 * uSqr * t * p1) + (3 * u * tSqr * p2) + (tCub * p3) };
				point.x = round(point.x);
				point.y = round(point.y);

				return point;
			}
		};

		class BezierPath final
		{
		public:
			BezierPath() {};
			~BezierPath() {};

			void AddCurve(dae::SpawnIn::Bezier curve, int samples) {
				m_Curves.push_back(curve);
				m_Samples.push_back(samples);
			}

			void Sample(std::vector<glm::vec2>* pSampledPath) {
				for (int index{}; index < m_Curves.size(); ++index) {
					for (float t{ }; t <= 1.0f; t += (1.0f / static_cast<float>(m_Samples[index]))) {
					//for (float t{ }; t <= 1.0f; t += (1.0f / m_Samples[index])) {
						pSampledPath->push_back(m_Curves[index].CalcPosOnCurve(t));
					}
				}
			}

		private:
			std::vector<dae::SpawnIn::Bezier> m_Curves;
			std::vector<int> m_Samples;
		};

	public:
		SpawnIn();
		SpawnIn(std::vector<glm::vec2> path, glm::vec2 endLocation);

		void Update(EnemyLogic& enemy);
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:
		std::vector<glm::vec2> m_Path;
		glm::vec2 m_EndLocation;
		glm::vec2 m_StartLocation;

		int m_CurrentWayPoint;
		static constexpr float EPSILON{ 5.0f };
	};

	class Patrol final : public EnemyState
	{
	public:
		Patrol() {};

		void Update(EnemyLogic& enemy);
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:

	};

	class Kamikaze final : public EnemyState
	{
	public:
		Kamikaze() {};

		void Update(EnemyLogic& enemy);
		void OnEnter(EnemyLogic& enemy) override;
		void OnExit(EnemyLogic& enemy) override;

	private:

	};
}