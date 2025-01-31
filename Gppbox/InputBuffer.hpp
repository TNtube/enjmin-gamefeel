#pragma once
#include <vector>

class InputBuffer
{
public:
	// jic, 10 input reserved to avoid useless realloc
	InputBuffer() : m_buffer(10) {}
	struct InputData
	{
		uint32_t keycode;
		float lifetime;
	};

	void update(double dt);

	void addInput(InputData data);
	bool hasInput(uint32_t keycode) const;
	void consumeInput(uint32_t keycode);

private:
	std::vector<InputData> m_buffer;
};
