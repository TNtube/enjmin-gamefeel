#include "WeaponFrame.hpp"
#include "SFML/Graphics/RenderTarget.hpp"



WeaponFrame::WeaponFrame(sf::Vector2f position)
	:	m_shape(size, 10, 5),
		m_scaleSod(8, .5f, 0, m_baseScale),
		m_basePosition(position), m_targetPosition(m_basePosition),
		m_positionSod(8, 1, 0, m_basePosition)
{
	m_shape.setScale(m_targetScale);
	m_shape.setOrigin(size / 2.0f);
	
}

void WeaponFrame::update(double dt)
{
	auto pos = m_positionSod.Update(dt, m_targetPosition);
	m_shape.setPosition(pos);

	auto scale = m_scaleSod.Update(dt, m_targetScale);
	m_shape.setScale(scale);
}

void WeaponFrame::draw(sf::RenderTarget& target) const
{
	target.draw(m_shape);
}
