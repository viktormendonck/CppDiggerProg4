#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include <utility>
//components
#include "TextureComponent.h"
#include "PlainTextComponent.h"
#include "ScoreDisplays.h"
#include "PlayerController.h"
#include "SpriteSheetComponent.h"
#include "TileMapComponent.h"
//devices
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "ControllerDevice.h"
//commands
#include "PlayerCommands.h"






void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Fps");
	//auto& input = dae::InputManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//add devices

	std::unique_ptr<dae::KeyboardDevice> pKeyboard = std::make_unique<dae::KeyboardDevice>();
	std::unique_ptr<dae::ControllerDevice> pController = std::make_unique<dae::ControllerDevice>(0);

	//add the title text
	const auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetLocalPosition({ 80, 20 });
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Programming 4 \nAssignment", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);

	//test map
	const auto pMapObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pTileSet{ dae::ResourceManager::GetInstance().LoadTexture("tileset.png") };
	pMapObject->GetTransform().SetLocalPosition({ 75, 200 });
	pMapObject->GetTransform().SetLocalScale({ 2,2 });
	pMapObject->AddComponent(std::make_unique<dae::TileMapComponent>(pMapObject.get(), pTileSet,1));
	scene.Add(pMapObject);

	dae::InputManager::GetInstance().AddInputDevice(std::move(pController));
	dae::InputManager::GetInstance().AddInputDevice(std::move(pKeyboard));
}

int main(int, char*[]) {
	SDL_SetMainReady();
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}