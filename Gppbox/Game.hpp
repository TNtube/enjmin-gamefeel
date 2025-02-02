#pragma once

#include <vector>

#include "BulletHandler.hpp"
#include "Camera.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "ParticleMan.hpp"

#include "entity/Entity.hpp"
#include "InputBuffer.hpp"
#include "World.hpp"
#include "HotReloadShader.hpp"
#include "ui/WeaponPicker.hpp"

using namespace sf;

enum class InputType : uint8_t
{
	Keyboard,
	Joystick
};

class HotReloadShader;
class Game {
public:
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;

	World							world;
	BulletHandler					bulletHandler;

	sf::RectangleShape				transparentWall;
	sf::Vector2i					cursorGrid;

	Entity							player;

	InputType						lastInput = InputType::Keyboard;

	// camera data
	Camera							camera;

	float							timeScale = 1.0f;
	double							unscaledDt = 0.0;

	// ui
	WeaponPicker					weaponPicker;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Game(sf::RenderWindow * win);

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	void im();

private:
	bool m_editMode;
	int m_selectedElement;
	InputBuffer m_inputBuffer;

	bool m_blurActive = false;

	HotReloadShader m_blurShader;
	sf::Texture winTex;
	sf::RenderTexture m_blurTexture;
	float m_blurFactor = 0.0f;

	float m_blurAnimCounter = 1000.f;

	bool m_pickerActive = false;
};