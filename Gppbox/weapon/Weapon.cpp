#include "Weapon.hpp"

bool Weapon::canShoot(KeyPressType pressType) const
{
	bool shouldShoot = m_timeUntilNextShot <= 0;

	if (m_isAutomatic)
	{
		if (pressType != KeyPressType::Released)
			return shouldShoot;
	}
	else
	{
		if (pressType == KeyPressType::Pressed)
			return shouldShoot;
	}
	return false;
}

float Weapon::getTimeUntilNextShot() const
{
	return m_timeUntilNextShot;
}
