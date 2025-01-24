#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Game;

class Entity
{
public:

	sf::RectangleShape sprite;
	
	// Base coordinates
	int cx, cy;
	float xr, yr;

	// Final coordinates
	float xx, yy;

	// Movement
	float dx, dy;

	bool onGround = false;

	Entity(Game* game, int x, int y);

	void update(double dt);
	void draw(sf::RenderWindow& win) const;
	void setCoordinates(float x, float y);

	void im();

private:
	Game* m_pGame;
};
