#include "LaserBeam.h"

#include "C.hpp"
#include "Dice.hpp"
#include "entity/Entity.hpp"
#include "Game.hpp"
#include "imgui.h"

LaserBeam::LaserBeam(Game* game, Entity* entity)
	: Weapon(game, entity, 5.0f, false, 15.0f)
{
}

void LaserBeam::shoot(double dt, KeyPressType pressType)
{
	if (pressType == KeyPressType::Pressed && m_timeUntilNextShot <= 0)
	{
		m_pressed = true;
		return;
	}
	
	sf::Vector2f pos = { m_Entity->xx, m_Entity->yy };
	sf::Vector2f from = { pos.x + (m_Entity->lastXDir > 0 ? C::GRID_SIZE : 0.f), m_Entity->yy + C::GRID_SIZE / 2.0f };
	m_Game->bulletHandler.laserStart = from;
	
	if (m_pressed && pressType == KeyPressType::Released && m_timeUntilNextShot <= 0)
	{
		m_pressed = false;
		m_shoot = true;
	}
	
	if (m_shoot && dt > 0)
	{
		m_timeUntilNextShot = m_fireRate;

		m_Game->bulletHandler.m_targetHeight = 3.0f + m_pressedTime * 15.0f;
		m_Game->bulletHandler.m_laserShake = 100.0f + m_pressedTime * 150.0f;
		m_Game->bulletHandler.ResetLaserTweens(0.5f + m_pressedTime);
		m_Game->bulletHandler.laserOn = true;
		m_Game->bulletHandler.laserDir = {static_cast<float>(m_Entity->lastXDir), 0};
		
		m_Entity->offsetDx = -m_Entity->lastXDir * m_recoil * m_pressedTime;

		m_shoot = false;
		m_pressedTime = 0;
	}
}

void LaserBeam::update(double dt)
{
	m_timeUntilNextShot -= dt;
	
	if (m_pressed)
	{
		m_pressedTime += dt;
		m_pressedTime = std::min(m_maxTime, m_pressedTime);
		m_Game->camera.addShake(dt*2, 2.f);

		m_Game->bulletHandler.m_ballSize = m_pressedTime * 10.0f;
		m_Game->bulletHandler.m_laserPower = m_pressedTime;
		m_Game->bulletHandler.GenerateLaserHead(m_Game->bulletHandler.laserStart + sf::Vector2f(15 * m_Entity->lastXDir, 0));
	}
}

void LaserBeam::im()
{
	Weapon::im();
	ImGui::DragFloat("Max time", &m_maxTime);
	ImGui::DragFloat("Pressed time", &m_pressedTime);
	ImGui::Text("Pressed: %d", m_pressed);
	ImGui::Text("Shoot: %d", m_shoot);
}
