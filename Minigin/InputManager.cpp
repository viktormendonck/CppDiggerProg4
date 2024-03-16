#include <SDL.h>
#include "InputManager.h"

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

	return true;
}

void dae::InputManager::AddInputDevice(std::unique_ptr<InputDevice> pDevice)
{
	m_pDevices.push_back(std::move(pDevice));
}
