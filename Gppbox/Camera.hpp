#pragma once
#include <random>
#include <SFML/Graphics/View.hpp>

#include "entity/Entity.hpp"
#include "math/SecondOrderDynamics.hpp"
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

	void setPlayer(Entity* player);
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

	SecondOrderDynamics2f m_followSod;

	// movement bounds
	float m_lowerBound	= 0;
	float m_upperBound	= 0;
	float m_leftBound	= 0;
	float m_rightBound	= 0;
	
	float m_yLevel = 0;
	float m_xLevel = 0;

	int m_orientation = 1;

	struct Shake
	{
		float duration; // in seconds
		float intensity; // radius, in pixels
	};

	std::vector<Shake> m_shakes;

	Throttle m_throttler;
	SecondOrderDynamics2f m_shakeSod;

	float m_shakeOffsetX = 0;
	float m_shakeOffsetY = 0;

	bool m_drawDebugLines = false;
};
