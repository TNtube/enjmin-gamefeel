#include "EntityController.hpp"

#include "C.hpp"
#include "Entity.hpp"
#include "Game.hpp"

void EntityController::update(double dt)
{
	// collision detection is done using grid coordinates.
	// The collision test is done then the movement is applied,
	// while the direction value is not 0.
	// if a collision is detected, we stop the movement
	
	// x movement
	m_Entity->lastXDir = m_Entity->dx > 0 ? 1 : (m_Entity->dx < 0 ? -1 : m_Entity->lastXDir);

	m_Entity->xr += m_Entity->dx * dt;
	m_Entity->dx *= 0.96f;

	// right
	while (m_Entity->xr > 0)
	{
		if (isWallCollision(m_Entity->cx+1, m_Entity->cy, true)) {
			m_Entity->xr = 0;
			m_Entity->dx = 0;
			break;
		}
		m_Entity->xr--; m_Entity->cx++;
	}

	// left
	while (m_Entity->xr < 0.0f)
	{
		if (isWallCollision(m_Entity->cx-1, m_Entity->cy, true)) {
			m_Entity->xr = 0.0f;
			m_Entity->dx = 0;
			break;
		}
		m_Entity->xr++; m_Entity->cx--;
	}

	// y movement
	m_Entity->yr += m_Entity->dy * dt;

	// down
	while(m_Entity->yr > 0.0f)
	{
		// ground check, reset both direction and position
		if (isWallCollision(m_Entity->cx, m_Entity->cy+1, false)) {
			m_Entity->dy = 0;
			m_Entity->yr = 0.0f;
			m_Entity->onGround = true;
			break;
		}
		m_Entity->onGround = false;
		m_Entity->cy++; m_Entity->yr--;
	}

	// up
	while(m_Entity->yr < 0.0f)
	{
		// roof check, only reset direction
		if (isWallCollision(m_Entity->cx, m_Entity->cy-1, false)) {
			m_Entity->dy = 0;
			m_Entity->yr = 0.0f;
			break;
		}
		m_Entity->cy--; m_Entity->yr++;
	}

	m_Entity->dy += C::GRAVITY * dt;

	m_Entity->xx =(m_Entity->cx + m_Entity->xr) * C::GRID_SIZE;
	m_Entity->yy =(m_Entity->cy + m_Entity->yr) * C::GRID_SIZE;
	m_Entity->sprite.setPosition(m_Entity->xx, m_Entity->yy);
}

bool EntityController::isWallCollision(int cx, int cy, bool isXAxis) const
{
	if (isXAxis)
		return m_pGame->world.isWall(cx,cy) || (m_Entity->yr > 0 && m_pGame->world.isWall(cx ,cy+1));
	return m_pGame->world.isWall(cx,cy) || (m_Entity->xr > 0 && m_pGame->world.isWall(cx+1 ,cy));
}