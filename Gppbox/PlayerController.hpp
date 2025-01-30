#pragma once
#include "EntityController.hpp"
#include "Throttle.h"

class PlayerController : public EntityController
{
public:
	PlayerController(Game* game, Entity* entity);
	void update(double dt) override;
	~PlayerController() override = default;
	void shoot(double dt);
private:
	bool m_lastFrameOnGround = false;
	Throttle m_fireThrottler;
};
