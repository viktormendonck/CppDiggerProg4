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
#include "PlayerComponent.h"
#include "TileMapComponent.h"
//devices
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "ControllerDevice.h"
//commands
#include "PlayerCommands.h"
//sound
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"



void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Fps");

	dae::ServiceLocator::RegisterSoundSystem(std::make_unique<dae::SDLSoundSystem>());
	dae::ServiceLocator::GetSoundSystem().AddSound("Data/Sounds/DeathSound.mp3", dae::SoundSystem::SoundType::SoundEffect);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//add devices


	std::unique_ptr<dae::KeyboardDevice> pKeyboard = std::make_unique<dae::KeyboardDevice>();
	std::unique_ptr<dae::ControllerDevice> pController = std::make_unique<dae::ControllerDevice>(0);

	//add the title text
	const auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetLocalPosition({ 80, 20 });
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Press space for sound", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);

	//test map
	const auto pWorldObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pTileSet{ dae::ResourceManager::GetInstance().LoadTexture("tileset.png") };
	pWorldObject->GetTransform().SetLocalPosition({ 75, 200 });
	pWorldObject->GetTransform().SetLocalScale({ 2,2 });
	pWorldObject->AddComponent(std::make_unique<dae::TileMapComponent>(pWorldObject.get(), pTileSet, glm::ivec2{ 6,3 }, glm::ivec2{ 40,25 }, 0));
	scene.Add(pWorldObject);

	const auto pPlayerObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pPlayerTexture{ dae::ResourceManager::GetInstance().LoadTexture("temp.png") };
	//pPlayerObject->AddComponent(std::make_unique<dae::SpriteSheetComponent>(pPlayerObject.get(), pPlayerTexture, glm::ivec2{ 1,4 }, true, 0.3f, true));
	pPlayerObject->AddComponent(std::make_unique<dae::TextureComponent>(pPlayerObject.get(), pPlayerTexture));
	pPlayerObject->AddComponent(std::make_unique<dae::PlayerComponent>(pPlayerObject.get(), 0));
	pPlayerObject->SetParent(pWorldObject.get(), false);

	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
		SDL_SCANCODE_W,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
		SDL_SCANCODE_S,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
		SDL_SCANCODE_D,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
		SDL_SCANCODE_A,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::KillCommand>(pPlayerObject.get()),
		SDL_SCANCODE_SPACE,
		dae::InputState::Pressed
	);


	dae::InputManager::GetInstance().AddInputDevice(std::move(pController));
	dae::InputManager::GetInstance().AddInputDevice(std::move(pKeyboard));
}

int main(int, char* []) {

	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}