#pragma once
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class BulletHandler
{
public:
	struct Bullet
	{
		Bullet(sf::Vector2f pos, sf::Vector2f dir) : position(pos), direction(dir), speed(800.f), timer(3.f) {}
		sf::Vector2f position;
		sf::Vector2f direction;
		float speed;
		float timer;
	};

	BulletHandler();

	void shoot(sf::Vector2f from, sf::Vector2f to);
	void update(double dt);
	void draw(sf::RenderWindow& win);

private:
	std::vector<Bullet> m_bullets;
	sf::RectangleShape m_bulletShape;
	
};
