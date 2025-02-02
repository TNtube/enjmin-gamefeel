#pragma once
#include <array>
#include <SFML/Graphics/CircleShape.hpp>

#include "RoundedRectangleShape.hpp"
#include "WeaponFrame.hpp"
#include "math/SecondOrderDynamics.hpp"

class Game;

class WeaponPicker
{
public:
	WeaponPicker(Game* game);

	void update(double dt);
	void draw(sf::RenderTarget& target);

	void im();

private:
	Game* m_game;

	sf::Vector2f m_center;
	
	sf::CircleShape m_cursor;
	SecondOrderDynamics2f m_cursorSod;

	std::array<WeaponFrame, 4> m_frames;

	int m_pickedWeapon = 0;
};
