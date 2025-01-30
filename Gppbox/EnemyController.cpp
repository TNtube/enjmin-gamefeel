#include "EnemyController.hpp"
#include "Entity.hpp"

void EnemyController::update(double dt)
{
	EntityController::update(dt);

	if (m_Entity->dx == 0.0f)
	{
		m_Direction = -m_Direction;
	}


	m_Entity->dx = m_Direction * m_Speed;
}
