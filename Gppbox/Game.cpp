
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"


static int cols = C::RES_X / C::GRID_SIZE;
static int lastLine = C::RES_Y / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {
	this->win = win;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	walls.push_back(Vector2i(0, lastLine-1));
	walls.push_back(Vector2i(0, lastLine-2));
	walls.push_back(Vector2i(0, lastLine-3));

	walls.push_back(Vector2i(cols-1, lastLine - 1));
	walls.push_back(Vector2i(cols-1, lastLine - 2));
	walls.push_back(Vector2i(cols-1, lastLine - 3));

	walls.push_back(Vector2i(cols >>2, lastLine - 2));
	walls.push_back(Vector2i(cols >>2, lastLine - 3));
	walls.push_back(Vector2i(cols >>2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	cacheWalls();


	entities.emplace_back(this, 5, 5);
	player = &entities.back();

	gameView.setSize(C::RES_X / 4.f, C::RES_Y / 4.f);
	gameView.setCenter(C::RES_X / 2.f, C::RES_Y / 2.f);


	transparentWall.setSize({C::GRID_SIZE, C::GRID_SIZE});
	transparentWall.setFillColor(sf::Color(0x07ff0788));
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(C::GRID_SIZE,C::GRID_SIZE));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
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

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	float playerDirX = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		playerDirX += -15;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		playerDirX += 15;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_editMode && !isWall(cursorGrid.x, cursorGrid.y))
		{
			walls.push_back(cursorGrid);
			cacheWalls();
		}

		if (m_editMode && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			auto found = walls.end();
			for (auto it = walls.begin(); it <= walls.end(); ++it)
			{
				if (*it == cursorGrid)
				{
					found = it;
					break;
				}
			}

			if (found != walls.end())
			{
				walls.erase(found);
				cacheWalls();
			}
		}
	}

	player->dx = playerDirX;
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
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	for (auto& entity : entities)
	{
		entity.update(dt);
	}
	afterParts.update(dt);

	gameView.setCenter({player->xx + C::GRID_SIZE / 2.f, player->yy  + C::GRID_SIZE / 2.f});

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
		win.setView(gameView);
	beforeParts.draw(win);

	for (sf::RectangleShape & r : wallSprites)
		win.draw(r);

	for (Entity& e : entities)
		e.draw(win);

	for (sf::RectangleShape& r : rects) 
		win.draw(r);

	if (m_editMode)
		win.draw(transparentWall);
	

	afterParts.draw(win);
	win.setView(defaultView);
}

void Game::onSpacePressed() {
	if (player->onGround)
		player->dy = -40;
}


bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{

	if (!m_editMode && ImGui::Button("Edit Mode"))
	{
		m_editMode = true;
	}

	if (m_editMode && ImGui::Button("Quit Edit Mode"))
	{
		m_editMode = false;
	}
	
	if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Indent();
		player->im();
		ImGui::Unindent();
	}
}

