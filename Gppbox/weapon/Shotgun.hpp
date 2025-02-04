#pragma once
#include "Weapon.hpp"

class Shotgun : public Weapon
{
public:
	Shotgun(Game* game, Entity* entity);
	void shoot(double dt, KeyPressType pressType) override;
	void update(double dt) override;
	
};
