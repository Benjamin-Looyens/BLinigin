#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetTranslation() const { return m_Translation; }

		void SetTranslation(float x, float y, float z);
		void SetTranslation(glm::vec3 newPos) { m_Translation = newPos; };

	private:
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}
