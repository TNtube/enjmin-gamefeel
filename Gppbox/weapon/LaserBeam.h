#pragma once
#include "Weapon.hpp"

class LaserBeam : public Weapon
{
public:
	LaserBeam(Game* game, Entity* entity, int idx);
	void shoot(double dt, KeyPressType pressType) override;
	void update(double dt) override;
	void im() override;

private:
	friend class PlayerController;
	bool m_shoot = false;
	bool m_pressed = false;
	float m_pressedTime = 0;
	float m_maxTime = 1.5f;
};
