#include "World.hpp"

#include <fstream>
#include <sstream>

#include "C.hpp"

bool World::addWall(int x, int y)
{
	if (isWall(x, y)) return false;

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
		cacheWalls();
	}

	return true;
}

void World::cacheWalls()
{
	m_wallSprites.clear();
	for (sf::Vector2i & w : m_walls) {
		sf::RectangleShape rect(sf::Vector2f(C::GRID_SIZE,C::GRID_SIZE));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		m_wallSprites.push_back(rect);
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

void World::draw(sf::RenderWindow& win)
{
	for (sf::RectangleShape & r : m_wallSprites)
		win.draw(r);
}

bool World::loadFile(const std::filesystem::path& filePath)
{
	if (!std::filesystem::exists(filePath)) return false;

	std::ifstream in(filePath);

	m_walls.clear();

	
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
	}

	cacheWalls();
	return true;
}

void World::saveFile(const std::filesystem::path& filePath) const
{
	std::ofstream out(filePath);

	for (auto wall : m_walls)
	{
		out << wall.x << " " << wall.y << " " << static_cast<int>(CellType::Wall)<<"\n";
	}
}
