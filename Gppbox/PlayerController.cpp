#include "PlayerController.hpp"

#include "Entity.hpp"
#include "Game.hpp"

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
}
