#include "BulletHandler.hpp"

#include "MathUtils.hpp"

BulletHandler::BulletHandler()
{
	m_bulletShape.setFillColor(sf::Color(0xffffffff));
	m_bulletShape.setSize({5, 5});
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
		}
	}
}

void BulletHandler::draw(sf::RenderWindow& win)
{
	for (auto bullet : m_bullets)
	{
		m_bulletShape.setPosition(bullet.position);
		win.draw(m_bulletShape);
	}
}
