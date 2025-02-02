#include "WeaponPicker.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "Game.hpp"
#include "imgui.h"
#include "math/MathUtils.hpp"

WeaponPicker::WeaponPicker(Game* game)
	:	m_game(game), m_center(static_cast<float>(game->win->getSize().x) / 2.f, static_cast<float>(game->win->getSize().y) / 2.f),
		m_cursorSod(8, 1, 0, m_center)
{
	float centerOffset = 150;

	sf::Vector2f north	= {m_center.x, m_center.y - centerOffset};
	sf::Vector2f south	= {m_center.x, m_center.y + centerOffset};
	sf::Vector2f east	= {m_center.x + centerOffset, m_center.y};
	sf::Vector2f west	= {m_center.x - centerOffset, m_center.y};

	m_frames[0] = WeaponFrame(north);
	m_frames[1] = WeaponFrame(east);
	m_frames[2] = WeaponFrame(south);
	m_frames[3] = WeaponFrame(west);

	m_cursor.setPosition(m_center);
	m_cursor.setRadius(10);
	m_cursor.setFillColor(sf::Color::Red);

	m_cursor.setOrigin(5, 5);
}

void WeaponPicker::update(double dt)
{
	sf::Vector2f joystickPosition = {
		sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U),
		sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V)
	};

	auto usedPos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*m_game->win));

	if (m_game->lastInput == InputType::Joystick)
	{
		if (std::abs(joystickPosition.x) > 50 || std::abs(joystickPosition.y) > 50)
		{
			usedPos = {
				m_center.x + joystickPosition.x * 2.0f,
				m_center.y + joystickPosition.y * 2.0f
			};
		}
		else
			usedPos = m_center;
	}

	usedPos.x -= m_cursor.getRadius() / 2.0f;
	usedPos.y -= m_cursor.getRadius() / 2.0f;

	auto pos = m_cursorSod.Update(dt, usedPos);
	m_cursor.setPosition(pos);


	auto direction = usedPos - m_center;
	auto length = MathUtils::length(direction);

	if (length >= 50)
	{
		float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;

		int pickedWeapon;
		if (angle > -135 && angle < -45)
			pickedWeapon = 0; // Up
		else if (angle > -45 && angle < 45)
			pickedWeapon = 1; // Right
		else if (angle > 45 && angle < 135)
			pickedWeapon = 2; // Down
		else
			pickedWeapon = 3; // Left

		if (pickedWeapon != m_pickedWeapon)
		{
			m_pickedWeapon = pickedWeapon;
			for (auto& frame : m_frames)
			{
				frame.setScale(1, 1);
			}
			m_frames[m_pickedWeapon].setScale(1.4f, 1.4f);
		}
	}

	for (auto& frame : m_frames)
	{
		frame.update(dt);
	}
}

void WeaponPicker::draw(sf::RenderTarget& target)
{
	for (auto& frame : m_frames)
	{
		frame.draw(target);
	}

	target.draw(m_cursor);
}

void WeaponPicker::im()
{
	sf::Vector2f cursorPos = m_cursor.getPosition();
	// ImGui::Text("Cursor pos: %f, %f", cursorPos.x, cursorPos.y);
	// ImGui::Text("Cursor lenght to center: %f", MathUtils::length(cursorPos - m_center));
	// ImGui::Text("Picked weapon: %d", m_pickedWeapon);
}

void WeaponPicker::fadeIn()
{
	for (auto& frame : m_frames)
	{
		frame.setScale(1.0f, 1.0f);
		frame.resetPosition();
	}
	m_frames[m_pickedWeapon].setScale(1.4f, 1.4f);
}

void WeaponPicker::fadeOut()
{
	for (auto& frame : m_frames)
	{
		frame.resetScale();
		frame.setPosition(m_center.x, m_center.y);
	}
}
