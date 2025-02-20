#include "MathUtils.hpp"
#include <cmath>
#include <algorithm>

float MathUtils::length(const sf::Vector2f& source)
{
	return std::sqrt((source.x * source.x) + (source.y * source.y));
}

sf::Vector2f MathUtils::normalize(const sf::Vector2f& source)
{
	float length = std::sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
		return {source.x / length, source.y / length};
		
	return source;
}

float MathUtils::dot(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

// unity implementation
sf::Vector2f MathUtils::slerp(sf::Vector2f start, sf::Vector2f end, float percent)
{
	float d = dot(start, end);
	d = std::clamp(d, -1.0f, 1.0f);
	float theta = std::acos(d) * percent;
	sf::Vector2f RelativeVec = normalize(end - start * d);
	return start*std::cos(theta) + RelativeVec * std::sin(theta);
}
