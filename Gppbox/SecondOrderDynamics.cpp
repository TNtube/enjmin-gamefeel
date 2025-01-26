#include "SecondOrderDynamics.h"

#include <algorithm>
#include <complex>

#include "Lib.hpp"

SecondOrderDynamics::SecondOrderDynamics(float frequency, float damping, float overshoot, sf::Vector2f x0)
	: m_previous(x0), m_y(x0), m_yd(0, 0)
{
	SetParams(frequency, damping, overshoot);
}

void SecondOrderDynamics::SetParams(float frequency, float damping, float overshoot)
{
	m_frequency = frequency;
	m_damping = damping;
	m_overshoot = overshoot;
	
	m_k1 = m_damping / (Lib::pi<float>() * m_frequency);
	m_k2 = 1.0f / std::pow(2 * Lib::pi<float>() * m_frequency, 2);
	m_k3 = m_overshoot * m_damping / (2 * Lib::pi<float>() * m_frequency);
}

sf::Vector2f SecondOrderDynamics::Update(float T, sf::Vector2f x)
{
	auto xd = (x - m_previous) / T;
	m_previous = x;

	float k2Stable = std::max({m_k2, T*T/2 + T*m_k1/2, T*m_k1});

	m_y  += T * m_yd;
	m_yd += T * (x + m_k3*xd - m_y - m_k1*m_yd) / k2Stable;

	return m_y;
}
