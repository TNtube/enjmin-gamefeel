#pragma once
#include <SFML/System/Vector2.hpp>

// implementation of https://www.youtube.com/watch?v=KPoeNZZ6H4s
class SecondOrderDynamics
{
public:
	SecondOrderDynamics(float frequency, float damping, float overshoot, sf::Vector2f x0);
	void SetParams(float frequency, float damping, float overshoot);
	sf::Vector2f Update(float T, sf::Vector2f x);

private:
	sf::Vector2f m_previous;
	sf::Vector2f m_y, m_yd;

	float m_k1;
	float m_k2;
	float m_k3;

	float m_frequency, m_damping, m_overshoot;
};
