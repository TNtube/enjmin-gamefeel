#include "World.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "C.hpp"
#include "entity/Entity.hpp"
#include "Game.hpp"

namespace
{
	namespace WallNeighbourhood
	{
		enum Value : uint8_t
		{
			Null		= 0,
			Top			= 1 << 0,
			Right		= 1 << 1,
			Bottom		= 1 << 2,
			Left		= 1 << 3,
		};
	}

	const std::unordered_map<uint32_t, Vector2f> textureCoords = {
		{WallNeighbourhood::Null, {18, 16}},
		{WallNeighbourhood::Top, {18, 15}},
		{WallNeighbourhood::Bottom, {18, 13}},
		{WallNeighbourhood::Right, {15, 16}},
		{WallNeighbourhood::Left, {17, 16}},
		{WallNeighbourhood::Left | WallNeighbourhood::Right, {16, 16}},
		{WallNeighbourhood::Top | WallNeighbourhood::Bottom, {18, 14}},
		{WallNeighbourhood::Top | WallNeighbourhood::Right, {15, 15}},
		{WallNeighbourhood::Top | WallNeighbourhood::Left, {17, 15}},
		{WallNeighbourhood::Bottom | WallNeighbourhood::Left, {17, 13}},
		{WallNeighbourhood::Bottom | WallNeighbourhood::Right, {15, 13}},
		{WallNeighbourhood::Top | WallNeighbourhood::Bottom | WallNeighbourhood::Right, {15, 14}},
		{WallNeighbourhood::Left | WallNeighbourhood::Top | WallNeighbourhood::Right, {16, 15}},
		{WallNeighbourhood::Top | WallNeighbourhood::Bottom | WallNeighbourhood::Left, {17, 14}},
		{WallNeighbourhood::Left | WallNeighbourhood::Right | WallNeighbourhood::Bottom, {16, 13}},
		{WallNeighbourhood::Left | WallNeighbourhood::Right | WallNeighbourhood::Top | WallNeighbourhood::Bottom, {18, 18}},
	};

	Vector2f getTextureCoordFromWallPosition(WallNeighbourhood::Value neighbourhood)
	{
		for (const auto& coord : textureCoords)
		{
			if (neighbourhood == coord.first)
				return coord.second;
		}

		return textureCoords.at(WallNeighbourhood::Null);
	}
}

World::World()
{
	if (!m_wallTexture.loadFromFile("res/textures/world_tilemap.png"))
		std::cerr << "Failed to load wall texture\n";
}

bool World::addWall(int x, int y)
{
	if (isWall(x, y) || isEnemyBasePosition(x, y)) return false;

	m_walls.emplace_back(x, y);

	return true;
}

bool World::removeWall(int x, int y)
{
	if (!isWall(x, y)) return false;

	auto position = sf::Vector2i{x, y};
	
	auto found = m_walls.end();
	for (auto it = m_walls.begin(); it <= m_walls.end(); ++it)
	{
		if (*it == position)
		{
			found = it;
			break;
		}
	}

	if (found != m_walls.end())
	{
		m_walls.erase(found);
	}

	return true;
}

void World::cacheWalls()
{
	m_wallSprites.clear();
	for (sf::Vector2i & w : m_walls) {
		constexpr int textureSize = 16;

		sf::Sprite sprite(m_wallTexture);
		uint8_t neighbourhood = WallNeighbourhood::Null;
		if (isWall(w.x, w.y - 1)) neighbourhood |= WallNeighbourhood::Top;
		if (isWall(w.x + 1, w.y)) neighbourhood |= WallNeighbourhood::Right;
		if (isWall(w.x, w.y + 1)) neighbourhood |= WallNeighbourhood::Bottom;
		if (isWall(w.x - 1, w.y)) neighbourhood |= WallNeighbourhood::Left;

		auto texCoord = getTextureCoordFromWallPosition(static_cast<WallNeighbourhood::Value>(neighbourhood));
		sprite.setTextureRect({static_cast<int>(texCoord.x * textureSize), static_cast<int>(texCoord.y * textureSize), textureSize, textureSize});
		sprite.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		m_wallSprites.push_back(sprite);
	}
}

bool World::isWall(int x, int y)
{
	for (sf::Vector2i & w : m_walls) {
		if (w.x == x && w.y == y)
			return true;
	}
	return false;
}

bool World::isEnemyBasePosition(int x, int y)
{
	for (sf::Vector2i & w : m_enemies) {
		if (w.x == x && w.y == y)
			return true;
	}
	return false;
}

Entity* World::getEnemyAt(float x, float y) const
{
	for (auto entt : m_entities)
	{
		auto topLeft = sf::Vector2f{entt->xx, entt->yy};
		auto bottomRight = sf::Vector2f{entt->xx + C::GRID_SIZE, entt->yy + C::GRID_SIZE};

		if (x >= topLeft.x && x <= bottomRight.x && y >= topLeft.y && y <= bottomRight.y)
			return entt;
	}

	return nullptr;
}

void World::update(double dt)
{
	for (auto it = m_entities.begin(); it != m_entities.end();)
	{
		auto entt = *it;
		entt->update(dt);
		if (entt->pv <= 0)
		{
			it = m_entities.erase(it);
			delete entt;
		}
		else
			++it;
	}
}

bool World::addEnemy(int x, int y)
{
	if (isWall(x, y) || isEnemyBasePosition(x, y)) return false;
	m_enemies.emplace_back(x, y);
	return true;
}

void World::cacheEnemies(Game* game)
{
	clearEnemies();
	m_entities.reserve(m_enemies.size());
	for (auto enemyPos : m_enemies)
	{
		auto entt = new Entity(game, enemyPos.x, enemyPos.y, Entity::Type::Enemy);
		m_entities.push_back(entt);
	}
}

bool World::removeEnemy(int x, int y)
{
	if (!isEnemyBasePosition(x, y)) return false;

	auto position = sf::Vector2i{x, y};
	
	auto found = m_enemies.end();
	for (auto it = m_enemies.begin(); it <= m_enemies.end(); ++it)
	{
		if (*it == position)
		{
			found = it;
			break;
		}
	}

	if (found != m_enemies.end())
		m_enemies.erase(found);

	return true;
}

void World::draw(sf::RenderTarget& win) const
{
	for (auto & r : m_wallSprites)
		win.draw(r);

	for (auto entt : m_entities)
		entt->draw(win);
}

bool World::loadFile(const std::filesystem::path& filePath)
{
	if (!std::filesystem::exists(filePath)) return false;

	std::ifstream in(filePath);

	m_walls.clear();
	clearEnemies();

	
	std::string line;
	while (std::getline(in, line))
	{
		std::istringstream iss(line);
		int x, y, type;
		if (!(iss >> x >> y >> type)) { break; } // error

		if (static_cast<CellType>(type) == CellType::Wall)
		{
			m_walls.emplace_back(x, y);
		}

		if (static_cast<CellType>(type) == CellType::Enemy)
		{
			m_enemies.emplace_back(x, y);
		}
	}
	return true;
}

void World::saveFile(const std::filesystem::path& filePath) const
{
	std::ofstream out(filePath);

	for (auto wall : m_walls)
	{
		out << wall.x << " " << wall.y << " " << static_cast<int>(CellType::Wall)<<"\n";
	}
	for (auto entt : m_enemies)
	{
		out << entt.x << " " << entt.y << " " << static_cast<int>(CellType::Enemy)<<"\n";
	}
}

void World::clearEnemies()
{
	for (auto entt : m_entities)
		delete entt;
	m_entities.clear();
}
