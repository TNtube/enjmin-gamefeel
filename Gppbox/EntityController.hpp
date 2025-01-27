#pragma once

class Game;
class Entity;

class EntityController
{
public:
	EntityController(Game* game, Entity* entity) : m_pGame(game), m_Entity(entity) {}
	virtual void update(double dt) = 0;
	virtual ~EntityController() = default;
protected:
	Game* m_pGame;
	Entity* m_Entity;
};
