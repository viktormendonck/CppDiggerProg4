#include <SDL.h>
#include "InputManager.h"

#include <stdexcept>

#include "imgui_impl_sdl2.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
		{
			return false;
		}
		// Passing event to ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	for (const auto& pDevice : m_pDevices)
	{
		pDevice->ProcessInput();
	}
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->ProcessInput();
	}

	return true;
}

int dae::InputManager::AddInputDevice(std::unique_ptr<ControllerDevice> pDevice)
{
	m_pDevices.push_back(std::move(pDevice));
	return static_cast<int>(m_pDevices.size() - 1);
}

void dae::InputManager::AddInputDevice(std::unique_ptr<KeyboardDevice> pDevice)
{
	if (m_pKeyboardDevice)
	{
		throw std::runtime_error("already added a keyboard!");
	} else
	{
		m_pKeyboardDevice = std::move(pDevice);
	}
}


void dae::InputManager::ClearCommands()
{
	for (const std::unique_ptr<ControllerDevice>& pDevice : m_pDevices)
	{
		pDevice->ClearCommands();
	}
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice->ClearCommands();
	}
}


