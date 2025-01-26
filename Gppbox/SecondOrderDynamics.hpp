#pragma once
#include <SFML/System/Vector2.hpp>
#include "Lib.hpp"

// implementation of https://www.youtube.com/watch?v=KPoeNZZ6H4s
template <typename T>
class SecondOrderDynamics
{
public:
	SecondOrderDynamics(float frequency, float damping, float overshoot, T x0);
	void SetParams(float frequency, float damping, float overshoot);
	T Update(float dt, T x);

private:
	T m_previous;
	T m_y, m_yd;

	float m_k1;
	float m_k2;
	float m_k3;

	float m_frequency, m_damping, m_overshoot;
};

template <typename T>
SecondOrderDynamics<T>::SecondOrderDynamics(float frequency, float damping, float overshoot, T x0)
	: m_previous(x0), m_y(x0)
{
	SetParams(frequency, damping, overshoot);
}


template <typename T>
void SecondOrderDynamics<T>::SetParams(float frequency, float damping, float overshoot)
{
	m_frequency = frequency;
	m_damping = damping;
	m_overshoot = overshoot;
	
	m_k1 = m_damping / (Lib::pi<float>() * m_frequency);
	m_k2 = 1.0f / std::pow(2 * Lib::pi<float>() * m_frequency, 2);
	m_k3 = m_overshoot * m_damping / (2 * Lib::pi<float>() * m_frequency);
}

template <typename T>
T SecondOrderDynamics<T>::Update(float dt, T x)
{
	auto xd = (x - m_previous) / dt;
	m_previous = x;

	float k2Stable = std::max({m_k2, dt*dt/2 + dt*m_k1/2, dt*m_k1});

	m_y  += dt * m_yd;
	m_yd += dt * (x + m_k3*xd - m_y - m_k1*m_yd) / k2Stable;

	return m_y;
}

typedef SecondOrderDynamics<sf::Vector2f> SecondOrderDynamics2f;
typedef SecondOrderDynamics<float> SecondOrderDynamicsF;