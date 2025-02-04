#pragma once

// used as a replacement for coroutines
class Throttle
{
public:
	Throttle(float interval) : m_interval(interval), m_elapsed(0) {}

	void reset() {
		m_elapsed = 0;
	}

	bool shouldExecute(double deltaTime) {
		m_elapsed += deltaTime;
		if (m_elapsed >= m_interval) {
			m_elapsed = 0;
			return true;
		}
		return false;
	}

private:
	float m_interval;
	double m_elapsed;
};