#include "BulletHandler.hpp"

#include "C.hpp"
#include "math/MathUtils.hpp"
#include "Game.hpp"
#include "imgui.h"
#include "Interp.hpp"

void BulletHandler::ResetLaserTweens()
{
	m_laserLengthTween = Tween<float>::From(0.0f).To(m_targetLength).For(0.3f);
	m_laserHeightTween = Tween<float>::From(0.0f).To(m_targetHeight).For(0.05f);
	m_laserEndTween = Tween<float>::From(m_targetHeight).To(0.0f).For(0.3f).SetEase(Ease::OutExpo);
	m_laserIdleTween = Tween<float>::From(0.0f).To(0.0f).For(1.0f);
}

BulletHandler::BulletHandler(Game* game)
	: m_game(game)
{
	m_bulletShape.setFillColor(sf::Color(0xffffffff));
	m_bulletShape.setRadius(2.5f);
	m_bulletShape.setOrigin(2.5f, 2.5f);

	ResetLaserTweens();
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
				enemy->offsetDx = bullet.direction.x * 30;
				enemy->offsetDy = -30;
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

	float dtf = static_cast<float>(dt);

	if (laserOn)
	{
		if (!m_laserLengthTween.IsFinished())
		{
			m_currentLength = m_laserLengthTween.Update(dtf);
			m_currentHeight = m_laserHeightTween.Update(dtf);
			m_game->camera.addShake(0.1f, m_laserShake*dtf);
		} else if (!m_laserIdleTween.IsFinished())
		{
			m_laserIdleTween.Update(dtf);
			m_game->camera.addShake(0.1f, m_laserShake*dtf);
		} else if (!m_laserEndTween.IsFinished())
		{
			m_currentHeight = m_laserEndTween.Update(dtf);
		} else
		{
			ResetLaserTweens();
			laserOn = false;
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


	if (!laserOn) return;
	float length = m_currentLength * laserDir.x;

	sf::Color red = sf::Color::Magenta;
	red.a = 0;

	Vector2f start = {laserStart.x + m_currentHeight * 3 * laserDir.x, laserStart.y};
	// start.x += currentLength * 3 * laserDir.x;

	// laser body
	sf::Vertex rectangle[] =
	{
		sf::Vertex({start.x, start.y - m_currentHeight}, red),
		sf::Vertex({start.x + length, start.y - m_currentHeight}, red),
		sf::Vertex({start.x + length, start.y}, sf::Color::White),
		sf::Vertex(start, sf::Color::White),
		sf::Vertex({start.x + length, start.y }, sf::Color::White),
		sf::Vertex({start.x, start.y}, sf::Color::White),
		sf::Vertex({start.x, start.y + m_currentHeight}, red),
		sf::Vertex({start.x + length, start.y + m_currentHeight}, red),
	};

	// laser head
	const int points = 50;
	float radiusX = m_currentHeight * 4;
	float radiusY = m_currentHeight;
	sf::Vector2f center(start.x, start.y);

	sf::Vertex leftEllipse[points + 2];
	leftEllipse[0] = sf::Vertex(center, sf::Color::White);
	leftEllipse[1] = sf::Vertex({center.x, center.y + m_currentHeight}, red);
	for (int i = 2; i <= points; ++i) {
		float angle = Lib::pi() / 2.0f + (i * Lib::pi() / points) * laserDir.x;
		float x = center.x + radiusX * cos(angle);
		float y = center.y + radiusY * sin(angle);
		leftEllipse[i] = sf::Vertex(sf::Vector2f(x, y), red);
	}
	
	leftEllipse[points + 1] = leftEllipse[1];
	
	// laser head
	center.x += length;

	sf::Vertex rightEllipse[points + 2];

	rightEllipse[0] = sf::Vertex(center, sf::Color::White);
	rightEllipse[1] = sf::Vertex({center.x, center.y + m_currentHeight}, red);
	for (int i = 2; i <= points; ++i) {
		float angle = Lib::pi() / 2.0f - (i * Lib::pi() / points) * laserDir.x;
		float x = center.x + radiusX * cos(angle);
		float y = center.y + radiusY * sin(angle);
		rightEllipse[i] = sf::Vertex(sf::Vector2f(x, y), red);
	}
	
	rightEllipse[points + 1] = rightEllipse[1];

	win.draw(leftEllipse, points + 2, sf::TrianglesFan);
	win.draw(rightEllipse, points + 2, sf::TrianglesFan);
	win.draw(rectangle, 8, sf::Quads);
}

void BulletHandler::im()
{
	ImGui::DragFloat("Laser half height", &m_currentHeight);
	ImGui::DragFloat2("Laser start", &laserStart.x);
}

bool BulletHandler::isCollidingWall(float x, float y) const
{
	int cx = static_cast<int>(x) / C::GRID_SIZE;
	int cy = static_cast<int>(y) / C::GRID_SIZE;

	if (m_game->world.isWall(cx, cy))
		return true;

	return false;
}


