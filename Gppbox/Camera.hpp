#pragma once
#include <random>
#include <SFML/Graphics/View.hpp>

#include "Entity.hpp"
#include "SecondOrderDynamics.hpp"
#include "Throttle.h"

namespace sf
{
	class RenderWindow;
}

class Camera
{
public:
	Camera(sf::Vector2f center, sf::Vector2f size);
	void update(double dt);

	void setPlayer(Entity* player) { m_player = player; }
	void setActive(sf::RenderWindow& win) const;

	void addShake(float duration, float intensity);

	void im();


private:
	sf::View m_view;
	Entity* m_player = nullptr;

	// movement
	float m_frequency = 1.5f;
	float m_damping = 1.0f;
	float m_overshoot = 0;
	float m_yLevel = 0;

	SecondOrderDynamics2f m_followSod;

	struct Shake
	{
		float duration; // in seconds
		float intensity; // radius, in pixels
	};

	std::vector<Shake> m_shakes;
	std::random_device m_rngDev;
	std::mt19937 m_rng;
	std::uniform_real_distribution<float> m_rngDist;

	Throttle m_throttler;
	SecondOrderDynamics2f m_shakeSod;

	float m_shakeOffsetX = 0;
	float m_shakeOffsetY = 0;
};
