#include "BulletHandler.hpp"

#include "C.hpp"
#include "MathUtils.hpp"
#include "Game.hpp"

BulletHandler::BulletHandler(Game* game)
	: m_game(game)
{
	m_bulletShape.setFillColor(sf::Color(0xffffffff));
	m_bulletShape.setRadius(2.5f);
}

void BulletHandler::shoot(sf::Vector2f from, sf::Vector2f to)
{
	sf::Vector2f direction = {to.x - from.x, to.y - from.y};


	m_bullets.emplace_back(from, MathUtils::normalize(direction));
}

void BulletHandler::update(double dt)
{
	for (auto it = m_bullets.begin(); it != m_bullets.end();)
	{
		auto& bullet = *it;

		if (bullet.timer <= 0)
			it = m_bullets.erase(it);
		else
		{
			auto dir = bullet.direction * bullet.speed;
			dir.x *= dt;
			dir.y *= dt;
			bullet.position += dir;
			++it;
			bullet.timer -= dt;

			if (isCollidingWall(bullet.position.x, bullet.position.y)) bullet.timer = 0;
			if (auto enemy =m_game->world.getEnemyAt(bullet.position.x, bullet.position.y))
			{
				enemy->pv -= 1;
				bullet.timer = 0;
				m_game->camera.addShake(0.1f, 200.f);
			}
		}
	}
}

void BulletHandler::draw(sf::RenderWindow& win)
{
	for (auto& bullet : m_bullets)
	{
		m_bulletShape.setPosition(bullet.position);
		win.draw(m_bulletShape);
	}
}

bool BulletHandler::isCollidingWall(float x, float y) const
{
	int cx = static_cast<int>(x) / C::GRID_SIZE;
	int cy = static_cast<int>(y) / C::GRID_SIZE;

	if (m_game->world.isWall(cx, cy))
		return true;

	return false;
}


