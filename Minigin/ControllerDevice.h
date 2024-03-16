#pragma once
#include "InputDevice.h"

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

        ~ControllerDevice() override;
        ControllerDevice(const ControllerDevice& other) = delete;
        ControllerDevice(const ControllerDevice&& other) = delete;
        ControllerDevice& operator=(const ControllerDevice& other) = delete;
        ControllerDevice& operator=(const ControllerDevice&& other) = delete;

        void ProcessInput() override;

        void BindCommand(std::unique_ptr<Command> pCommand, ControllerButton button, InputState state);
	private:

        class ControllerDeviceImpl;
        std::unique_ptr<ControllerDeviceImpl> m_pImpl;
	};
}

