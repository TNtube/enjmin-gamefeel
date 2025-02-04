#pragma once
#include "Weapon.hpp"

class Rifle : public Weapon
{
public:
	Rifle(Game* game, Entity* entity);
	void shoot(double dt, KeyPressType pressType) override;
	void update(double dt) override;
};
