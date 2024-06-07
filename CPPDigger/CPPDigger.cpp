#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include "ControllerDevice.h"
#include "HighScoreDisplay.h"
#include "InputManager.h"
#include "Minigin.h"

#include "MapData.h"
#include "json.hpp"
#include "ResourceManager.h"

void load()
{
    using namespace dae;
    std::unique_ptr<KeyboardDevice> pKeyboard = std::make_unique<dae::KeyboardDevice>();
    std::unique_ptr<ControllerDevice> pController = std::make_unique<dae::ControllerDevice>(0);
    std::unique_ptr<ControllerDevice> pControllerTwo = std::make_unique<dae::ControllerDevice>(1);

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    levelLoader::OpenMenu();
   
    InputManager::GetInstance().AddInputDevice(std::move(pController));
    InputManager::GetInstance().AddInputDevice(std::move(pControllerTwo));
    InputManager::GetInstance().AddInputDevice(std::move(pKeyboard));
}

int main(int, char* []) {

	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}