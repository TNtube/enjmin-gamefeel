#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"
#include "imgui.h"
#include "PlayerController.hpp"

Entity::Entity(Game* game, int cx, int cy)
	: sprite(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE)),
	  cx(cx), cy(cy), xr(0), yr(0),
	  xx(0), yy(0), dx(0), dy(0)
{
	sprite.setPosition(xx, yy);
	sprite.setFillColor(sf::Color(0xff0707ff));

	m_pController = std::make_unique<PlayerController>(game, this);
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

void Entity::update(double dt) const
{
	m_pController->update(dt);
}

void Entity::draw(sf::RenderWindow& win) const
{
	win.draw(sprite);
}
