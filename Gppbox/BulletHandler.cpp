#include "BulletHandler.hpp"

#include "C.hpp"
#include "math/MathUtils.hpp"
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
			dir.x *= static_cast<float>(dt);
			dir.y *= static_cast<float>(dt);
			bullet.position += dir;
			++it;
			bullet.timer -= static_cast<float>(dt);

			if (isCollidingWall(bullet.position.x, bullet.position.y)) bullet.timer = 0;
			if (auto enemy =m_game->world.getEnemyAt(bullet.position.x, bullet.position.y))
			{
				enemy->pv -= 1;
				bullet.timer = 0;
				m_game->camera.addShake(0.1f, 5.f);

				for (int i = 0; i < 10; ++i)
				{
					float x = (Dice::randF() * 2.0f - 1.0f) * 150;
					float y = (Dice::randF() * 2.0f - 1.0f) * 150;

					auto slerpDir = MathUtils::slerp({x, y}, -bullet.direction, 0.9f);
					
					Particle p;
					p.x = bullet.position.x;
					p.y = bullet.position.y;
					
					p.dx = slerpDir.x;
					p.dy = slerpDir.y;
					p.bhv = [](Particle* p, float dt) {};

					p.scaleX = 0.15f;
					p.scaleY = 0.15f;

					p.life = 0.3f;

					m_game->afterParts.add(p);
				}
			}
		}
	}
}

void BulletHandler::draw(sf::RenderTarget& win)
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


