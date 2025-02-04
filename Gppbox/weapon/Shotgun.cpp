#include "Shotgun.hpp"

#include "Dice.hpp"
#include <SFML/System/Vector2.hpp>
#include "C.hpp"

#include "entity/Entity.hpp"
#include "Game.hpp"

Shotgun::Shotgun(Game* game, Entity* entity)
	:	Weapon(game, entity, 0.5f, false, 50)
{
}

void Shotgun::shoot(double dt, KeyPressType pressType)
{
	// dont shoot if time is frozen
	if (canShoot(pressType) && dt > 0)
	{
		m_timeUntilNextShot = m_fireRate;
		sf::Vector2f pos = {m_Entity->xx, m_Entity->yy};

		sf::Vector2f from = {pos.x + (m_Entity->lastXDir > 0 ? C::GRID_SIZE : 0.f), m_Entity->yy + C::GRID_SIZE * (1.f/4.f)};
		auto to = from;
		to.x += m_Entity->lastXDir;

		float yOffset = (Dice::randF() * 2.0f - 1.0f) * 5.0f;
		from.y += yOffset;
		to.y += yOffset;
		m_Game->bulletHandler.shoot(from, to);

		float fromOffset = 5;
		float toOffset = 0.05f;
		from.y += fromOffset;
		to.y += fromOffset + toOffset * 2;
		m_Game->bulletHandler.shoot(from, to);

		from.y -= fromOffset * 2;
		to.y -= toOffset * 4 + fromOffset * 2;
		m_Game->bulletHandler.shoot(from, to);
		
		m_Entity->offsetDx = -m_Entity->lastXDir * m_recoil;
	}
}

void Shotgun::update(double dt)
{
	m_timeUntilNextShot -= dt;
}
