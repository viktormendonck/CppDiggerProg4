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
//components
#include "TextureComponent.h"
#include "FPSdisplay.h"
#include "PlainTextComponent.h"
#include "RotationComponent.h"
#include "TrashTheCache.h"
//devices
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "ControllerDevice.h"
//commands
#include "MoveCommand.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Fps");
	auto& input = dae::InputManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//add devices

	std::unique_ptr<dae::KeyboardDevice> pKeyboard = std::make_unique<dae::KeyboardDevice>();
	std::unique_ptr<dae::ControllerDevice> pController = std::make_unique<dae::ControllerDevice>(0);

	//add the background
	auto pBackgroundObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pBackground{ dae::ResourceManager::GetInstance().LoadTexture("background.tga") };
	pBackgroundObject->AddComponent(std::make_unique<dae::TextureComponent>(pBackgroundObject.get(), pBackground));
	scene.Add(pBackgroundObject);

	//add the logo
	auto pLogoObject = std::make_shared<dae::GameObject>();
	pLogoObject->GetTransform().SetLocalPosition({ 216, 180});
	std::shared_ptr<dae::Texture2D> pLogo{ dae::ResourceManager::GetInstance().LoadTexture("logo.tga") };
	pLogoObject->AddComponent(std::make_unique<dae::TextureComponent>( pLogoObject.get(), pLogo));
	scene.Add(pLogoObject);

	//add the title text
	auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetLocalPosition({ 80, 20});
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Programming 4 Assignment", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);

	auto pPackmanObject = std::make_shared<dae::GameObject>();
	pPackmanObject->GetTransform().SetLocalPosition({ 100, 100 });
	std::shared_ptr<dae::Texture2D> pPackmanSprite{ dae::ResourceManager::GetInstance().LoadTexture("packman.tga") };
	pPackmanObject->AddComponent(std::make_unique<dae::TextureComponent>(pPackmanObject.get(), pPackmanSprite));
	scene.Add(pPackmanObject);

	MoveCommand* pPackmanMoveRight = new MoveCommand(pPackmanObject.get(), glm::vec2{ 100,0 });
	MoveCommand* pPackmanMoveLeft =	new MoveCommand(pPackmanObject.get(), glm::vec2{ -100,0 });
	MoveCommand* pPackmanMoveUp = new MoveCommand(pPackmanObject.get(), glm::vec2{ 0,-100});
	MoveCommand* pPackmanMoveDown =	new MoveCommand(pPackmanObject.get(), glm::vec2{ 0,100});
	pController->BindCommand(std::shared_ptr<MoveCommand>(pPackmanMoveRight), dae::ControllerDevice::ControllerButton::DPadRight, dae::InputState::Held);
	pController->BindCommand(std::shared_ptr<MoveCommand>(pPackmanMoveLeft), dae::ControllerDevice::ControllerButton::DPadLeft, dae::InputState::Held);
	pController->BindCommand(std::shared_ptr<MoveCommand>(pPackmanMoveUp), dae::ControllerDevice::ControllerButton::DPadUp, dae::InputState::Held);
	pController->BindCommand(std::shared_ptr<MoveCommand>(pPackmanMoveDown), dae::ControllerDevice::ControllerButton::DPadDown, dae::InputState::Held);
	
	auto pMsPackmanObject = std::make_shared<dae::GameObject>();
	pMsPackmanObject->GetTransform().SetLocalPosition({ 50, 100 });
	std::shared_ptr<dae::Texture2D> pMsPackmanSprite{ dae::ResourceManager::GetInstance().LoadTexture("MsPackman.tga") };
	pMsPackmanObject->AddComponent(std::make_unique<dae::TextureComponent>(pMsPackmanObject.get(), pMsPackmanSprite));
	scene.Add(pMsPackmanObject);
	
	MoveCommand* pMsPackmanMoveRight = new MoveCommand(pMsPackmanObject.get(), glm::vec2{ 50,0 });
	MoveCommand* pMsPackmanMoveLeft = new MoveCommand(pMsPackmanObject.get(), glm::vec2{ -50,0 });
	MoveCommand* pMsPackmanMoveUp = new MoveCommand(pMsPackmanObject.get(), glm::vec2{ 0,-50 });
	MoveCommand* pMsPackmanMoveDown = new MoveCommand(pMsPackmanObject.get(), glm::vec2{ 0,50 });
	pKeyboard->BindCommand(std::shared_ptr<MoveCommand>(pMsPackmanMoveRight),SDL_SCANCODE_D, dae::InputState::Held);
	pKeyboard->BindCommand(std::shared_ptr<MoveCommand>(pMsPackmanMoveLeft),SDL_SCANCODE_A, dae::InputState::Held);
	pKeyboard->BindCommand(std::shared_ptr<MoveCommand>(pMsPackmanMoveUp),SDL_SCANCODE_W, dae::InputState::Held);
	pKeyboard->BindCommand(std::shared_ptr<MoveCommand>(pMsPackmanMoveDown),SDL_SCANCODE_S, dae::InputState::Held);
	

	input.AddInputDevice(std::move(pKeyboard));
	input.AddInputDevice(std::move(pController));
	
}

int main(int, char*[]) {
	SDL_SetMainReady();
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}