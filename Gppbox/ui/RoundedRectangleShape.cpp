#include "RoundedRectangleShape.hpp"

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned int cornerPointCount = 5)
{
	m_size = size;
	m_radius = radius;
	m_cornerPointCount = cornerPointCount;
	update();
}

std::size_t RoundedRectangleShape::getPointCount() const
{
	return m_cornerPointCount*4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
	if(index >= m_cornerPointCount*4)
		return {0,0};

	float deltaAngle = 90.0f/(m_cornerPointCount-1);
	sf::Vector2f center;
	unsigned int centerIndex = index/m_cornerPointCount;
	static const float pi = 3.141592654f;

	switch(centerIndex)
	{
		case 0: center.x = m_size.x - m_radius; center.y = m_radius; break;
		case 1: center.x = m_radius; center.y = m_radius; break;
		case 2: center.x = m_radius; center.y = m_size.y - m_radius; break;
		case 3: center.x = m_size.x - m_radius; center.y = m_size.y - m_radius; break;
	}

	return sf::Vector2f(m_radius*cos(deltaAngle*(index-centerIndex)*pi/180)+center.x,
					-m_radius*sin(deltaAngle*(index-centerIndex)*pi/180)+center.y);
}