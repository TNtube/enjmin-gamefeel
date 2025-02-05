#pragma once
#include "Weapon.hpp"

class LaserBeam : public Weapon
{
public:
	LaserBeam(Game* game, Entity* entity);
	void shoot(double dt, KeyPressType pressType) override;
	void update(double dt) override;
};
