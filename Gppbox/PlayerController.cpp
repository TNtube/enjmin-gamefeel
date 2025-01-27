#include "PlayerController.hpp"
#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"

void PlayerController::update(double dt)
{
	// y movement
	m_Entity->yr += m_Entity->dy * dt;

	// roof check, only reset direction
	if ((m_pGame->world.isWall(m_Entity->cx ,m_Entity->cy-1) || (m_Entity->xr > 0 && m_pGame->world.isWall(m_Entity->cx+1 ,m_Entity->cy-1))) && m_Entity->yr < 0.0f) {
		m_Entity->dy = 0;
	}
	// ground check, reset both direction and position
	if ((m_pGame->world.isWall(m_Entity->cx,m_Entity->cy+1) || (m_Entity->xr > 0 && m_pGame->world.isWall(m_Entity->cx+1 ,m_Entity->cy+1))) && m_Entity->yr>=0.0f) {
		m_Entity->dy = 0;
		m_Entity->yr = 0.0f;
		m_Entity->onGround = true;
	} else
	{
		m_Entity->onGround = false;
	}
	m_Entity->dy += 144.0f * dt;

	while( m_Entity->yr>1 ) { m_Entity->cy++; m_Entity->yr--;}
	while( m_Entity->yr<0 ) {	m_Entity->cy--; m_Entity->yr++;}

	// x movement
	m_Entity->lastXDir = m_Entity->dx > 0 ? 1 : (m_Entity->dx < 0 ? -1 : m_Entity->lastXDir);

	m_Entity->xr += m_Entity->dx * dt;
	m_Entity->dx *= 0.96f;
	if (m_pGame->world.isWall(m_Entity->cx+1,m_Entity->cy) && m_Entity->xr>=0.0f) {
		m_Entity->xr = 0;
		m_Entity->dx = 0;
	}
	if (m_pGame->world.isWall(m_Entity->cx-1,m_Entity->cy) && m_Entity->xr<=0.0f) {
		m_Entity->xr = 0.0f;
		m_Entity->dx = 0;
	}

	while( m_Entity->xr>1 ) {	m_Entity->xr --;	m_Entity->cx ++;}
	while( m_Entity->xr<0 ) {	m_Entity->xr ++;	m_Entity->cx --;}

	m_Entity->xx =(m_Entity->cx + m_Entity->xr) * C::GRID_SIZE;
	m_Entity->yy =(m_Entity->cy + m_Entity->yr) * C::GRID_SIZE;
	m_Entity->sprite.setPosition(m_Entity->xx, m_Entity->yy);
}
