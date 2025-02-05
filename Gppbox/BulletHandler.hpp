#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Entity;
class Game;

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

	BulletHandler(Game* game);

	size_t getBulletCount() const { return m_bullets.size(); }

	void shoot(sf::Vector2f from, sf::Vector2f to);
	void update(double dt);
	void draw(sf::RenderTarget& win);
	void im();

	bool laserOn = false;
	
	sf::Vector2f laserStart {300, 500};
	sf::Vector2f laserDir {1, 0};

private:
	bool isCollidingWall(float x, float y) const;
	std::vector<Bullet> m_bullets;
	sf::CircleShape m_bulletShape;
	Game* m_game;

	float laserLengthDuration = 0.3f;
	float laserHeightDuration = 0.05f;
	float laserIdleDuration = 1.0f;
	float laserEndDuration = 0.3f;
	float startAnimationTimer = 0;
	float idleAnimationTimer = 0;
	float endAnimationTimer = 0;

	float currentHeight = 0;
	float currentLength = 0;
	float targetHeight = 10;
	float targetLength = 1200;


	float laserShake = 0.0f;
	// float laserShake = 400.0f;
	
};
