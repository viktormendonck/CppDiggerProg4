#include "KeyboardDevice.h"

#include <iostream>
#include <SDL_events.h>

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

		for (const auto& [pCommand, button, state] : m_Commands)
		{
			if (state == InputState::Pressed && (m_CurrentState[button] && !m_LastFrameState[button]) ||
				state == InputState::Held && (m_CurrentState[button] && m_LastFrameState[button]) ||
				state == InputState::Released && (!m_CurrentState[button] && m_LastFrameState[button]))
			{
				pCommand->Execute();
			}
		}	
	}

	void KeyboardDevice::BindCommand(const std::shared_ptr<Command>& pCommand, SDL_Scancode button, InputState state)
	{
		m_Commands.emplace_back(pCommand, button, state);
	}
}
