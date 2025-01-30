#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "EntityController.hpp"

class Game;

class Entity
{
public:
	enum class Type : uint8_t
	{
		Player,
		Enemy
	};

	sf::RectangleShape sprite;
	
	// Base coordinates
	int cx, cy;
	float xr, yr;

	// Final coordinates
	float xx, yy;

	// Movement
	float dx, dy;

	float lastXDir = 1.f;

	bool onGround = false;

	Entity(Game* game, int x, int y, Type type);

	void update(double dt) const;
	void draw(sf::RenderWindow& win) const;
	void setCoordinates(float x, float y);

	void im();

private:
	Type m_type;
	std::unique_ptr<EntityController> m_pController;
};
