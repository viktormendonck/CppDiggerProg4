#pragma once

#include "Command.h"

namespace dae
{
	enum class InputState
	{
		Pressed,
		Held,
		Released
	};
	class InputDevice
	{
	public:
		InputDevice() = default;
		virtual ~InputDevice() = default;

		InputDevice(const InputDevice&) = delete;
		InputDevice(InputDevice&&) noexcept = delete;
		InputDevice& operator=(const InputDevice&) = delete;
		InputDevice& operator=(InputDevice&&) noexcept = delete;

		virtual void ProcessInput() = 0;
	};
}

