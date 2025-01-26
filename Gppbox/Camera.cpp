#include "Camera.hpp"

#include <iostream>

#include "C.hpp"
#include "imgui.h"

Camera::Camera(sf::Vector2f center, sf::Vector2f size)
	: m_view(center, size), m_sod(m_frequency, m_damping, m_overshoot, sf::Vector2f{0, 0})
{
}

void Camera::update(double dt)
{
	if (m_player == nullptr)
	{
		std::cerr << "Camera has no player" << std::endl;
		return;
	}
	auto playerCenter = sf::Vector2f(m_player->xx + C::GRID_SIZE / 2.f, m_player->yy + C::GRID_SIZE / 2.f);

	float lowerBound = m_yLevel + (m_view.getSize().y / 5.f);
	float upperBound = m_yLevel - (m_view.getSize().y / 3.f);


	if (playerCenter.y < upperBound)
	{
		m_yLevel = playerCenter.y + (m_view.getSize().y / 3.f);
	}
	if (playerCenter.y > lowerBound)
	{
		m_yLevel = playerCenter.y - (m_view.getSize().y / 5.f);
	}

	if (m_player->onGround)
	{
		m_yLevel = playerCenter.y - (m_view.getSize().y / 5.f);
	}

	auto target = sf::Vector2f {
		playerCenter.x + (m_view.getSize().x / 4.0f * m_player->lastXDir),
		m_yLevel
	};

	auto cameraPosition = m_sod.Update(dt, target);
	m_view.setCenter(cameraPosition);
}

void Camera::setActive(sf::RenderWindow& win) const
{
	win.setView(m_view);
}

void Camera::im()
{	float duration = 1.0f;
	float timeStep = 0.01f;

	int plots = static_cast<int>(duration / timeStep);


	bool edit = false;
	edit |= ImGui::DragFloat("frequency", &m_frequency, 0.01f);
	edit |= ImGui::DragFloat("damping", &m_damping, 0.01f);
	edit |= ImGui::DragFloat("overshoot", &m_overshoot, 0.01f);

	if (edit)
	{
		m_sod.SetParams(m_frequency, m_damping, m_overshoot);
	}

	auto plotSod = SecondOrderDynamicsF(m_frequency, m_damping, m_overshoot, 0);

	auto lambda = [](void* data, int i) -> float
	{
		auto sod = static_cast<SecondOrderDynamicsF*>(data);
		const auto pos = sod->Update(0.01f, 1);
		return pos * 40 + 40;
	};

	ImGui::PlotLines("SOD",  lambda, &plotSod, plots, 0, nullptr, 0, 100, ImVec2(0, 80));
}
