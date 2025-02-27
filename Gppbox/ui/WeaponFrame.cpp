#include "WeaponFrame.hpp"

#include <cassert>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics/Text.hpp>

#include "SFML/Graphics/RenderTarget.hpp"



WeaponFrame::WeaponFrame(sf::Vector2f position)
	:	m_shape(size, 10, 5),
		m_scaleSod(8, .5f, 0, m_baseScale),
		m_basePosition(position), m_targetPosition(m_basePosition),
		m_positionSod(8, 1, 0, m_basePosition)
{
	m_shape.setScale(m_targetScale);
	m_shape.setOrigin(size / 2.0f);
	
	m_font.loadFromFile("res/MAIAN.TTF");
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
	sf::Text text(m_weaponName, m_font, 20.0f * std::max(0.1f, m_shape.getScale().x));
	text.setStyle(sf::Text::Bold);
	auto bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
	text.setPosition(m_shape.getPosition() - sf::Vector2f(0, 20));
	text.setFillColor(sf::Color::Black);
	target.draw(text);

	if (m_reloadTime >= 0)
	{
		// replace to C++20 std::format if possible, this is ass
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << m_reloadTime << "s";
		text.setString(stream.str());
		text.setCharacterSize(15.0f * std::max(0.1f, m_shape.getScale().x));
		bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
		text.setPosition(m_shape.getPosition() + sf::Vector2f(0, 20));
		target.draw(text);
	}
}
