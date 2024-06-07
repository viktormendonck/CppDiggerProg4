
#include "KeyboardDevice.h"

#include <iostream>
#include <SDL_events.h>

#include "ControllerDevice.h"

namespace dae
{
	void dae::KeyboardDevice::ProcessInput()
	{
		m_LastFrameState = m_CurrentState;

		SDL_PumpEvents();

		const Uint8* pCurrentKeys = SDL_GetKeyboardState(nullptr);

		for (int i{}; i < SDL_NUM_SCANCODES; ++i)
		{
			m_CurrentState[i] = pCurrentKeys[i];
		}

		for (const auto& command : m_Commands)
		{
			const auto& pCommand = std::get<0>(command);
			const auto& button = std::get<1>(command);
			const auto& state = std::get<2>(command);

			if (state == InputState::Pressed && (m_CurrentState[button] && !m_LastFrameState[button]) ||
				state == InputState::Held && (m_CurrentState[button] && m_LastFrameState[button]) ||
				state == InputState::Released && (!m_CurrentState[button] && m_LastFrameState[button]))
			{
				pCommand->Execute();
			}
		}	
	}
	void KeyboardDevice::BindCommand(std::unique_ptr<Command> pCommand, SDL_Scancode button, InputState state)
	{
		m_Commands.emplace_back(std::move(pCommand), button, state);
	}

	void KeyboardDevice::ClearCommands()
	{
		m_Commands.clear();
	}
}
