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
	class ControllerDevice::ControllerDeviceImpl
	{
	public:
		explicit ControllerDeviceImpl(int controllerIndex);

		void ProcessInput();

		void BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button, InputState state);
	private:
		std::vector<std::tuple<std::unique_ptr<Command>, ControllerButton, InputState>> m_Commands;
		const int m_ControllerIndex{};

		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};

		WORD m_PressedThisFrame{};
		WORD m_ReleasedThisFrame{};
	};

	ControllerDevice::ControllerDeviceImpl::ControllerDeviceImpl(int controllerIndex)
		: m_ControllerIndex{controllerIndex}
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	}

	void ControllerDevice::ControllerDeviceImpl::BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button,InputState state)
	{
		m_Commands.emplace_back(std::move(pCommand), button, state);
	}


	void ControllerDevice::ControllerDeviceImpl::ProcessInput()
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
				state == InputState::Released && m_ReleasedThisFrame & static_cast<int>(button))			{
				pCommand->Execute();
			}
		}
		
	}
	ControllerDevice::ControllerDevice(int controllerIndex)	: m_pImpl{std::make_unique<ControllerDeviceImpl>(controllerIndex)}{}
	ControllerDevice::~ControllerDevice() = default;
	void ControllerDevice::ProcessInput()
	{
		m_pImpl->ProcessInput();
	}

	void ControllerDevice::BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button,InputState state)
	{
		m_pImpl->BindCommand(std::move(pCommand), button, state);
	}
}
