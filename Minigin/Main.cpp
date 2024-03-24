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
//devices
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "ControllerDevice.h"
//commands
#include "PlayerCommands.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Fps");
	auto& input = dae::InputManager::GetInstance();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//add devices

	std::unique_ptr<dae::KeyboardDevice> pKeyboard = std::make_unique<dae::KeyboardDevice>();
	std::unique_ptr<dae::ControllerDevice> pController = std::make_unique<dae::ControllerDevice>(0);

	//add the background
	const auto pBackgroundObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pBackground{ dae::ResourceManager::GetInstance().LoadTexture("background.tga") };
	pBackgroundObject->AddComponent(std::make_unique<dae::TextureComponent>(pBackgroundObject.get(), pBackground));
	scene.Add(pBackgroundObject);

	//add the logo
	const auto pLogoObject = std::make_shared<dae::GameObject>();
	pLogoObject->GetTransform().SetLocalPosition({ 216, 180});
	std::shared_ptr<dae::Texture2D> pLogo{ dae::ResourceManager::GetInstance().LoadTexture("logo.tga") };
	pLogoObject->AddComponent(std::make_unique<dae::TextureComponent>( pLogoObject.get(), pLogo));
	scene.Add(pLogoObject);

	//add the title text
	const auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetLocalPosition({ 80, 20});
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Programming 4 \nAssignment", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);

	const auto pPackmanObject = std::make_shared<dae::GameObject>();
	pPackmanObject->GetTransform().SetLocalPosition({ 100, 100 });
	std::shared_ptr<dae::Texture2D> pPackmanSprite{ dae::ResourceManager::GetInstance().LoadTexture("packman.tga") };
	pPackmanObject->AddComponent(std::make_unique<dae::PlayerController>(pPackmanObject.get()));

	pPackmanObject->AddComponent(std::make_unique<dae::TextureComponent>(pPackmanObject.get(), pPackmanSprite));
	scene.Add(pPackmanObject);

	const auto pPackmanScoreDisplay{ std::make_shared<dae::GameObject>() };
	pPackmanScoreDisplay->GetTransform().SetLocalPosition({ 20, 100 });
	pPackmanScoreDisplay->AddComponent(std::make_unique<dae::PlainTextComponent>(pPackmanScoreDisplay.get(), "Score: 0", font, SDL_Color{ 255,255,255,255 }));
	pPackmanScoreDisplay->AddComponent(std::make_unique<dae::ScoreDisplay>(pPackmanScoreDisplay.get(), pPackmanObject->GetComponent<dae::PlayerController>()));
	scene.Add(pPackmanScoreDisplay);
	const auto pPackmanLivesDisplay{ std::make_shared<dae::GameObject>() };
	pPackmanLivesDisplay->GetTransform().SetLocalPosition({ 20, 150 });
	pPackmanLivesDisplay->AddComponent(std::make_unique<dae::PlainTextComponent>(pPackmanLivesDisplay.get(), "Lives: 3", font, SDL_Color{ 255,255,255,255 }));
	pPackmanLivesDisplay->AddComponent(std::make_unique<dae::LivesDisplay>(pPackmanLivesDisplay.get(), pPackmanObject->GetComponent<dae::PlayerController>()));
	scene.Add(pPackmanLivesDisplay);

	std::unique_ptr<dae::MoveCommand> pPackmanMoveRight{ std::make_unique<dae::MoveCommand>(pPackmanObject.get(), glm::vec2{ 100,0 })};
	std::unique_ptr<dae::MoveCommand> pPackmanMoveLeft{ std::make_unique<dae::MoveCommand>(pPackmanObject.get(), glm::vec2{ -100,0 })};
	std::unique_ptr<dae::MoveCommand> pPackmanMoveUp{ std::make_unique<dae::MoveCommand>(pPackmanObject.get(), glm::vec2{ 0,-100})	};
	std::unique_ptr<dae::MoveCommand> pPackmanMoveDown{ std::make_unique<dae::MoveCommand>(pPackmanObject.get(), glm::vec2{ 0,100})	};
	pController->BindCommand(std::move(pPackmanMoveRight), dae::ControllerDevice::ControllerButton::DPadRight, dae::InputState::Held);
	pController->BindCommand(std::move(pPackmanMoveLeft), dae::ControllerDevice::ControllerButton::DPadLeft, dae::InputState::Held);
	pController->BindCommand(std::move(pPackmanMoveUp), dae::ControllerDevice::ControllerButton::DPadUp, dae::InputState::Held);
	pController->BindCommand(std::move(pPackmanMoveDown), dae::ControllerDevice::ControllerButton::DPadDown, dae::InputState::Held);
	pController->BindCommand(std::make_unique<dae::AddScoreCommand>(pPackmanObject.get()),dae::ControllerDevice::ControllerButton::B , dae::InputState::Released);
	pController->BindCommand(std::make_unique<dae::KillCommand>(pPackmanObject.get()), dae::ControllerDevice::ControllerButton::A, dae::InputState::Released);

	const auto pMsPackmanObject = std::make_shared<dae::GameObject>();
	pMsPackmanObject->GetTransform().SetLocalPosition({ 50, 100 });
	std::shared_ptr<dae::Texture2D> pMsPackmanSprite{ dae::ResourceManager::GetInstance().LoadTexture("MsPackman.tga") };
	pMsPackmanObject->AddComponent(std::make_unique<dae::TextureComponent>(pMsPackmanObject.get(), pMsPackmanSprite));
	pMsPackmanObject->AddComponent(std::make_unique<dae::PlayerController>(pMsPackmanObject.get()));
	scene.Add(pMsPackmanObject);
	const auto pMsPackmanScoreDisplay{ std::make_shared<dae::GameObject>() };
	pMsPackmanScoreDisplay->GetTransform().SetLocalPosition({ 20, 250 });
	pMsPackmanScoreDisplay->AddComponent(std::make_unique<dae::PlainTextComponent>(pMsPackmanScoreDisplay.get(), "Score: 0", font, SDL_Color{ 255,255,255,255 }));
	pMsPackmanScoreDisplay->AddComponent(std::make_unique<dae::ScoreDisplay>(pMsPackmanScoreDisplay.get(), pMsPackmanObject->GetComponent<dae::PlayerController>()));
	scene.Add(pMsPackmanScoreDisplay);
	const auto pMsPackmanLivesDisplay{ std::make_shared<dae::GameObject>() };
	pMsPackmanLivesDisplay->GetTransform().SetLocalPosition({ 20, 300 });
	pMsPackmanLivesDisplay->AddComponent(std::make_unique<dae::PlainTextComponent>(pMsPackmanLivesDisplay.get(), "Lives: 3", font, SDL_Color{ 255,255,255,255 }));
	pMsPackmanLivesDisplay->AddComponent(std::make_unique<dae::LivesDisplay>(pMsPackmanLivesDisplay.get(), pMsPackmanObject->GetComponent<dae::PlayerController>()));
	scene.Add(pMsPackmanLivesDisplay);

	std::unique_ptr<dae::MoveCommand> pMsPackmanMoveRight{ std::make_unique<dae::MoveCommand>(pMsPackmanObject.get(), glm::vec2{ 50,0 }) };
	std::unique_ptr<dae::MoveCommand> pMsPackmanMoveLeft{ std::make_unique<dae::MoveCommand>(pMsPackmanObject.get(), glm::vec2{ -50,0 }) };
	std::unique_ptr<dae::MoveCommand> pMsPackmanMoveUp{ std::make_unique<dae::MoveCommand>(pMsPackmanObject.get(), glm::vec2{ 0,-50}) };
	std::unique_ptr<dae::MoveCommand> pMsPackmanMoveDown{ std::make_unique<dae::MoveCommand>(pMsPackmanObject.get(), glm::vec2{ 0,50}) };
	pKeyboard->BindCommand(std::move(pMsPackmanMoveRight),SDL_SCANCODE_D, dae::InputState::Held);
	pKeyboard->BindCommand(std::move(pMsPackmanMoveLeft),SDL_SCANCODE_A, dae::InputState::Held);
	pKeyboard->BindCommand(std::move(pMsPackmanMoveUp),SDL_SCANCODE_W, dae::InputState::Held);
	pKeyboard->BindCommand(std::move(pMsPackmanMoveDown),SDL_SCANCODE_S, dae::InputState::Held);
	pKeyboard->BindCommand(std::make_unique<dae::AddScoreCommand>(pMsPackmanObject.get()), SDL_SCANCODE_T, dae::InputState::Released);
	pKeyboard->BindCommand(std::make_unique<dae::KillCommand>(pMsPackmanObject.get()), SDL_SCANCODE_R, dae::InputState::Released);
	input.AddInputDevice(std::move(pKeyboard));
	input.AddInputDevice(std::move(pController));
}

int main(int, char*[]) {
	SDL_SetMainReady();
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}