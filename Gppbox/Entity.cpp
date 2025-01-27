#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"
#include "imgui.h"

Entity::Entity(Game* game, int cx, int cy)
	: sprite(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE)),
	  cx(cx), cy(cy), xr(0), yr(0),
	  xx(0), yy(0), dx(0), dy(0),
	  m_pGame(game)
{
	sprite.setPosition(xx, yy);
	sprite.setFillColor(sf::Color(0xff0707ff));
	
}

void Entity::setCoordinates(float x, float y)
{
	xx = x;
	yy = y;
	cx = static_cast<int>(xx/C::GRID_SIZE);	
	cy = static_cast<int>(yy/C::GRID_SIZE);	
	xr = (xx - cx*C::GRID_SIZE) / C::GRID_SIZE;	
	yr = (yy - cy*C::GRID_SIZE) / C::GRID_SIZE;
}

void Entity::im()
{
	
	float x = xx;
	float y = yy;
	bool edit = false;
	edit |= ImGui::DragFloat("x", &x);
	edit |= ImGui::DragFloat("y", &y);

	
	if (edit) {
		setCoordinates(x, y);
	}

	ImGui::DragFloat("dx", &dx);
	ImGui::DragFloat("dy", &dy);
}

void Entity::update(double dt)
{
	// y movement
	yr += dy * dt;


	// roof check, only reset direction
	if ((m_pGame->world.isWall(cx ,cy-1) || (xr > 0 && m_pGame->world.isWall(cx+1 ,cy-1))) && yr < 0.0f) {
		dy = 0;
	}
	// ground check, reset both direction and position
	if ((m_pGame->world.isWall(cx,cy+1) || (xr > 0 && m_pGame->world.isWall(cx+1 ,cy+1))) && yr>=0.0f) {
		dy = 0;
		yr = 0.0f;
		onGround = true;
	} else
	{
		onGround = false;
	}
	dy += 144.0f * dt;

	while( yr>1 ) { cy++; yr--;}
	while( yr<0 ) {	cy--; yr++;}


	// x movement

	lastXDir = dx > 0 ? 1 : (dx < 0 ? -1 : lastXDir);

	xr += dx * dt;
	dx *= 0.96f;
	if (m_pGame->world.isWall(cx+1,cy) && xr>=0.0f) {
		xr = 0;
		dx = 0;
	}
	if (m_pGame->world.isWall(cx-1,cy) && xr<=0.0f) {
		xr = 0.0f;
		dx = 0;
	}

	while( xr>1 ) {	xr --;	cx ++;}
	while( xr<0 ) {	xr ++;	cx --;}

	xx =(cx+xr) * C::GRID_SIZE;
	yy =(cy+yr) * C::GRID_SIZE;
	sprite.setPosition(xx, yy);
}

void Entity::draw(sf::RenderWindow& win) const
{
	win.draw(sprite);
}
