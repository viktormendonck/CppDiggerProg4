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
#include "TextInputField.h"
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
	namespace GameLoader
	{
		//all functions needed to add objects to the game and start parts of it
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
			if (requirements.gameMode == GameMode::CoOp)
			{
				if (requirements.playerIndex == 0)
				{
					pPlayerObject->AddComponent(std::make_unique<ScoreComponent>(pPlayerObject.get(), requirements.pGemPickedUpSignal, requirements.pGoldPickedUpSignal, requirements.pEnemyKilledSignal, std::move(requirements.pScoreChangedSignal)));
				}
				else
				{
					requirements.pParent->GetComponents<PlayerComponent>()[0]->GetParent()->GetComponent<ScoreComponent>()->SetSecondPlayer(pPlayerObject.get());
				}
			}
			else if (requirements.gameMode == GameMode::SinglePlayer)
			{
				pPlayerObject->AddComponent(std::make_unique<ScoreComponent>(pPlayerObject.get(), requirements.pGemPickedUpSignal, requirements.pGoldPickedUpSignal, requirements.pEnemyKilledSignal, std::move(requirements.pScoreChangedSignal)));
			}
			if (requirements.playerIndex == 0)
			{
				pPlayerObject->AddComponent(std::make_unique<HealthComponent>(pPlayerObject.get(), std::move(requirements.pLivesChangedSignal)));
			}
			else
			{
				pPlayerObject->AddComponent(std::make_unique<HealthComponent>(pPlayerObject.get(), std::move(requirements.pPlayerTwoLivesChangedSignal)));
			}
			uint16_t damageLayers = static_cast<uint16_t>(CollisionLayers::EnemyDamage);
			if (requirements.gameMode == GameMode::Versus)
			{
				damageLayers |= static_cast<uint16_t>(CollisionLayers::PlayerDamage);
			}

			pPlayerObject->SetParent(requirements.pParent, false);
			if (requirements.playerIndex == 0)
			{
				requirements.pKeyboard->BindCommand(
					std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
					SDL_SCANCODE_W,
					InputState::Held
				);
				requirements.pKeyboard->BindCommand(
					std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
					SDL_SCANCODE_S,
					InputState::Held
				);
				requirements.pKeyboard->BindCommand(
					std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
					SDL_SCANCODE_D,
					InputState::Held
				);
				requirements.pKeyboard->BindCommand(
					std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
					SDL_SCANCODE_A,
					InputState::Held
				);
				requirements.pKeyboard->BindCommand(
					std::make_unique<ShootCommand>(pPlayerObject.get(), requirements.pFireBallTex,damageLayers),
					SDL_SCANCODE_SPACE,
					InputState::Pressed
				);
			}
			ControllerDevice* pController{};
			if (requirements.gameMode == GameMode::SinglePlayer || requirements.playerIndex != 0)
			{
				pController = requirements.pController;
			}
			else
			{
				pController = requirements.pControllerTwo;
			}

			pController->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, -1 }),
				ControllerDevice::ControllerButton::DPadUp,
				InputState::Held
			);
			pController->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 0, 1 }),
				ControllerDevice::ControllerButton::DPadDown,
				InputState::Held
			);
			pController->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ 1, 0 }),
				ControllerDevice::ControllerButton::DPadRight,
				InputState::Held
			);
			pController->BindCommand(
				std::make_unique<MoveCommand>(pPlayerObject.get(), glm::ivec2{ -1, 0 }),
				ControllerDevice::ControllerButton::DPadLeft,
				InputState::Held
			);
			pController->BindCommand(
				std::make_unique<ShootCommand>(pPlayerObject.get(), requirements.pFireBallTex,damageLayers),
				ControllerDevice::ControllerButton::X,
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

		void StartGame(GameMode gameMode,const std::string& playerName)
		{
			SDL_StopTextInput();
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
			std::unique_ptr<Signal<int>> pPlayerOneLivesChangedSignal = std::make_unique<Signal<int>>();

			std::unique_ptr<Signal<int>> pPlayerTwoLivesChangedSignal = std::make_unique<Signal<int>>();

			Signal<int>* pScoreChangedSignalPtr = pScoreChangedSignal.get();
			Signal<int>* pPlayerOneLivesChangedSignalPtr = pPlayerOneLivesChangedSignal.get();
			Signal<int>* pPlayerTwoLivesChangedSignalPtr = pPlayerTwoLivesChangedSignal.get();


			pWorldObject->AddComponent(std::make_unique<LevelManagerComponent>(pWorldObject.get(), gameMode, std::move(pScoreChangedSignal), std::move(pPlayerOneLivesChangedSignal), std::move(pPlayerTwoLivesChangedSignal), pKeyboard, pController, pControllerTwo,playerName));

			pKeyboard->BindCommand(
				std::make_unique<SkipLevelCommand>(pWorldObject.get()),
				SDL_SCANCODE_F1,
				InputState::Pressed
			);

			std::shared_ptr<Texture2D> pLife{ ResourceManager::GetInstance().LoadTexture("life.png") };
			const std::shared_ptr<GameObject> pUiObject = std::make_shared<GameObject>();

			const auto pLivesDisplayObject = std::make_shared<GameObject>();
			pLivesDisplayObject->AddComponent(std::make_unique<LivesDisplay>(pLivesDisplayObject.get(), pLife, pPlayerOneLivesChangedSignalPtr));
			pLivesDisplayObject->GetTransform().SetLocalPosition(glm::vec2(600, 20));
			pLivesDisplayObject->SetParent(pUiObject.get(), false);

			if (gameMode != GameMode::SinglePlayer)
			{
				const auto pSecondLivesDisplayObject = std::make_shared<GameObject>();
				pSecondLivesDisplayObject->AddComponent(std::make_unique<LivesDisplay>(pSecondLivesDisplayObject.get(), pLife, pPlayerTwoLivesChangedSignalPtr));
				pSecondLivesDisplayObject->GetTransform().SetLocalPosition(glm::vec2(600, 600));
				pSecondLivesDisplayObject->SetParent(pUiObject.get(), false);
			}
			std::shared_ptr<Texture2D> pNumbers{ ResourceManager::GetInstance().LoadTexture("nums.png") };


			if (gameMode != GameMode::Versus)
			{
				const std::shared_ptr<GameObject> pScoreObject = std::make_shared<GameObject>();
				pScoreObject->AddComponent(std::make_unique<NumberDisplay>(pScoreObject.get(), pScoreChangedSignalPtr, pNumbers, glm::vec2(2, 2)));
				pScoreObject->GetTransform().SetLocalPosition(glm::vec2(20, 20));
				pScoreObject->SetParent(pUiObject.get(), false);
			}


			scene.Add(pUiObject);
			scene.Add(pWorldObject);
			scene.Init();
		}

		void NameFillMenu()
		{
			SDL_StartTextInput();
			SceneManager::GetInstance().RemoveScene("Naming");
			auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
			Scene& scene = SceneManager::GetInstance().CreateScene("Naming");
			SceneManager::GetInstance().SetActiveScene("Naming");

			std::shared_ptr<Texture2D> pNameFieldTex{ ResourceManager::GetInstance().LoadTexture("TextField.png") };
			std::shared_ptr<Texture2D> pPlayButtonTex{ ResourceManager::GetInstance().LoadTexture("PlayButton.png") };

			std::shared_ptr<GameObject> pNamingScene = std::make_shared<GameObject>();

			std::shared_ptr<GameObject> pTextInputFieldObject = std::make_shared<GameObject>();
			pTextInputFieldObject->AddComponent(std::make_unique<TextureComponent>(pTextInputFieldObject.get(), pNameFieldTex, false));
			pTextInputFieldObject->AddComponent(std::make_unique<TextInputField>(pTextInputFieldObject.get(), font));
			pTextInputFieldObject->GetTransform().SetLocalPosition(glm::vec2{ 300,300 });
			pTextInputFieldObject->SetParent(pNamingScene.get(), false);

			std::shared_ptr<GameObject> pPlayButton = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pVersusButton = std::make_unique<UIButtonComponent>(pPlayButton.get(), glm::vec2{ 220,50 });
			pVersusButton->onClick.AddListener([]()
			{
				InputManager::GetInstance().textInput.RemoveListener(0);
				std::string name{};
				for (std::shared_ptr<GameObject> pObject : SceneManager::GetInstance().GetScene("Naming")->GetObjects())
				{
					if (TextInputField* InputField = pObject->GetComponents<TextInputField>()[0])
					{
						name =InputField->GetText();
					}
				}
				StartGame(GameMode::SinglePlayer, name);
			});
			pPlayButton->AddComponent(std::move(pVersusButton));
			pPlayButton->AddComponent(std::make_unique<TextureComponent>(pPlayButton.get(), pPlayButtonTex, false));
			pPlayButton->GetTransform().SetLocalPosition({300,380});
			pPlayButton->SetParent(pNamingScene.get(), false);


			scene.Add(pNamingScene);

		}
		void OpenMenu()
		{
			OpenMenu("---", 0);
		}


		void OpenMenu(const std::string& name, int score)
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
			glm::vec2 coOpButtonPos{ 450, 310 };
			glm::vec2 versusButtonPos{ 450, 380 };

			std::shared_ptr<GameObject> pSinglePlayerButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pSinglePlayerButton = std::make_unique<UIButtonComponent>(pSinglePlayerButtonObject.get(), buttonSize);
			pSinglePlayerButton->onClick.AddListener([]() {NameFillMenu(); });
			pSinglePlayerButtonObject->AddComponent(std::move(pSinglePlayerButton));
			pSinglePlayerButtonObject->AddComponent(std::make_unique<TextureComponent>(pSinglePlayerButtonObject.get(), pSinglePlayerButtonTex, false));
			pSinglePlayerButtonObject->GetTransform().SetLocalPosition(singlePlayerButtonPos);
			pSinglePlayerButtonObject->SetParent(pMainUIScene.get(), false);

			std::shared_ptr<GameObject> pCoOpButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pCoOpButton = std::make_unique<UIButtonComponent>(pCoOpButtonObject.get(), buttonSize);
			pCoOpButton->onClick.AddListener([]() {StartGame(GameMode::CoOp,""); });
			pCoOpButtonObject->AddComponent(std::move(pCoOpButton));
			pCoOpButtonObject->AddComponent(std::make_unique<TextureComponent>(pCoOpButtonObject.get(), pCoopButtonTex, false));
			pCoOpButtonObject->GetTransform().SetLocalPosition(coOpButtonPos);
			pCoOpButtonObject->SetParent(pMainUIScene.get(), false);

			std::shared_ptr<GameObject> pVersusButtonObject = std::make_shared<GameObject>();
			std::unique_ptr<UIButtonComponent> pVersusButton = std::make_unique<UIButtonComponent>(pVersusButtonObject.get(), buttonSize);
			pVersusButton->onClick.AddListener([]() {StartGame(GameMode::Versus,""); });
			pVersusButtonObject->AddComponent(std::move(pVersusButton));
			pVersusButtonObject->AddComponent(std::make_unique<TextureComponent>(pVersusButtonObject.get(), pVersusButtonTex, false));
			pVersusButtonObject->GetTransform().SetLocalPosition(versusButtonPos);
			pVersusButtonObject->SetParent(pMainUIScene.get(), false);

			pMainUIScene->Init();
			scene.Add(pMainUIScene);
		}
	}
}
