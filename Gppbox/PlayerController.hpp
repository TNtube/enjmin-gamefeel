#pragma once
#include "EntityController.hpp"

class PlayerController : public EntityController
{
public:
	PlayerController(Game* game, Entity* entity) : EntityController(game, entity) {}
	void update(double dt) override;
	~PlayerController() override = default;
private:
	bool m_lastFrameOnGround = false;
};
