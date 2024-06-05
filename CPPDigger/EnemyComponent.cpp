#include "EnemyComponent.h"

#include "GameData.h"
#include "MapData.h"

namespace dae
{
	namespace enemyStates
	{
		void NormalState::Init()
		{
			m_pTileMap = GetStateMachine()->GetParent()->GetPatriarch()->GetComponent<TileMapComponent>();
			m_pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			m_CurrentTile = m_pTileMap->LocalToTile(GetStateMachine()->GetParent()->GetTransform().GetLocalPosition());
			GetNextDir();
		}

		void NormalState::OnEnter()
		{
			m_pSpriteSheet->SetSprite({ 0,0 });
		}

		void NormalState::Update()
		{
			//update current and previous tile, if the new current tile is different then get a new direction
			glm::ivec2 newTile = m_pTileMap->LocalToTile(GetStateMachine()->GetParent()->GetTransform().GetLocalPosition());
			if (newTile != m_CurrentTile)
			{
				m_PreviousTile = m_CurrentTile;
				m_CurrentTile = newTile;
				GetNextDir();
			}
			GetStateMachine()->GetParent()->GetTransform().Translate(m_Dir * m_Speed * GameData::GetInstance().GetDeltaTime());
		}

		void NormalState::GetNextDir()
		{
			//get the tile above, below to the left and right of the enemy and see if they are empty
			const std::vector<glm::ivec2> directions{ {0,-1},{0,1},{-1,0},{1,0} };
			std::vector<bool> validDirections(4, false);
			bool foundValidDir{};
			for (int i{}; i < directions.size(); ++i)
			{
				glm::ivec2 nextTile = m_CurrentTile + directions[i];
				if (m_pTileMap->GetTileSprite(nextTile) == static_cast<int>(MapData::TileType::Empty) && nextTile != m_PreviousTile)
				{
					validDirections[i] = true;
					foundValidDir = true;
				}
			}
			if (!foundValidDir)
			{
				m_Dir *= -1;
			}
			else
			{
				int dirIdx{};
				do
				{
					dirIdx = rand() % 4;
				} while (!validDirections[dirIdx]);
				m_Dir = directions[dirIdx];
			}
		}
	}


	EnemyComponent::EnemyComponent(GameObject* pParent, std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal)
		:
		DiggingCharacterComponent(pParent),
		m_pAnyEnemyKilledSignal(std::move(pAnyEnemyKilledSignal))
	{
		m_pStateMachine = std::make_unique<FiniteStateMachine>(pParent);
		m_pStateMachine->AddState(std::make_unique < enemyStates::NormalState>());
	}

	auto EnemyComponent::Init() -> void
	{
		m_pStateMachine->Init();
		m_pStateMachine->SetState(static_cast<int>(enemyStates::EnemyState::Normal));
	}

	void EnemyComponent::Update()
	{
		m_pStateMachine->Update();
	}

	void EnemyComponent::OnPlayerDeath()
	{
		GetParent()->Destroy();
	}

	void EnemyComponent::OnCollision(CollisionRectComponent* pOther)
	{
		if (pOther->ExistsOn(static_cast<int>(CollisionLayers::EnemyDamage)))
		{
			GetParent()->Destroy();
		}
	}

}