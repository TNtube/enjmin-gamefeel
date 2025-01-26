#pragma once

#include <chrono>
#include <ctime>
#include <SFML/Graphics/Color.hpp>



using namespace std::chrono;

namespace Lib {
	double getTimeStamp();
	template <typename T>
	constexpr T pi() { return static_cast<T>(3.141592653589793238462643383279502884); }
	constexpr double pi() { return 3.141592653589793238462643383279502884; }
	sf::Color makeFromHSV(float _h, float _s, float _v);
};