#include "LaserBeam.h"

#include "C.hpp"
#include "Dice.hpp"
#include "entity/Entity.hpp"
#include "Game.hpp"

LaserBeam::LaserBeam(Game* game, Entity* entity)
	: Weapon(game, entity, 5.0f, false, 15.0f)
{
}

void LaserBeam::shoot(double dt, KeyPressType pressType)
{
	// dont shoot if time is frozen
	if (canShoot(pressType) && dt > 0)
	{
		m_timeUntilNextShot = m_fireRate;
		sf::Vector2f pos = { m_Entity->xx, m_Entity->yy };

		sf::Vector2f from = { pos.x + (m_Entity->lastXDir > 0 ? C::GRID_SIZE : 0.f), m_Entity->yy + C::GRID_SIZE / 2.0f };

		m_Game->bulletHandler.ResetLaserTweens();

		m_Game->bulletHandler.laserStart = from;
		m_Game->bulletHandler.laserOn = true;
		m_Game->bulletHandler.laserDir = {m_Entity->lastXDir, 0};
		
		m_Entity->offsetDx = -m_Entity->lastXDir * m_recoil;
	}
}

void LaserBeam::update(double dt)
{
	m_timeUntilNextShot -= dt;
}
