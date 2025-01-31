
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include "PlayerController.hpp"


static int cols = C::RES_X / C::GRID_SIZE;
static int lastLine = C::RES_Y / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win)
	:	bulletHandler(this),
		player(this, 5, 5, Entity::Type::Player),
		camera({C::RES_X / 2.f, C::RES_Y / 2.f}, {C::RES_X / 2.5f, C::RES_Y / 2.5f}),
		m_editMode(false), m_selectedElement(0)
{
	this->win = win;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	world.loadFile("world.sav");
	world.cacheWalls();
	world.cacheEnemies(this);

	camera.setPlayer(&player);

	transparentWall.setSize({C::GRID_SIZE, C::GRID_SIZE});
	transparentWall.setFillColor(sf::Color(0x07ff0788));
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		
	
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {
	if (!win->hasFocus()) return;

	m_inputBuffer.update(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
		&& !ImGui::IsWindowHovered()
		&& !ImGui::IsAnyItemHovered()
		&& !ImGui::IsAnyItemActive()
		&& !ImGui::IsAnyItemFocused())
	{
		if (m_editMode)
		{
			if (m_selectedElement == 0 && world.addWall(cursorGrid.x, cursorGrid.y))
				world.cacheWalls();
			if (m_selectedElement == 1 && world.addEnemy(cursorGrid.x, cursorGrid.y))
				world.cacheEnemies(this);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				if (m_selectedElement == 0 && world.removeWall(cursorGrid.x, cursorGrid.y))
					world.cacheWalls();
				if (m_selectedElement == 1 && world.removeEnemy(cursorGrid.x, cursorGrid.y))
					world.cacheEnemies(this);
			}
		}
	}

	float playerDirX = 0;
	float maxSpeed = 15;

	float jsX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);

	if (std::abs(jsX) > 50.0f)
	{
		playerDirX = jsX / 100.f * maxSpeed;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		playerDirX += -maxSpeed;

	}
	if (std::abs(playerDirX) > 0)
	{
		player.dx = playerDirX;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		playerDirX += maxSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::isButtonPressed(0, 4))
		timeScale = 0.05f;
	else
		timeScale = 1.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) || sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -50) {
		player.getController<PlayerController>()->shoot(dt);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Joystick::isButtonPressed(0, 0)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}
}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	dt *= timeScale;
	pollInput(dt);

	if (player.onGround && m_inputBuffer.hasInput(sf::Keyboard::Space))
	{
		player.dy = -40;
		m_inputBuffer.consumeInput(sf::Keyboard::Space);
	}

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	if (!m_editMode)
	{
		world.update(dt);
		player.update(dt);
		bulletHandler.update(dt);
	}

	afterParts.update(dt);

	camera.update(dt);

	auto cursorPos = sf::Mouse::getPosition(*win);
	cursorGrid.x = cursorPos.x / C::GRID_SIZE;
	cursorGrid.y = cursorPos.y / C::GRID_SIZE;

	transparentWall.setPosition({static_cast<float>(cursorGrid.x) * C::GRID_SIZE, static_cast<float>(cursorGrid.y) * C::GRID_SIZE});
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	auto defaultView = win.getView();

	if (!m_editMode)
		camera.setActive(win);
	beforeParts.draw(win);

	world.draw(win);

	player.draw(win);

	for (sf::RectangleShape& r : rects) 
		win.draw(r);

	if (m_editMode)
		win.draw(transparentWall);

	bulletHandler.draw(win);

	afterParts.draw(win);
	win.setView(defaultView);
}

void Game::onSpacePressed() {
	m_inputBuffer.addInput({sf::Keyboard::Space, 0.15f});
}

void Game::im()
{
	if (!m_editMode && ImGui::Button("Edit Mode"))
	{
		m_editMode = true;
		world.cacheEnemies(this);
	}

	if (m_editMode)
	{
		if (ImGui::Button("Quit Edit Mode")) m_editMode = false;

		static int selectedItem = 0;
		static const char* items[] {"Walls", "Enemy"};
		if (ImGui::Combo("ElementType", &selectedItem, items, IM_ARRAYSIZE(items)))
		{
			m_selectedElement = selectedItem;
			if (selectedItem == 0)
				transparentWall.setFillColor(sf::Color(0x07ff07ff));
			if (selectedItem == 1)
				transparentWall.setFillColor(sf::Color(0x0707ffff));
		}

		if (ImGui::Button("Load data"))
		{
			world.loadFile("world.sav");
			world.cacheWalls();
			world.cacheEnemies(this);
		}
		if (ImGui::Button("Save data")) world.saveFile("world.sav");
	}

	ImGui::LabelText("Bullet count", "%zu", bulletHandler.getBulletCount());
	
	if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		player.im();
		ImGui::Unindent();
	}
	
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		camera.im();
		ImGui::Unindent();
	}
}

