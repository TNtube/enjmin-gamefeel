#pragma once
#include <memory>
#include <vector>

#include "EntityController.hpp"
#include "KeyPressType.hpp"
#include "weapon/Weapon.hpp"

class PlayerController : public EntityController
{
public:
	PlayerController(Game* game, Entity* entity);
	void update(double dt) override;
	~PlayerController() override = default;
	void shoot(double dt, KeyPressType pressType) const;

	void setWeapon(int index) { m_currentWeapon = m_weapons[index].get(); }
private:
	bool m_lastFrameOnGround = false;

	std::vector<std::unique_ptr<Weapon>> m_weapons;
	Weapon* m_currentWeapon = nullptr;
};
