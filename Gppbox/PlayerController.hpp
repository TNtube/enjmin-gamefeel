#pragma once
#include "EntityController.hpp"

class PlayerController : public EntityController
{
public:
	PlayerController(Game* game, Entity* entity) : EntityController(game, entity) {}
	~PlayerController() override = default;
};
