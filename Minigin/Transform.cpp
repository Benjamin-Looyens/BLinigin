#include "Transform.h"

void dae::Transform::SetTranslation(const float x, const float y, const float z)
{
	m_Translation.x = x;
	m_Translation.y = y;
	m_Translation.z = z;
}
