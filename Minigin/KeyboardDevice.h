#pragma once
#include <array>
#include <SDL_scancode.h>

#include "Command.h"
#include "InputDevice.h"
namespace dae
{
    class KeyboardDevice final : public InputDevice
    {
    public:
        
        explicit KeyboardDevice()
        {
			m_CurrentState.fill(0);
			m_LastFrameState.fill(0);
        };

        ~KeyboardDevice() override = default;
        KeyboardDevice(const KeyboardDevice& other) = delete;
        KeyboardDevice(const KeyboardDevice&& other) = delete;
        KeyboardDevice& operator=(const KeyboardDevice& other) = delete;
        KeyboardDevice& operator=(const KeyboardDevice&& other) = delete;

        void ProcessInput() override;

        void BindCommand(std::unique_ptr<Command> pCommand, SDL_Scancode button, InputState state);
        void ClearCommands();
    private:
        
        std::vector<std::tuple<std::unique_ptr<Command>, SDL_Scancode, InputState>> m_Commands;

        std::array<Uint8, SDL_NUM_SCANCODES> m_CurrentState{};
        std::array<Uint8, SDL_NUM_SCANCODES> m_LastFrameState{};
    };
}
