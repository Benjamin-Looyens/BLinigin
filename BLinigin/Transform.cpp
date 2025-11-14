#include "Transform.h"

dae::Transform::Transform()
	: m_Translation{}, m_Rotation {}, m_Scale{ 1.0f, 1.0f }
{}

dae::Transform::Transform(glm::vec2 translation, glm::vec2 rotation, glm::vec2 scale)
	: m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }
{}

void dae::Transform::MultiplyScale(float multiplier)
{
	m_Scale.x *= multiplier;
	m_Scale.y *= multiplier;
}

void dae::Transform::AddTranslation(glm::vec2 addition)
{
	m_Translation += addition;
}

void dae::Transform::AddRotation(glm::vec2 addition)
{
	m_Rotation += addition;

	m_Rotation.x = std::fmod(m_Rotation.x, 360.0f);
	m_Rotation.y = std::fmod(m_Rotation.y, 360.0f);
}
