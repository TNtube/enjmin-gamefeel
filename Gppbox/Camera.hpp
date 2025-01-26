#pragma once
#include <SFML/Graphics/View.hpp>

#include "Entity.hpp"
#include "SecondOrderDynamics.hpp"

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

	void im();


private:
	sf::View m_view;
	Entity* m_player = nullptr;

	float m_frequency = 1.5f;
	float m_damping = 1.0f;
	float m_overshoot = 0;
	float m_yLevel = 0;

	SecondOrderDynamics2f m_sod;
};
