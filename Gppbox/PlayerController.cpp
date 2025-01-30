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
		// intensity is excessively high because the camera sod will never reach the target
		// m_pGame->camera.addShake(0.1f, 200.f);
	}

	m_lastFrameOnGround = m_Entity->onGround;
	if (m_fireThrottler.shouldExecute(dt))
		m_canShoot = true;
}

void PlayerController::shoot(double dt)
{
	if (m_canShoot)
	{
		m_canShoot = false;
		sf::Vector2f pos = {m_Entity->xx, m_Entity->yy};
		sf::Vector2f from = {pos.x + (m_Entity->lastXDir > 1 ? C::GRID_SIZE : 0.f), m_Entity->yy + C::GRID_SIZE * (1.f/3.f)};
		auto to = from;
		to.x += m_Entity->lastXDir;
		m_pGame->bulletHandler.shoot(from, to);
	}
}
