#pragma once

class Game;
class Entity;

class EntityController
{
public:
	EntityController(Game* game, Entity* entity) : m_pGame(game), m_Entity(entity) {}
	virtual void update(double dt);
	virtual ~EntityController() = default;
private:
	bool isWallCollision(int cx, int cy, bool isXAxis) const;
protected:
	Game* m_pGame;
	Entity* m_Entity;
};
