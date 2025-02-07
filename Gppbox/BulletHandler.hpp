#pragma once
#include <random>
#include <vector>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "Tween.hpp"

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
	friend class LaserBeam;
	void ResetLaserTweens();
	bool isCollidingWall(float x, float y) const;
	std::vector<Bullet> m_bullets;
	sf::CircleShape m_bulletShape;
	Game* m_game;

	Tween<float> m_laserLengthTween;
	Tween<float> m_laserHeightTween;
	Tween<float> m_laserIdleTween;
	Tween<float> m_laserEndTween;

	float m_currentHeight = 0;
	float m_currentLength = 0;
	float m_targetHeight = 10;
	float m_targetLength = 1200;

	float m_laserShake = 400.0f;
	
};
