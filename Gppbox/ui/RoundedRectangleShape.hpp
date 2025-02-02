#pragma once
#include <SFML/Graphics/Shape.hpp>

// source: https://github.com/SFML/SFML/wiki/Source%3A-Draw-Rounded-Rectangle
class RoundedRectangleShape : public sf::Shape
{
public:
	RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned int cornerPointCount);
	sf::Vector2f getSize() const { return m_size; }
	virtual std::size_t getPointCount() const;
	virtual sf::Vector2f getPoint(std::size_t index) const;

private:
	sf::Vector2f m_size;
	float m_radius;
	unsigned int m_cornerPointCount;
};
