#include "LevelManagerComponent.h"

#include <fstream>

#include "GemComponent.h"
#include "ResourceManager.h"
#include "json.hpp"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "ScoreComponent.h"

dae::LevelManagerComponent::LevelManagerComponent(GameObject* pParent, levelLoader::GameMode gameMode,
	std::unique_ptr<Signal<int>> pScoreChangedSignal, std::unique_ptr<Signal<int>> pLivesChangedSignal,
	std::unique_ptr<Signal<int>> pPlayerTwoLivesChangedSignal, KeyboardDevice* pKeyboard, ControllerDevice* pController,
	ControllerDevice* pControllerTwo)
	: Component(pParent)
	, m_pKeyboard(pKeyboard)
	, m_pController(pController)
	, m_pControllerTwo(pControllerTwo)
	, m_pScoreChangedSignal(std::move(pScoreChangedSignal))
	, m_pLivesChangedSignal(std::move(pLivesChangedSignal))
	, m_pPlayerTwoLivesChangedSignal(std::move(pPlayerTwoLivesChangedSignal))
	, m_GameMode(gameMode)

{
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	m_pPlayerTexture = resourceManager.LoadTexture("PlayerSprites.png");
	m_pEnemyTexture = resourceManager.LoadTexture("NobbinSprites.png");
	m_pFireBallTexture = resourceManager.LoadTexture("FireBall.png");
	m_pGemTexture = resourceManager.LoadTexture("Gem.png");
	m_pGoldBagTexture = resourceManager.LoadTexture("GoldBagSprites.png");
	m_pAnyGoldPickedUpSignal = std::make_shared<dae::Signal<dae::GameObject*>>();
	m_pAnyGemPickedUpSignal = std::make_shared<dae::Signal<dae::GameObject*>>();
	m_pAnyEnemyKilledSignal = std::make_shared<dae::Signal<dae::GameObject*>>();
	m_Level = 0;
}



void dae::LevelManagerComponent::Init()
{
	if (m_IsInitialized) return;
	m_IsInitialized = true;
	LoadNextLevel();
}

void dae::LevelManagerComponent::Update()
{

	if (m_ShouldLoadNextLevel)
	{
		LoadNextLevel();
	}
	else
	{
		std::vector<EnemySpawnerComponent*> spawners = GetParent()->GetComponents<EnemySpawnerComponent>();
		bool allSpawnersDead = false;
		for (size_t i = 0; i < spawners.size(); i++)
		{
			if (spawners[i]->GetRemainingCharges())
			{
				allSpawnersDead = true;
			}
		}


		if (GetParent()->GetComponents<GemComponent>().empty() || (GetParent()->GetComponents<EnemyComponent>().empty() && !allSpawnersDead))
		{
			NextLevel();
		}
		std::vector<PlayerComponent*> players = GetParent()->GetComponents<PlayerComponent>();
		bool allPlayersDead = true;
		for (PlayerComponent* pPlayer : players)
		{
			if (pPlayer->GetLives())
			{
				allPlayersDead = false;
			}
		}

		if (allPlayersDead || m_Level == m_Levels.size())
		{
			if (m_GameMode == levelLoader::GameMode::SinglePlayer)
			{
				levelLoader::OpenMenu(m_PlayerName, players[0]->GetParent()->GetComponent<ScoreComponent>()->GetScore());
			}
			else
			{
				levelLoader::OpenMenu();
			}
		}
	}
}

void dae::LevelManagerComponent::NextLevel()
{
	CloseLevel();
	if (m_Level == m_Levels.size()) return;
	m_ShouldLoadNextLevel = true;
}


void dae::LevelManagerComponent::LoadNextLevel()
{
	m_ShouldLoadNextLevel = false;
	const std::ifstream file(m_Levels[m_Level]);
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file: " + m_Levels[m_Level]);
	}
	m_Level++;
	std::stringstream buffer;
	buffer << file.rdbuf();
	const nlohmann::json json = nlohmann::json::parse(buffer.str());
	std::vector<int> playerPos{ json["playerPos"].get<std::vector<int>>() };
	std::vector<int> map{ json["tiles"].get<std::vector<int>>() };
	glm::ivec2 playerSpawnPos{ playerPos[0] ,playerPos[1] };

	dae::TileMapComponent* pTileMap = GetParent()->GetComponent<dae::TileMapComponent>();
	if (!pTileMap)
	{
		std::shared_ptr<dae::Texture2D> pTileSet{ dae::ResourceManager::GetInstance().LoadTexture("tileset.png") };
		std::unique_ptr<dae::TileMapComponent> tileMap{ std::make_unique<dae::TileMapComponent>(GetParent(), pTileSet, glm::ivec2{6,3}, glm::ivec2{40,25}) };
		pTileMap = tileMap.get();
		GetParent()->AddComponent(std::move(tileMap));
	}
	pTileMap->SetMap(map);

	if (!m_pPlayer)
		{
			levelLoader::PlayerRequirements playerRequirements
			{
				GetParent(),
				m_pPlayerTexture,
				m_pFireBallTexture,
				pTileMap->TileToLocal(playerSpawnPos),
				m_pKeyboard,
				m_pController,
				m_pControllerTwo,
				m_pAnyGemPickedUpSignal.get(),
				m_pAnyGoldPickedUpSignal.get(),
				m_pAnyEnemyKilledSignal.get(),
				std::move(m_pScoreChangedSignal),
				std::move(m_pLivesChangedSignal),
				std::move(m_pPlayerTwoLivesChangedSignal),
				m_GameMode,
				0
			};

			AddPlayer(playerRequirements);
			for (const auto& child : GetParent()->GetChildren())
			{
				if (child->GetComponent<PlayerComponent>())
				{
					m_pPlayer = child;
					break;
				}
			}
			if (m_GameMode != levelLoader::GameMode::SinglePlayer)
			{
				playerRequirements.playerIndex = 1;
				AddPlayer(playerRequirements);
			}

		}
	m_pPlayer->GetComponent<PlayerComponent>()->SetRespawnPos(pTileMap->TileToLocal(playerSpawnPos));


	for (const std::vector<int>& bagPos : json["moneyBagPositions"].get<std::vector<std::vector<int>>>())
	{
		levelLoader::AddGoldBag(GetParent(), m_pAnyGoldPickedUpSignal, m_pGoldBagTexture, pTileMap->TileToLocal(glm::ivec2{ bagPos[0], bagPos[1] }));
	}

	for (const std::vector<int>& gemPos : json["gemPositions"].get<std::vector<std::vector<int>>>())
	{
		levelLoader::AddGem(GetParent(), m_pAnyGemPickedUpSignal, m_pGemTexture, pTileMap->TileToLocal(glm::ivec2{ gemPos[0], gemPos[1] }));
	}

	for (const std::vector<int>& spawnerInfo : json["spawnerPositions"].get<std::vector<std::vector<int>>>())
	{
		levelLoader::AddEnemySpawner(GetParent(), pTileMap->TileToLocal(glm::ivec2{ spawnerInfo[0], spawnerInfo[1] }), m_pAnyEnemyKilledSignal, m_pEnemyTexture, spawnerInfo[2], static_cast<float>(spawnerInfo[3]));
	}
	GetParent()->Init();
}

void dae::LevelManagerComponent::CloseLevel()
{
	for (auto& gameObject : GetParent()->GetChildren())
	{
		if (gameObject->GetComponent<PlayerComponent>() || gameObject->GetComponent<TileMapComponent>())
			continue;
		gameObject->Destroy();
	}
}

