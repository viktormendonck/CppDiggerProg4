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
#include "CollisionRectComponent.h"
#include "GemComponent.h"
#include "GoldBagComponent.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"
#include "SpriteSheetComponent.h"

#include "MapData.h"

using dae::CollisionLayers;
void AddGem(dae::GameObject* pParent, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos)
{
	const auto pGemObject = std::make_shared<dae::GameObject>();
	pGemObject->GetTransform().SetLocalPosition(pos + glm::vec2(4, 0)); // add half a tile to center the object
	pGemObject->AddComponent(std::make_unique<dae::TextureComponent>(pGemObject.get(), pTexture, false));
	pGemObject->GetComponent<dae::TextureComponent>()->SetRenderScale(glm::vec2(2, 2));
	pGemObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pGemObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
		static_cast<uint16_t>(CollisionLayers::Pickup), 
		uint16_t{ 0 }));
	pGemObject->AddComponent(std::make_unique<dae::GemComponent>(pGemObject.get()));

	pGemObject->SetParent(pParent, false);
}

void AddGoldBag(dae::GameObject* pParent, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos)
{
	const auto pGoldBagObject = std::make_shared<dae::GameObject>();
	pGoldBagObject->AddComponent(std::make_unique<dae::SpriteSheetComponent>(pGoldBagObject.get(), pTexture, glm::ivec2{ 3,3 }, false, 0.2f, true, false, glm::ivec2{ 2,1 }));
	pGoldBagObject->GetTransform().SetLocalPosition(pos+glm::vec2(4,0));// add half of a tile to center the object
	pGoldBagObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pGoldBagObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
		static_cast<uint16_t>(CollisionLayers::Push) , 
		uint16_t{ 0 }));
	pGoldBagObject->GetComponent<dae::SpriteSheetComponent>()->SetRenderScale(glm::vec2(2, 2));
	pGoldBagObject->AddComponent(std::make_unique<dae::GoldBagComponent>(pGoldBagObject.get()));

	pGoldBagObject->SetParent(pParent, false);
}
void AddPlayer(dae::GameObject* pParent, const std::shared_ptr<dae::Texture2D>& pPlayerTexture, const std::shared_ptr<dae::Texture2D>& pFireBallTex, glm::vec2 pos, dae::KeyboardDevice* pKeyboard, SDL_Scancode upButton, SDL_Scancode downButton,SDL_Scancode leftButton,SDL_Scancode rightButton,SDL_Scancode attackButton)
{

	const auto pPlayerObject = std::make_shared<dae::GameObject>();
	pPlayerObject->GetTransform().SetLocalPosition(pos);
	pPlayerObject->AddComponent(std::make_unique<dae::SpriteSheetComponent>(pPlayerObject.get(), pPlayerTexture, glm::ivec2{ 4,4 }, true, 0.3f, true, true));
	pPlayerObject->GetComponent<dae::SpriteSheetComponent>()->SetRenderScale(glm::vec2(2, 2));
	pPlayerObject->AddComponent(std::make_unique<dae::PlayerComponent>(pPlayerObject.get(), 0));
	pPlayerObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pPlayerObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,15 },
		uint16_t{ static_cast<uint16_t>(CollisionLayers::PlayerDamage) },
		uint16_t{ static_cast<uint16_t>(CollisionLayers::Pickup) | static_cast<uint16_t>(CollisionLayers::Push) }
		));
	pPlayerObject->SetParent(pParent, false);

	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
		upButton,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
		downButton,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
		rightButton,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
		leftButton,
		dae::InputState::Pressed
	);
	pKeyboard->BindCommand(
		std::make_unique<dae::ShootCommand>(pPlayerObject.get(),pFireBallTex),
		attackButton,
		dae::InputState::Pressed
	);
	
}

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
	std::unique_ptr<dae::TileMapComponent> pTileMap = std::make_unique<dae::TileMapComponent>(pWorldObject.get(), pTileSet, glm::ivec2{ 6,3 }, glm::ivec2{ 40,25 }, 0);
	dae::TileMapComponent* pTileMapPtr = pTileMap.get();
	pWorldObject->AddComponent(std::move(pTileMap));
	pWorldObject->GetComponent<dae::TileMapComponent>()->SetMap(dae::MapData::m_Levels[0]);
	scene.Add(pWorldObject);


	std::shared_ptr<dae::Texture2D> pPlayerTexture{ dae::ResourceManager::GetInstance().LoadTexture("PlayerSprites.png") };
	std::shared_ptr<dae::Texture2D> pFireBallTexture{ dae::ResourceManager::GetInstance().LoadTexture("Fireball.png") };

	AddPlayer(pWorldObject.get(), pPlayerTexture,pFireBallTexture, {0,0}, pKeyboard.get(), SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,SDL_SCANCODE_SPACE);
	
	std::shared_ptr<dae::Texture2D> pGemTexture{ dae::ResourceManager::GetInstance().LoadTexture("Gem.png") };
	AddGem(pWorldObject.get(), pGemTexture, pTileMapPtr->TileToLocal(glm::ivec2(10,10)));

	std::shared_ptr<dae::Texture2D> pGoldBagTexture{ dae::ResourceManager::GetInstance().LoadTexture("GoldBagSprites.png") };
	AddGoldBag(pWorldObject.get(), pGoldBagTexture, pTileMapPtr->TileToLocal(glm::ivec2(15,10)));

	dae::InputManager::GetInstance().AddInputDevice(std::move(pController));
	dae::InputManager::GetInstance().AddInputDevice(std::move(pKeyboard));
}

int main(int, char* []) {

	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}