#pragma once
#include "KeyPressType.hpp"

class Game;
class Entity;


enum class WeaponType
{
	Rifle,
	Shotgun,
	Laser,
	PrismaticSpray
};


class Weapon
{
public:
	Weapon(Game* game, Entity* entity, float fireRate, bool isAutomatic, float recoil = 0.0f)
		: m_Game(game), m_Entity(entity), m_isAutomatic(isAutomatic), m_fireRate(fireRate), m_recoil(recoil) {}
	virtual ~Weapon() = default;
	virtual void shoot(double dt, KeyPressType pressType) = 0;
	virtual void update(double dt) = 0;
	bool canShoot(KeyPressType pressType) const;
	float getTimeUntilNextShot() const;
	float getFireRate() const { return m_fireRate; }
	bool isAutomatic() const { return m_isAutomatic; }

protected:
	Game* m_Game;
	Entity* m_Entity;
	bool m_isAutomatic = false;
	float m_fireRate = 0.0f;
	float m_timeUntilNextShot = 0.0f;
	float m_recoil = 0.0f;
};
