#include "Entity.hpp"
#include "Game.hpp"

#include "C.hpp"
#include "EnemyController.hpp"
#include "imgui.h"
#include "PlayerController.hpp"

Entity::Entity(Game* game, int cx, int cy, Type type)
	:	sprite(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE)),
		cx(cx), cy(cy), xr(0), yr(0),
		xx(static_cast<float>(cx) * C::GRID_SIZE), yy(static_cast<float>(cy) * C::GRID_SIZE),
		dx(0), dy(0), offsetDx(0), offsetDy(0), m_type(type)
{
	sprite.setPosition(xx, yy);

	switch (m_type)
	{
	case Type::Player:
		m_pController = std::make_unique<PlayerController>(game, this);
		sprite.setFillColor(sf::Color(0xff0707ff));
		break;
	case Type::Enemy:
		m_pController = std::make_unique<EnemyController>(game, this);
		sprite.setFillColor(sf::Color(0x0707ffff));
		break;
	}
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
