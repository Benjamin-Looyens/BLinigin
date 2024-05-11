#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform();
		Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

		// Getters
		const glm::vec3& GetTranslation() const { return m_Translation; }
		const glm::vec3& GetRotation()	  const { return m_Rotation; };
		const glm::vec3& GetScale()		  const { return m_Scale; };

		// Setters
		void SetTranslation(glm::vec3 newPos) { m_Translation = newPos; };
		void SetTranslation(float x, float y, float z) { SetTranslation({ x, y, z }); };

		void SetRotation(glm::vec3 newRotation) { m_Rotation = newRotation; };
		void SetRotation(float xAngle, float yAngle, float zAngle) { SetRotation({ xAngle, yAngle, zAngle }); };

		void SetScale(glm::vec3 newPos) { m_Scale = newPos; };
		void SetScale(float xScale, float yScale, float zScale) { SetRotation({ xScale, yScale, zScale }); };
		void SetScale(float universalScale) { SetRotation({ universalScale ,universalScale ,universalScale }); };

		// Modifiers
		void MultiplyScale(float multiplier);
		void AddTranslation(glm::vec3 addition);
		void AddRotation(glm::vec3 addition);
		
	private:
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}
