#include "Transform.h"

dae::Transform::Transform()
	: m_Translation{}, m_Rotation {}, m_Scale{ 1.0f, 1.0f, 1.0f }
{}

dae::Transform::Transform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
	: m_Translation{ translation }, m_Rotation{ rotation }, m_Scale{ scale }
{}

void dae::Transform::MultiplyScale(float multiplier)
{
	m_Scale.x *= multiplier;
	m_Scale.y *= multiplier;
	m_Scale.z *= multiplier;
}

void dae::Transform::AddTranslation(glm::vec3 addition)
{
	m_Translation += addition;
}

void dae::Transform::AddRotation(glm::vec3 addition)
{
	m_Rotation += addition; 
	// TODO: probably do a check for if exceeds 360 degree rotation, and do fmod for it
}
