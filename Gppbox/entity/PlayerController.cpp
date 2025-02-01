#include "PlayerController.hpp"

#include <iostream>

#include "C.hpp"
#include "Entity.hpp"
#include "Game.hpp"

PlayerController::PlayerController(Game* game, Entity* entity)
	: EntityController(game, entity), m_fireThrottler(0.1f) // magic fire rate number
{
	
}

void PlayerController::update(double dt)
{
	EntityController::update(dt);
	if (m_lastFrameOnGround != m_Entity->onGround && m_Entity->onGround)
	{
		// on land
	}

	m_lastFrameOnGround = m_Entity->onGround;
	if (m_fireThrottler.shouldExecute(dt))
		m_canShoot = true;
}

void PlayerController::shoot(double dt)
{
	// dont shoot if time is frozen
	if (m_canShoot && dt > 0)
	{
		m_canShoot = false;
		sf::Vector2f pos = {m_Entity->xx, m_Entity->yy};
		sf::Vector2f from = {pos.x + (m_Entity->lastXDir > 0 ? C::GRID_SIZE : 0.f), m_Entity->yy + C::GRID_SIZE * (1.f/4.f)};
		auto to = from;
		to.x += m_Entity->lastXDir;
		m_pGame->bulletHandler.shoot(from, to);
		m_Entity->offsetDx = -m_Entity->lastXDir * 5;
	}
}
