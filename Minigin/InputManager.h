#pragma once
#include "ControllerDevice.h"
#include "KeyboardDevice.h"
#include "Singleton.h"
#include "GameObject.h"

namespace dae
{
	class KeyboardDevice;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		int AddInputDevice(std::unique_ptr<ControllerDevice> pDevice);
		void AddInputDevice(std::unique_ptr<KeyboardDevice> pDevice);
		KeyboardDevice* GetKeyboardDevice() const { return m_pKeyboardDevice.get(); }
		ControllerDevice* GetControllerDevice(int index) const { return m_pDevices[index].get(); }
		void ClearCommands();
	private:
		std::vector<std::unique_ptr<ControllerDevice>> m_pDevices;
		std::unique_ptr<KeyboardDevice> m_pKeyboardDevice;
	};
}
