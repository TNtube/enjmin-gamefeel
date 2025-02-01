#pragma once

#include <cstdlib>
#include <random>

#define _USE_MATH_DEFINES
#include <cmath>

class Dice {
public:
	static inline constexpr double pi = 3.14159265358979323846;

	static int randSign() {
		return (modernRand() % 2) ? 1 : -1;
	}

	static int roll(int min, int max) {
		return min + modernRand() % (max-min);
	}

	static bool toss() {
		return roll(0, 1) == 1;
	}
	
	static float randF() {
		return 1.0f * static_cast<float>(modernRand()) / RAND_MAX;
	}

	static float angleRad() {
		return randF() * 2 * static_cast<float>(pi);
	}

	static float angleDeg() {
		return randF() * 360;
	}

	static int modernRand()
	{
		static std::random_device device;
		static std::mt19937 rng(device());
		static std::uniform_int_distribution dist(0, RAND_MAX);
		return dist(rng);
	}
};