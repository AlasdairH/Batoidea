#include "Light.h"

namespace Batoidea
{
	Light::Light()
	{
		m_position = glm::vec3(0, 0, 0);
		m_colour = glm::vec4(1, 1, 1, 0);
	}	
	
	Light::Light(const glm::vec3 &_position, const glm::vec4 &_colour)
	{
		m_position = _position;
		m_colour = _colour;
	}
}