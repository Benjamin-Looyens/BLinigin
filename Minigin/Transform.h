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
		const glm::vec2& GetTranslation() const { return m_Translation; }
		const glm::vec2& GetRotation()	  const { return m_Rotation; };
		const glm::vec2& GetScale()		  const { return m_Scale; };

		// Setters
		void SetTranslation(glm::vec2 newPos) { m_Translation = newPos; };
		void SetTranslation(float x, float y) { SetTranslation({ x, y }); };

		void SetRotation(glm::vec2 newRotation) { m_Rotation = newRotation; };
		void SetRotation(float xAngle, float yAngle) { SetRotation({ xAngle, yAngle }); };

		void SetScale(glm::vec2 newScale) { m_Scale = newScale; };
		void SetScale(float xScale, float yScale) { SetScale({ xScale, yScale}); };
		void SetScale(float universalScale) { SetScale({ universalScale, universalScale }); };

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
