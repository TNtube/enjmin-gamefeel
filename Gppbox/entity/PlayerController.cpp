#include "PlayerController.hpp"

#include "Entity.hpp"
#include "Game.hpp"
#include "KeyPressType.hpp"
#include "weapon/LaserBeam.h"
#include "weapon/Rifle.hpp"
#include "weapon/Shotgun.hpp"

PlayerController::PlayerController(Game* game, Entity* entity)
	: EntityController(game, entity)
{
	auto rifle = std::make_unique<Rifle>(game, entity);
	m_currentWeapon = rifle.get();
	m_weapons.push_back(std::move(rifle));

	m_weapons.push_back(std::make_unique<Shotgun>(game, entity));
	m_weapons.push_back(std::make_unique<LaserBeam>(game, entity));
	m_weapons.push_back(std::make_unique<Shotgun>(game, entity));
}

void PlayerController::update(double dt)
{
	LaserBeam* laser = dynamic_cast<LaserBeam*>(m_currentWeapon);
	int lastXDir = m_Entity->lastXDir;
	if (laser && laser->m_pressed)
	{
		float sign = m_Entity->dx > 0 ? 1 : -1;
		// slow down the player when charging the laser
		m_Entity->dx = std::min(std::abs(m_Entity->dx), 3.f) * sign;
	}
	
	EntityController::update(dt);

	if (laser && laser->m_pressed)
	{
		// prevent direction change while charging the laser
		m_Entity->lastXDir = lastXDir;
	}
	
	if (m_lastFrameOnGround != m_Entity->onGround && m_Entity->onGround)
	{
		// on land
	}

	m_lastFrameOnGround = m_Entity->onGround;

	for (const auto& weapon : m_weapons)
	{
		weapon->update(dt);
	}
}

void PlayerController::shoot(double dt, KeyPressType pressType) const
{
	m_currentWeapon->shoot(dt, pressType);
}

void PlayerController::im()
{
	m_currentWeapon->im();
}
