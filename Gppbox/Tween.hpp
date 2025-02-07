#pragma once
#include <cmath>
#include <cstdint>

#include "Lib.hpp"


enum class Ease : uint8_t
{
	Linear,
	InSine,
	OutSine,
	InOutSine,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InExpo,
	OutExpo,
	InOutExpo,
};

template <typename T>
class Tween
{
public:
	static Tween From(T from)
	{
		Tween tween;
		tween.m_from = from;
		return tween;
	}
	
	Tween& To(T to)
	{
		m_to = to;
		return *this;
	}
	Tween& For(float duration) 
	{
		m_duration = duration;
		return *this;
	}
	Tween& SetEase(Ease type) 
	{
		m_ease = type;
		return *this;
	}

	T Update(float dt);

	bool IsFinished() const { return m_elapsed >= m_duration; }


private:
	float m_elapsed = 0;
	float m_duration = 0;
	Ease m_ease = Ease::Linear;
	T m_from;
	T m_to;
};

template <typename T>
T Tween<T>::Update(float dt)
{
	m_elapsed += dt;

	if (m_elapsed >= m_duration)
		return m_to;

	if (m_ease == Ease::Linear)
		return m_from + (m_to - m_from) * (m_elapsed / m_duration);

	if (m_ease == Ease::InSine)
		return m_from + (m_to - m_from) * (1 - cos(m_elapsed / m_duration * (Lib::pi<float>() / 2)));

	if (m_ease == Ease::OutSine)
		return m_from + (m_to - m_from) * sin(m_elapsed / m_duration * (Lib::pi<float>() / 2));

	if (m_ease == Ease::InOutSine)
		return m_from + (m_to - m_from) * (1 - cos(m_elapsed / m_duration * Lib::pi<float>())) / 2;

	if (m_ease == Ease::InQuad)
		return m_from + (m_to - m_from) * (m_elapsed / m_duration) * (m_elapsed / m_duration);

	if (m_ease == Ease::OutQuad)
	{
		float t = m_elapsed / m_duration;
		return m_from + (m_to - m_from) * t * (2 - t);
	}

	if (m_ease == Ease::InOutQuad)
	{
		float t = m_elapsed / m_duration;
		if (t < 0.5f)
			return m_from + (m_to - m_from) * 2 * t * t;
		
		return m_from + (m_to - m_from) * (-1 + (4 - 2 * t) * t);
	}

	if (m_ease == Ease::InCubic)
	{
		float t = m_elapsed / m_duration;
		return m_from + (m_to - m_from) * t * t * t;
	}

	if (m_ease == Ease::OutCubic)
	{
		float t = m_elapsed / m_duration;
		return m_from + (m_to - m_from) * (t - 1) * (t - 1) * (t - 1) + 1;
	}

	if (m_ease == Ease::InOutCubic)
	{
		float t = m_elapsed / m_duration;
		if (t < 0.5f)
			return m_from + (m_to - m_from) * 4 * t * t * t;
		
		return m_from + (m_to - m_from) * (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
	}

	if (m_ease == Ease::InExpo)
	{
		float t = m_elapsed / m_duration;
		return m_from + (m_to - m_from) * pow(2, 10 * (t - 1));
	}

	if (m_ease == Ease::OutExpo)
	{
		float t = m_elapsed / m_duration;
		return m_from + (m_to - m_from) * (-pow(2, -10 * t) + 1);
	}

	if (m_ease == Ease::InOutExpo)
	{
		float t = m_elapsed / m_duration;
		if (t < 0.5f)
			return m_from + (m_to - m_from) * pow(2, 10 * (2 * t - 1)) / 2;
		
		return m_from + (m_to - m_from) * (-pow(2, -10 * (2 * t - 1)) + 2) / 2;
	}

	return m_to;
}

