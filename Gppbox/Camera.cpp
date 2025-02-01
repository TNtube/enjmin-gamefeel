#include "Camera.hpp"

#include <iostream>

#include "C.hpp"
#include "Dice.hpp"
#include "imgui.h"

Camera::Camera(sf::Vector2f center, sf::Vector2f size)
	:	m_view(center, size),
		m_followSod(m_frequency, m_damping, m_overshoot, sf::Vector2f{0, 0}),
		m_yLevel(center.y), m_xLevel(center.x),
		m_throttler(0.016f),
		m_shakeSod(8, 1, 0, {0, 0}) // magic numbers for shake
{
}

void Camera::update(double dt)
{
	if (m_player == nullptr)
	{
		std::cerr << "Camera has no player\n";
		return;
	}
	auto playerCenter = sf::Vector2f(m_player->xx + C::GRID_SIZE / 2.f, m_player->yy + C::GRID_SIZE / 2.f);

	auto viewSize = m_view.getSize();
	
	m_lowerBound = m_yLevel + (viewSize.y / 5.f);
	m_upperBound = m_yLevel - (viewSize.y / 3.f);

	if (playerCenter.y < m_upperBound)
		m_yLevel = playerCenter.y + (m_view.getSize().y / 3.f);
	if (playerCenter.y > m_lowerBound)
		m_yLevel = playerCenter.y - (m_view.getSize().y / 5.f);

	if (m_player->onGround)
		m_yLevel = playerCenter.y - (m_view.getSize().y / 5.f);

	if (m_orientation > 0)
	{
		m_rightBound = m_xLevel - (viewSize.x / 8.0f);
		m_leftBound  = m_xLevel - (viewSize.x / 3.5f);

		if (playerCenter.x < m_leftBound)
		{
			m_xLevel = playerCenter.x + (m_view.getSize().x / 3.5f);
			m_orientation = static_cast<int>(m_player->lastXDir);
		}
		if (playerCenter.x > m_rightBound)
			m_xLevel = playerCenter.x + (m_view.getSize().x / 8.0f);
	}
	else
	{
		m_rightBound = m_xLevel + (viewSize.x / 3.5f);
		m_leftBound  = m_xLevel + (viewSize.x / 8.0f);

		if (playerCenter.x < m_leftBound)
			m_xLevel = playerCenter.x - (m_view.getSize().x / 8.0f);
		if (playerCenter.x > m_rightBound)
		{
			m_xLevel = playerCenter.x - (m_view.getSize().x / 3.5f);
			m_orientation = static_cast<int>(m_player->lastXDir);
		}
	}

	auto target = sf::Vector2f {
		m_xLevel,
		m_yLevel
	};

	auto cameraPosition = m_followSod.Update(dt, target);

	// shake
	float radius = 0;
	for (auto it = m_shakes.begin(); it != m_shakes.end();)
	{
		it->duration -= dt;
		if (it->duration <= 0)
			it = m_shakes.erase(it);
		else
		{
			radius += it->intensity;
			++it;
		}
	}
	radius = std::min(radius, 100.0f); // avoid too much shaking

	if (radius > 0)
	{
		if (m_throttler.shouldExecute(dt))
		{
			m_shakeOffsetX = (Dice::randF() * 2.0f - 1.0f) * radius;
			m_shakeOffsetY = (Dice::randF() * 2.0f - 1.0f) * radius;
		}
	}
	else
	{
		m_shakeOffsetX = 0;
		m_shakeOffsetY = 0;
	}

	// run most of the time for nothing, but we need to keep the internal state of the SOD up-to-date
	// multiple sods are used to give different feels depending on the situation
	cameraPosition = m_shakeSod.Update(dt, cameraPosition + sf::Vector2f{m_shakeOffsetX, m_shakeOffsetY});

	m_view.setCenter(cameraPosition);
}

void Camera::setPlayer(Entity* player)
{
	m_player = player;
	m_xLevel = player->xx;
	m_yLevel = player->yy;
	
}

void Camera::addShake(float duration, float intensity)
{
	m_shakes.push_back({duration, intensity});
}

void Camera::setActive(sf::RenderTarget& win) const
{
	win.setView(m_view);

	float w = static_cast<float>(win.getSize().x);
	float h = static_cast<float>(win.getSize().y);

	if (m_drawDebugLines)
	{
		sf::RectangleShape line;
		line.setFillColor(sf::Color{0xff07ffff});
		line.setSize({w * 2.0f, 3.0f});
		line.setPosition({-w, m_upperBound});
		win.draw(line);
		line.setPosition({-w, m_lowerBound});
		win.draw(line);

		line.setSize({2.0f, h * 2.0f});
		line.setPosition({m_leftBound, -h});
		win.draw(line);
		line.setPosition({m_rightBound, -h});
		win.draw(line);
	}
}

void Camera::im()
{	float duration = 1.0f;
	float timeStep = 0.01f;

	int plots = static_cast<int>(duration / timeStep);

	ImGui::Checkbox("Draw Bound Lines", &m_drawDebugLines);

	bool edit = false;
	edit |= ImGui::DragFloat("frequency", &m_frequency, 0.01f);
	edit |= ImGui::DragFloat("damping", &m_damping, 0.01f);
	edit |= ImGui::DragFloat("overshoot", &m_overshoot, 0.01f);

	if (edit)
	{
		m_followSod.SetParams(m_frequency, m_damping, m_overshoot);
	}

	auto plotSod = SecondOrderDynamics2f(m_frequency, m_damping, m_overshoot, {0, 0});

	auto lambda = [](void* data, int i) -> float
	{
		auto sod = static_cast<SecondOrderDynamics2f*>(data);
		const auto pos = sod->Update(0.01f, {1, 1});
		return pos.x * 40 + 40;
	};

	ImGui::PlotLines("SOD",  lambda, &plotSod, plots, 0, nullptr, 0, 100, ImVec2(0, 80));
}
