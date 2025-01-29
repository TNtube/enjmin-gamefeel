#include "PlayerController.hpp"
#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"

void PlayerController::update(double dt)
{
	// y movement
	m_Entity->yr += m_Entity->dy * dt;

	do
	{
		// ground check, reset both direction and position
		if ((m_pGame->world.isWall(m_Entity->cx,m_Entity->cy+1) || (m_Entity->xr > 0 && m_pGame->world.isWall(m_Entity->cx+1 ,m_Entity->cy+1))) && m_Entity->yr>=0.0f) {
			m_Entity->dy = 0;
			m_Entity->yr = 0.0f;
			m_Entity->onGround = true;
			break;
		}

		m_Entity->onGround = false;

		if (m_Entity->yr > 1) { m_Entity->cy++; m_Entity->yr--;}
	}
	while( m_Entity->yr>1 );

	do
	{	// roof check, only reset direction
		if ((m_pGame->world.isWall(m_Entity->cx ,m_Entity->cy-1) || (m_Entity->xr > 0 && m_pGame->world.isWall(m_Entity->cx+1 ,m_Entity->cy-1))) && m_Entity->yr < 0.0f) {
			m_Entity->dy = 0;
			break;
		}

		if (m_Entity->yr <0) { m_Entity->cy--; m_Entity->yr++; }
	}
	while( m_Entity->yr<0 );
	
	m_Entity->dy += 144.0f * dt;

	// x movement
	m_Entity->lastXDir = m_Entity->dx > 0 ? 1 : (m_Entity->dx < 0 ? -1 : m_Entity->lastXDir);

	m_Entity->xr += m_Entity->dx * dt;
	m_Entity->dx *= 0.96f;

	do
	{
		if (m_pGame->world.isWall(m_Entity->cx+1,m_Entity->cy) && m_Entity->xr>=0.0f) {
			m_Entity->xr = 0;
			m_Entity->dx = 0;
		}
		if (m_Entity->xr>1) { m_Entity->xr --; m_Entity->cx ++; }
	}
	while( m_Entity->xr>1 );

	do
	{
		if (m_pGame->world.isWall(m_Entity->cx-1,m_Entity->cy) && m_Entity->xr<=0.0f) {
			m_Entity->xr = 0.0f;
			m_Entity->dx = 0;
		}
		if (m_Entity->xr < 0) { m_Entity->xr ++; m_Entity->cx --; }
	}
	while( m_Entity->xr<0 );

	m_Entity->xx =(m_Entity->cx + m_Entity->xr) * C::GRID_SIZE;
	m_Entity->yy =(m_Entity->cy + m_Entity->yr) * C::GRID_SIZE;
	m_Entity->sprite.setPosition(m_Entity->xx, m_Entity->yy);
}
