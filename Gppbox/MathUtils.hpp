#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

class MathUtils
{
public:
	static sf::Vector2f normalize(const sf::Vector2f& source)
	{
		float length = std::sqrt((source.x * source.x) + (source.y * source.y));
		if (length != 0)
			return {source.x / length, source.y / length};
		
		return source;
	}
};
