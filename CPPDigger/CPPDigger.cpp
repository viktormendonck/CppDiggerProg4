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
#include "ScoreComponent.h"
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

#include "json.hpp"
#include <fstream>

#include "MapData.h"
#include "NumberDisplay.h"

using dae::CollisionLayers;
void AddGem(dae::GameObject* pParent, std::shared_ptr<dae::Signal<dae::GameObject*>> pAnyGemPickedUpSignal, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos)
{
	const auto pGemObject = std::make_shared<dae::GameObject>();
	pGemObject->GetTransform().SetLocalPosition(pos + glm::vec2(4, 0)); // add half a tile to center the object
	pGemObject->AddComponent(std::make_unique<dae::TextureComponent>(pGemObject.get(), pTexture, false));
	pGemObject->GetComponent<dae::TextureComponent>()->SetRenderScale(glm::vec2(2, 2));
	pGemObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pGemObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
		static_cast<uint16_t>(CollisionLayers::Pickup), 
		uint16_t{ 0}));
	pGemObject->AddComponent(std::make_unique<dae::GemComponent>(pGemObject.get(),std::move(pAnyGemPickedUpSignal)));

	pGemObject->SetParent(pParent, false);
}
void AddGoldBag(dae::GameObject* pParent,std::shared_ptr<dae::Signal<dae::GameObject*>> pAnyGoldPickedUpSignal, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos)
{
	const auto pGoldBagObject = std::make_shared<dae::GameObject>();
	pGoldBagObject->AddComponent(std::make_unique<dae::SpriteSheetComponent>(pGoldBagObject.get(), pTexture, glm::ivec2{ 3,3 }, false, 0.2f, true, false, glm::ivec2{ 2,1 }));
	pGoldBagObject->GetTransform().SetLocalPosition(pos+glm::vec2(4,0));// add half of a tile to center the object
	pGoldBagObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pGoldBagObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
		static_cast<uint16_t>(CollisionLayers::Push) , 
		uint16_t{ 0 }));
	pGoldBagObject->GetComponent<dae::SpriteSheetComponent>()->SetRenderScale(glm::vec2(2.2f, 2.2f));
	pGoldBagObject->AddComponent(std::make_unique<dae::GoldBagComponent>(pGoldBagObject.get(), std::move(pAnyGoldPickedUpSignal)));
	pGoldBagObject->SetParent(pParent, false);
}

struct PlayerRequirements
{
	dae::GameObject* pParent;
	std::shared_ptr<dae::Texture2D> pPlayerTexture;
	std::shared_ptr<dae::Texture2D> pFireBallTex;
	glm::vec2 pos;
	dae::KeyboardDevice* pKeyboard;
	dae::Signal<dae::GameObject*>* pGemPickedUpSignal;
	dae::Signal<dae::GameObject*>* pGoldPickedUpSignal;
	dae::Signal<dae::GameObject*>* pEnemyKilledUpSignal;
	std::unique_ptr<dae::Signal<int>> pScoreChangedSignal;
	SDL_Scancode upButton;
	SDL_Scancode downButton;
	SDL_Scancode leftButton;
	SDL_Scancode rightButton;
	SDL_Scancode attackButton;
};

void AddPlayer(PlayerRequirements& requirements)
{

	const auto pPlayerObject = std::make_shared<dae::GameObject>();
	pPlayerObject->GetTransform().SetLocalPosition(requirements.pos);
	pPlayerObject->AddComponent(std::make_unique<dae::SpriteSheetComponent>(pPlayerObject.get(), requirements.pPlayerTexture, glm::ivec2{ 4,5 }, true, 0.3f, true, true));
	pPlayerObject->GetComponent<dae::SpriteSheetComponent>()->SetRenderScale(glm::vec2(2, 2));
	pPlayerObject->AddComponent(std::make_unique<dae::PlayerComponent>(pPlayerObject.get()));
	pPlayerObject->GetComponent<dae::PlayerComponent>()->SetRespawnPos(requirements.pos);
	pPlayerObject->AddComponent(std::make_unique<dae::CollisionRectComponent>(pPlayerObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,15 },
		uint16_t{ static_cast<uint16_t>(CollisionLayers::PlayerDamage) },
		uint16_t{ static_cast<uint16_t>(CollisionLayers::Pickup) | static_cast<uint16_t>(CollisionLayers::Push) }
		));
	pPlayerObject->AddComponent(std::make_unique<dae::ScoreComponent>(pPlayerObject.get(), requirements.pGemPickedUpSignal, requirements.pGoldPickedUpSignal, requirements.pEnemyKilledUpSignal,std::move(requirements.pScoreChangedSignal)));
	pPlayerObject->SetParent(requirements.pParent, false);

	requirements.pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
		requirements.upButton,
		dae::InputState::Pressed
	);
	requirements.pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
		requirements.downButton,
		dae::InputState::Pressed
	);
	requirements.pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
		requirements.rightButton,
		dae::InputState::Pressed
	);
	requirements.pKeyboard->BindCommand(
		std::make_unique<dae::MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
		requirements.leftButton,
		dae::InputState::Pressed
	);
	requirements.pKeyboard->BindCommand(
		std::make_unique<dae::ShootCommand>(pPlayerObject.get(), requirements.pFireBallTex),
		requirements.attackButton,
		dae::InputState::Pressed
	);
	
}

void LoadLevelFromJson(dae::KeyboardDevice* pKeyboard, dae::ControllerDevice*, const std::shared_ptr<dae::GameObject>& pWorldObject, std::unique_ptr<dae::Signal<int>> pScoreChangedSignal, const std::string& filePath)
{
	const std::ifstream file(filePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + filePath);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	const nlohmann::json json = nlohmann::json::parse(buffer.str());
	std::vector<int> playerPos{ json["playerPos"].get<std::vector<int>>() };
	std::vector<int> map{json["tiles"].get<std::vector<int>>()};
 	glm::ivec2 playerSpawnPos{ playerPos[0] ,playerPos[1] }; 

	std::shared_ptr<dae::Texture2D> pPlayerTexture{ dae::ResourceManager::GetInstance().LoadTexture("PlayerSprites.png") };
	std::shared_ptr<dae::Texture2D> pFireBallTexture{ dae::ResourceManager::GetInstance().LoadTexture("Fireball.png") };
	std::shared_ptr<dae::Texture2D> pGemTexture{ dae::ResourceManager::GetInstance().LoadTexture("Gem.png") };
	std::shared_ptr<dae::Texture2D> pGoldBagTexture{ dae::ResourceManager::GetInstance().LoadTexture("GoldBagSprites.png") };

	dae::TileMapComponent* pTileMap = pWorldObject->GetComponent<dae::TileMapComponent>();
	if (!pTileMap)
	{
		std::shared_ptr<dae::Texture2D> pTileSet{ dae::ResourceManager::GetInstance().LoadTexture("tileset.png") };
		std::unique_ptr<dae::TileMapComponent> tileMap{ std::make_unique<dae::TileMapComponent>(pWorldObject.get(), pTileSet, glm::ivec2{6,3}, glm::ivec2{40,25}) };
		pTileMap = tileMap.get();
		pWorldObject->AddComponent(std::move(tileMap));
	}

	pTileMap->SetMap(map);
	std::shared_ptr pAnyGoldPickedUpSignal = std::make_shared<dae::Signal<dae::GameObject*>>();
	std::shared_ptr pAnyGemPickedUpSignal = std::make_shared<dae::Signal<dae::GameObject*>>();
	std::shared_ptr pAnyEnemyKilledSignal = std::make_shared<dae::Signal<dae::GameObject*>>();

	PlayerRequirements playerRequirements
	{
		pWorldObject.get(),
		pPlayerTexture,
		pFireBallTexture,
		pTileMap->TileToLocal(playerSpawnPos),
		pKeyboard,
		pAnyGemPickedUpSignal.get(),
		pAnyGoldPickedUpSignal.get(),
		pAnyEnemyKilledSignal.get(),
		std::move(pScoreChangedSignal),
		SDL_SCANCODE_W,
		SDL_SCANCODE_S,
		SDL_SCANCODE_A,
		SDL_SCANCODE_D,
		SDL_SCANCODE_SPACE
	};

	AddPlayer(playerRequirements);

	for (const std::vector<int>& bagPos : json["moneyBagPositions"].get<std::vector<std::vector<int>>>())
	{
		AddGoldBag(pWorldObject.get(),pAnyGoldPickedUpSignal, pGoldBagTexture, pTileMap->TileToLocal(glm::ivec2{bagPos[0], bagPos[1]}));
	}

	for (const std::vector<int>& gemPos : json["gemPositions"].get<std::vector<std::vector<int>>>())
	{
		AddGem(pWorldObject.get(), pAnyGemPickedUpSignal, pGemTexture, pTileMap->TileToLocal(glm::ivec2{ gemPos[0], gemPos[1] }));
	}
	
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

	const auto pWorldObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pTileSet{ dae::ResourceManager::GetInstance().LoadTexture("tileset.png") };
	pWorldObject->GetTransform().SetLocalPosition({ 75, 200 });
	pWorldObject->GetTransform().SetLocalScale({ 2,2 });
	std::unique_ptr<dae::TileMapComponent> pTileMap = std::make_unique<dae::TileMapComponent>(pWorldObject.get(), pTileSet, glm::ivec2{ 6,3 }, glm::ivec2{ 40,25 });
	pWorldObject->AddComponent(std::move(pTileMap));
	std::unique_ptr<dae::Signal<int>> pScoreChangedSignal = std::make_unique<dae::Signal<int>>();
	dae::Signal<int>* pScoreChangedSignalPtr = pScoreChangedSignal.get();
	LoadLevelFromJson(pKeyboard.get(), pController.get(), pWorldObject, std::move(pScoreChangedSignal), "Data/level1.json");
	scene.Add(pWorldObject);

	const auto pUiObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pNumbers{ dae::ResourceManager::GetInstance().LoadTexture("nums.png") };


	const auto pScoreObject = std::make_shared<dae::GameObject>();
	pScoreObject->AddComponent(std::make_unique<dae::NumberDisplay>(pScoreObject.get(),pScoreChangedSignalPtr,pNumbers,glm::vec2(2,2)));
	pScoreObject->GetTransform().SetLocalPosition(glm::vec2(20, 20));

	pScoreObject->SetParent(pUiObject.get(), true);

	scene.Add(pUiObject);


	dae::InputManager::GetInstance().AddInputDevice(std::move(pController));
	dae::InputManager::GetInstance().AddInputDevice(std::move(pKeyboard));
}

int main(int, char* []) {

	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}