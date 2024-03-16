#pragma once
#include "InputDevice.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Xinput.h>

namespace dae
{
	class ControllerDevice final :public InputDevice
	{
	public:
        enum class ControllerButton
        {
            DPadUp = 0x0001,
            DPadDown = 0x0002,
            DPadLeft = 0x0004,
            DPadRight = 0x0008,
            Start = 0x0010,
            Back = 0x0020,
            LeftThumb = 0x0040,
            RightThumb = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,
            A = 0x1000,
            B = 0x2000,
            X = 0x4000,
            Y = 0x8000
        };
        explicit ControllerDevice(int controllerIndex);

        ~ControllerDevice() override = default;
        ControllerDevice(const ControllerDevice& other) = delete;
        ControllerDevice(const ControllerDevice&& other) = delete;
        ControllerDevice& operator=(const ControllerDevice& other) = delete;
        ControllerDevice& operator=(const ControllerDevice&& other) = delete;

        void ProcessInput() override;

        void BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button, InputState state);
	private:
        std::vector<std::tuple<std::unique_ptr<Command>, ControllerButton, InputState>> m_Commands;
        const int m_ControllerIndex{};

        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};

        WORD m_PressedThisFrame{};
        WORD m_ReleasedThisFrame{};
	};
}

