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

#include "EnemySpawnerComponent.h"
#include "MapData.h"

#include "CheatCommands.h"
#include "HealthComponent.h"
#include "HighScoreDisplay.h"
#include "LevelManagerComponent.h"
#include "LivesDisplay.h"
#include "NumberDisplay.h"
#include "UIButtonComponent.h"
#include "UITagComponent.h"


namespace dae
{
	namespace mapData
	{

		uint16_t GetTileRule(TileType type)
		{
			constexpr uint16_t empty = 0b0000000000000000;
			return g_TileRules.GetValue(type, empty);
		}

		TileType GetTileType(uint16_t tile)
		{
			return g_TileRules.GetValue(tile, TileType::Empty);
		}

		TileType CompareTiles(TileType newTile, TileType oldTile)
		{
			return GetTileType(GetTileRule(newTile) & GetTileRule(oldTile));
		}

	}
	namespace levelLoader
	{

		void AddGem(GameObject* pParent,
			std::shared_ptr<Signal<GameObject*>> pAnyGemPickedUpSignal,
			const std::shared_ptr<Texture2D>& pTexture, glm::vec2 pos)
		{
			const auto pGemObject = std::make_shared<GameObject>();
			pGemObject->GetTransform().SetLocalPosition(pos + glm::vec2(4, 0)); // add half a tile to center the object
			pGemObject->AddComponent(std::make_unique<TextureComponent>(pGemObject.get(), pTexture, false));
			pGemObject->GetComponent<TextureComponent>()->SetRenderScale(glm::vec2(2, 2));
			pGemObject->AddComponent(std::make_unique<CollisionRectComponent>(pGemObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
				static_cast<uint16_t>(CollisionLayers::Pickup),
				uint16_t{ 0 }));
			pGemObject->AddComponent(std::make_unique<GemComponent>(pGemObject.get(), std::move(pAnyGemPickedUpSignal)));

			pGemObject->SetParent(pParent, false);
		}

		void AddGoldBag(GameObject* pParent,
			std::shared_ptr<Signal<GameObject*>> pAnyGoldPickedUpSignal,
			const std::shared_ptr<Texture2D>& pTexture, glm::vec2 pos)
		{
			const auto pGoldBagObject = std::make_shared<GameObject>();
			pGoldBagObject->AddComponent(std::make_unique<SpriteSheetComponent>(pGoldBagObject.get(), pTexture, glm::ivec2{ 3,3 }, false, 0.2f, true, false, glm::ivec2{ 2,1 }));
			pGoldBagObject->GetTransform().SetLocalPosition(pos + glm::vec2(4, 0));// add half of a tile to center the object
			pGoldBagObject->AddComponent(std::make_unique<CollisionRectComponent>(pGoldBagObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 },
				static_cast<uint16_t>(CollisionLayers::Push),
				uint16_t{ 0 }));
			pGoldBagObject->GetComponent<SpriteSheetComponent>()->SetRenderScale(glm::vec2(2.2f, 2.2f));
			pGoldBagObject->AddComponent(std::make_unique<GoldBagComponent>(pGoldBagObject.get(), std::move(pAnyGoldPickedUpSignal)));
			pGoldBagObject->SetParent(pParent, false);
		}

		void AddPlayer(PlayerRequirements& requirements)
		{

			const auto pPlayerObject = std::make_shared<GameObject>();
			pPlayerObject->GetTransform().SetLocalPosition(requirements.pos);
			pPlayerObject->AddComponent(std::make_unique<SpriteSheetComponent>(pPlayerObject.get(), requirements.pPlayerTexture, glm::ivec2{ 4,5 }, true, 0.3f, true, true));
			pPlayerObject->GetComponent<SpriteSheetComponent>()->SetRenderScale(glm::vec2(2, 2));
			pPlayerObject->AddComponent(std::make_unique<PlayerComponent>(pPlayerObject.get()));
			pPlayerObject->GetComponent<PlayerComponent>()->SetRespawnPos(requirements.pos);
			pPlayerObject->AddComponent(std::make_unique<CollisionRectComponent>(pPlayerObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,15 },
				uint16_t{ static_cast<uint16_t>(CollisionLayers::PlayerDamage) },
				uint16_t{ static_cast<uint16_t>(CollisionLayers::Pickup) | static_cast<uint16_t>(CollisionLayers::Push) }
			));
			pPlayerObject->AddComponent(std::make_unique<ScoreComponent>(pPlayerObject.get(), requirements.pGemPickedUpSignal, requirements.pGoldPickedUpSignal, requirements.pEnemyKilledSignal, std::move(requirements.pScoreChangedSignal)));
			pPlayerObject->AddComponent(std::make_unique<HealthComponent>(pPlayerObject.get(), std::move(requirements.pLivesChangedSignal)));
			pPlayerObject->SetParent(requirements.pParent, false);
			requirements.pKeyboard->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
				requirements.upButton,
				InputState::Held
			);
			requirements.pKeyboard->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
				requirements.downButton,
				InputState::Held
			);
			requirements.pKeyboard->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
				requirements.rightButton,
				InputState::Held
			);
			requirements.pKeyboard->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
				requirements.leftButton,
				InputState::Held
			);
			requirements.pKeyboard->BindCommand(
				std::make_unique<ShootCommand>(pPlayerObject.get(), requirements.pFireBallTex),
				requirements.attackButton,
				InputState::Pressed
			);

		}

		void AddEnemySpawner(GameObject* pParent, glm::vec2 pos,
			std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal, std::shared_ptr<Texture2D> pEnemyTex,
			int charges, float timeBetweenSpawns)
		{
			const auto pSpawnerObject = std::make_shared<GameObject>();
			pSpawnerObject->AddComponent(std::make_unique<EnemySpawnerComponent>(pSpawnerObject.get(), pAnyEnemyKilledSignal, pEnemyTex, charges, timeBetweenSpawns));

			pSpawnerObject->AddComponent(std::make_unique<CollisionRectComponent>(pSpawnerObject.get(), glm::vec2{ 24,24 }, glm::vec2{ 0,0 }, uint16_t{ 0 }, uint16_t{ 0 }));
			pSpawnerObject->GetTransform().SetLocalPosition(pos);
			pSpawnerObject->SetParent(pParent, false);
		}

		void StartGame(GameMode gameMode)
		{
			InputManager::GetInstance().ClearCommands();
			SceneManager::GetInstance().RemoveScene("Game");
			auto& scene = SceneManager::GetInstance().CreateScene("Game");
			SceneManager::GetInstance().SetActiveScene("Game");
			ServiceLocator::RegisterSoundSystem(std::make_unique<SDLSoundSystem>());
			ServiceLocator::GetSoundSystem().AddSound("Data/Sounds/DeathSound.mp3", SoundSystem::SoundType::SoundEffect);

			//add devices

			KeyboardDevice* pKeyboard = InputManager::GetInstance().GetKeyboardDevice();
			ControllerDevice* pController = InputManager::GetInstance().GetControllerDevice(0);
			ControllerDevice* pControllerTwo = InputManager::GetInstance().GetControllerDevice(1);
			const std::shared_ptr<GameObject> pWorldObject = std::make_shared<GameObject>();
			std::shared_ptr<Texture2D> pTileSet{ ResourceManager::GetInstance().LoadTexture("tileset.png") };
			pWorldObject->GetTransform().SetLocalPosition({ 75, 200 });
			pWorldObject->GetTransform().SetLocalScale({ 2,2 });
			std::unique_ptr<TileMapComponent> pTileMap = std::make_unique<TileMapComponent>(pWorldObject.get(), pTileSet, glm::ivec2{ 6,3 }, glm::ivec2{ 40,25 });
			pWorldObject->AddComponent(std::move(pTileMap));
			std::unique_ptr<Signal<int>> pScoreChangedSignal = std::make_unique<Signal<int>>();
			std::unique_ptr<Signal<int>> pLivesChangedSignal = std::make_unique<Signal<int>>();

			Signal<int>* pScoreChangedSignalPtr = pScoreChangedSignal.get();
			Signal<int>* pLivesChangedSignalPtr = pLivesChangedSignal.get();
			pWorldObject->AddComponent(std::make_unique<LevelManagerComponent>(pWorldObject.get(), gameMode, std::move(pScoreChangedSignal),std::move(pLivesChangedSignal), pKeyboard, pController, pControllerTwo));

			pKeyboard->BindCommand(
				std::make_unique<SkipLevelCommand>(pWorldObject.get()),
				SDL_SCANCODE_F1,
				InputState::Pressed
			);

			const std::shared_ptr<GameObject> pUiObject = std::make_shared<GameObject>();
			pUiObject->AddComponent(std::make_unique<UITagComponent>(pUiObject.get()));
			std::shared_ptr<Texture2D> pNumbers{ ResourceManager::GetInstance().LoadTexture("nums.png") };

			const std::shared_ptr<GameObject> pScoreObject = std::make_shared<GameObject>();
			pScoreObject->AddComponent(std::make_unique<NumberDisplay>(pScoreObject.get(), pScoreChangedSignalPtr, pNumbers, glm::vec2(2, 2)));
			pScoreObject->GetTransform().SetLocalPosition(glm::vec2(20, 20));
			pScoreObject->SetParent(pUiObject.get(), false);

			const auto pLivesObject = std::make_shared<GameObject>();
			std::shared_ptr<Texture2D> pLife{ ResourceManager::GetInstance().LoadTexture("life.png") };

			pLivesObject->AddComponent(std::make_unique<LivesDisplay>(pLivesObject.get(), pLife,pLivesChangedSignalPtr ));
			pLivesObject->GetTransform().SetLocalPosition(glm::vec2(600, 20));
			pLivesObject->SetParent(pUiObject.get(), false);

			scene.Add(pUiObject);
			scene.Add(pWorldObject);
			scene.Init();
		}

		void OpenMenu()
		{
			OpenMenu("---", 0);
		}

		void OpenMenu(const std::string& name , int score )
		{
			SceneManager::GetInstance().RemoveScene("UI");
			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
			Scene& scene = SceneManager::GetInstance().CreateScene("UI");
			SceneManager::GetInstance().SetActiveScene("UI");

			std::shared_ptr<GameObject> pMainUIScene = std::make_shared<GameObject>();
			std::shared_ptr<Texture2D> pHomeScreenTex{ ResourceManager::GetInstance().LoadTexture("HomeScreen.png") };

			pMainUIScene->AddComponent(std::make_unique<TextureComponent>(pMainUIScene.get(), pHomeScreenTex, false));
			std::shared_ptr<Texture2D> pNumbers{ ResourceManager::GetInstance().LoadTexture("nums.png") };

			std::shared_ptr<GameObject> pHighScoreObject = std::make_shared<GameObject>();
			pHighScoreObject->AddComponent(std::make_unique<HighScoreDisplayComponent>(pHighScoreObject.get(), "Data/high_scores.json", score, name, font, pNumbers));
			pHighScoreObject->GetTransform().SetLocalPosition(glm::vec2{ 100,220 });
			pHighScoreObject->SetParent(pMainUIScene.get(), false);
			glm::vec2 buttonSize{ 220,50 };

			std::shared_ptr<Texture2D> pSinglePlayerButtonTex{ ResourceManager::GetInstance().LoadTexture("SinglePlayerButton.png") };
			std::shared_ptr<Texture2D> pCoopButtonTex{ ResourceManager::GetInstance().LoadTexture("CoopButton.png") };
			std::shared_ptr<Texture2D> pVersusButtonTex{ ResourceManager::GetInstance().LoadTexture("VersusButton.png") };
			glm::vec2 singlePlayerButtonPos{ 450, 240 };
			glm::vec2 coOpButtonPos { 450, 310 };
			glm::vec2 versusButtonPos { 450, 380 };

			std::shared_ptr<GameObject> pSinglePlayerButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pSinglePlayerButton = std::make_unique<UIButtonComponent>(pSinglePlayerButtonObject.get(),buttonSize);
			pSinglePlayerButton->onClick.AddListener([]() {StartGame(GameMode::SinglePlayer); });
			pSinglePlayerButtonObject->AddComponent(std::move(pSinglePlayerButton));
			pSinglePlayerButtonObject->AddComponent(std::make_unique<TextureComponent>(pSinglePlayerButtonObject.get(), pSinglePlayerButtonTex, false));
			pSinglePlayerButtonObject->GetTransform().SetLocalPosition(singlePlayerButtonPos);
			pSinglePlayerButtonObject->SetParent(pMainUIScene.get(),false);

			std::shared_ptr<GameObject> pCoOpButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pCoOpButton = std::make_unique<UIButtonComponent>(pCoOpButtonObject.get(), buttonSize);
			pCoOpButton->onClick.AddListener([]() {StartGame(GameMode::CoOp); });
			pCoOpButtonObject->AddComponent(std::move(pCoOpButton));
			pCoOpButtonObject->AddComponent(std::make_unique<TextureComponent>(pCoOpButtonObject.get(), pCoopButtonTex, false));
			pCoOpButtonObject->GetTransform().SetLocalPosition(coOpButtonPos);
			pCoOpButtonObject->SetParent(pMainUIScene.get(), false);

			std::shared_ptr<GameObject> pVersusButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pVersusButton = std::make_unique<UIButtonComponent>(pVersusButtonObject.get(), buttonSize);
			pVersusButton->onClick.AddListener([]() {StartGame(GameMode::Versus); });
			pVersusButtonObject->AddComponent(std::move(pVersusButton));
			pVersusButtonObject->AddComponent(std::make_unique<TextureComponent>(pVersusButtonObject.get(), pVersusButtonTex, false));
			pVersusButtonObject->GetTransform().SetLocalPosition(versusButtonPos);
			pVersusButtonObject->SetParent(pMainUIScene.get(), false);

			pMainUIScene->Init();
			scene.Add(pMainUIScene);
		}
	}
}
