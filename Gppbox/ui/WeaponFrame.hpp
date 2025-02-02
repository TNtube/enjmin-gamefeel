#pragma once
#include <SFML/System/Vector2.hpp>

#include "RoundedRectangleShape.hpp"
#include "math/SecondOrderDynamics.hpp"

class WeaponFrame
{
public:
	WeaponFrame(sf::Vector2f position = {0, 0});

	sf::Vector2f size = {100, 100};

	void update(double dt);
	void draw(sf::RenderTarget& target) const;

	void setScale(float x, float y) { m_targetScale = {x, y}; }
	

private:
	RoundedRectangleShape m_shape;

	sf::Vector2f m_targetScale = {0.05f, 0.05f};
	SecondOrderDynamics2f m_scaleSod;

	sf::Vector2f m_finalPosition;
	sf::Vector2f m_targetPosition;
	SecondOrderDynamics2f m_positionSod;
};
