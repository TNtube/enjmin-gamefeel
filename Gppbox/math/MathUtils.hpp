#pragma once
#include <SFML/System/Vector2.hpp>

class MathUtils
{
public:
	static float length(const sf::Vector2f& source);
	static sf::Vector2f normalize(const sf::Vector2f& source);
	static float dot(sf::Vector2f a, sf::Vector2f b);
	static sf::Vector2f slerp(sf::Vector2f start, sf::Vector2f end, float percent);
};
