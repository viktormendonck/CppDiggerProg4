#pragma once
#include <SDL_syswm.h>

#include "Singleton.h"
#include "GameObject.h"
#include "InputDevice.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void AddInputDevice(std::unique_ptr<InputDevice> pDevice);
	private:
		std::vector<std::unique_ptr<InputDevice>> m_pDevices;
	};

}
