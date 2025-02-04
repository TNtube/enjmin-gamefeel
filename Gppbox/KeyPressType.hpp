#pragma once
#include <cstdint>

enum class KeyPressType : uint8_t
{
	Pressed,
	Repeated,
	Released
};
