#include "ControllerDevice.h"

#include <iostream> 
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

#include "InputManager.h"
#include "Command.h"

namespace dae
{
	ControllerDevice::ControllerDevice(int controllerIndex)
		: m_ControllerIndex{controllerIndex}
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	}

	void ControllerDevice::ProcessInput()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentState);
		

		const WORD changes(m_PreviousState.Gamepad.wButtons ^ m_CurrentState.Gamepad.wButtons);
		m_PressedThisFrame = changes & m_CurrentState.Gamepad.wButtons;
		m_ReleasedThisFrame = changes & m_PreviousState.Gamepad.wButtons;

		for (const auto& command : m_Commands)
		{
			const auto& pCommand = std::get<0>(command);
			const auto& button = std::get<1>(command);
			const auto& state = std::get<2>(command);
			if (state == InputState::Pressed && m_PressedThisFrame & static_cast<int>(button) ||
				state == InputState::Held && m_CurrentState.Gamepad.wButtons & static_cast<int>(button) ||
				state == InputState::Released && m_ReleasedThisFrame & static_cast<int>(button))
			{
				pCommand->Execute();
			}
		}
		
	}

	void ControllerDevice::BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button,InputState state)
	{
		m_Commands.emplace_back(std::move(pCommand), button, state);
	}
}
