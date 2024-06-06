#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform();
		Transform(glm::vec2 translation, glm::vec2 rotation, glm::vec2 scale);

		// Getters
		inline const glm::vec2& GetTranslation() const { return m_Translation; }
		inline const glm::vec2& GetRotation()	 const { return m_Rotation; };
		inline const glm::vec2& GetScale()		 const { return m_Scale; };

		// Setters
		inline void SetTranslation(glm::vec2 newPos) { m_Translation = newPos; };
		inline void SetTranslation(float x, float y) { SetTranslation({ x, y }); };

		inline void SetRotation(glm::vec2 newRotation) { m_Rotation = newRotation; };
		inline void SetRotation(float xAngle, float yAngle) { SetRotation({ xAngle, yAngle }); };

		inline void SetScale(glm::vec2 newScale) { m_Scale = newScale; };
		inline void SetScale(float xScale, float yScale) { SetScale({ xScale, yScale}); };
		inline void SetScale(float universalScale) { SetScale({ universalScale, universalScale }); };

		// Modifiers
		void MultiplyScale(float multiplier);
		void AddTranslation(glm::vec2 addition);
		void AddRotation(glm::vec2 addition);
		
	private:
		glm::vec2 m_Translation;
		glm::vec2 m_Rotation;
		glm::vec2 m_Scale;
	};
}
