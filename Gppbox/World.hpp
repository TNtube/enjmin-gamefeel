#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>

#include "Entity.hpp"

class World
{
public:
	enum class CellType : uint8_t
	{
		Wall,
		Enemy
	};

	bool addWall(int x, int y);
	void cacheWalls();
	bool removeWall(int x, int y);
	bool isWall(int x, int y);

	void draw(sf::RenderWindow& win);


	bool loadFile(const std::filesystem::path& filePath);
	void saveFile(const std::filesystem::path& filePath) const;

private:
	std::vector<Entity>				m_entities;
	std::vector<sf::Vector2i>		m_walls;
	std::vector<sf::RectangleShape> m_wallSprites;
};
