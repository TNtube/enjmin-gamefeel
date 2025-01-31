#include "InputBuffer.hpp"

void InputBuffer::update(double dt)
{
	for (auto it = m_buffer.begin(); it != m_buffer.end();)
	{
		it->lifetime -= dt;
		if (it->lifetime <= 0)
			it = m_buffer.erase(it);
		else
			++it;
	}
}

void InputBuffer::addInput(InputData data)
{
	m_buffer.push_back(data);
}

void InputBuffer::consumeInput(uint32_t keycode)
{
	for (auto it = m_buffer.begin(); it != m_buffer.end();)
	{
		if (it->keycode == keycode)
			it = m_buffer.erase(it);
		else
			++it;
	}
}

bool InputBuffer::hasInput(uint32_t keycode) const
{
	for (auto inputData : m_buffer)
	{
		if (inputData.keycode == keycode && inputData.lifetime > 0)
			return true;
	}

	return false;
}
