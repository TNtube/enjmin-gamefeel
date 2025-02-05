#include "BulletHandler.hpp"

#include "C.hpp"
#include "math/MathUtils.hpp"
#include "Game.hpp"
#include "imgui.h"
#include "Interp.hpp"

BulletHandler::BulletHandler(Game* game)
	: m_game(game)
{
	m_bulletShape.setFillColor(sf::Color(0xffffffff));
	m_bulletShape.setRadius(2.5f);
	m_bulletShape.setOrigin(2.5f, 2.5f);
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

	float dtf = static_cast<float>(dt);

	if (laserOn)
	{
		startAnimationTimer += dtf;

		if (startAnimationTimer <= laserLengthDuration)
		{
			if (startAnimationTimer > 0)
			{
				currentLength = (startAnimationTimer <= laserLengthDuration) ? Interp::lerp(0, targetLength, startAnimationTimer / laserLengthDuration) : targetLength;
				currentHeight = (startAnimationTimer <= laserHeightDuration) ? Interp::lerp(0, targetHeight, startAnimationTimer / laserHeightDuration) : targetHeight;
				m_game->camera.addShake(0.1f, laserShake*dtf);
			}
		}
		else
		{
			idleAnimationTimer += dtf;
		}

		if (idleAnimationTimer <= laserIdleDuration)
		{
			if (idleAnimationTimer > 0)
				m_game->camera.addShake(0.1f, laserShake*dtf);
		}
		else
		{
			endAnimationTimer += dtf;
		}

		if (endAnimationTimer <= laserEndDuration)
		{
			if (endAnimationTimer > 0)
				currentHeight = (endAnimationTimer <= laserEndDuration) ? Interp::lerp(targetHeight, 0, endAnimationTimer / laserEndDuration) : 0;
		}
		else
		{
			laserOn = false;
			startAnimationTimer = 0;
			idleAnimationTimer = 0;
			endAnimationTimer = 0;
			currentHeight = 0;
			currentLength = 0;
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
	float length = currentLength * laserDir.x;

	sf::Color red = sf::Color::Blue;
	red.a = 0;

	Vector2f start = {laserStart.x + currentHeight * 3 * laserDir.x, laserStart.y};
	// start.x += currentLength * 3 * laserDir.x;

	// laser body
	sf::Vertex rectangle[] =
	{
		sf::Vertex({start.x, start.y - currentHeight}, red),
		sf::Vertex({start.x + length, start.y - currentHeight}, red),
		sf::Vertex({start.x + length, start.y}, sf::Color::White),
		sf::Vertex(start, sf::Color::White),
		sf::Vertex({start.x + length, start.y }, sf::Color::White),
		sf::Vertex({start.x, start.y}, sf::Color::White),
		sf::Vertex({start.x, start.y + currentHeight}, red),
		sf::Vertex({start.x + length, start.y + currentHeight}, red),
	};

	// laser head
	const int points = 50;
	float radiusX = currentHeight * 4;
	float radiusY = currentHeight;
	sf::Vector2f center(start.x, start.y);

	sf::Vertex leftEllipse[points + 2];
	leftEllipse[0] = sf::Vertex(center, sf::Color::White);
	leftEllipse[1] = sf::Vertex({center.x, center.y + currentHeight}, red);
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
	rightEllipse[1] = sf::Vertex({center.x, center.y + currentHeight}, red);
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
	ImGui::DragFloat("Laser half height", &currentHeight);
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


