#pragma once
#include <cstdint>
#include <filesystem>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>


class Entity;
class Game;

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

	bool addEnemy(int x, int y);
	void cacheEnemies(Game* game);
	bool removeEnemy(int x, int y);
	bool isEnemyBasePosition(int x, int y);

	Entity* getEnemyAt(float x, float y) const;

	void update(double dt);
	void draw(sf::RenderTarget& win);


	bool loadFile(const std::filesystem::path& filePath);
	void saveFile(const std::filesystem::path& filePath) const;

private:
	void clearEnemies();
	std::vector<sf::Vector2i>		m_enemies;
	std::vector<Entity*>			m_entities;
	std::vector<sf::Vector2i>		m_walls;
	std::vector<sf::RectangleShape> m_wallSprites;
};
