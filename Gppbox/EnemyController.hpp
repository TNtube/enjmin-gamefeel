#pragma once

#include "EntityController.hpp"

class EnemyController : public EntityController
{
public:
	EnemyController(Game* game, Entity* entity) : EntityController(game, entity) {}
	void update(double dt) override;
	~EnemyController() override = default;

private:
	float m_Direction = 1.f;
	float m_Speed = 5.f;
};
