#include "EnemyComponent.h"

#include <random>
#include <glm/gtx/norm.inl>

#include "FireBallComponent.h"
#include "GameData.h"
#include "MapData.h"
#include "PlayerComponent.h"
#include "Renderer.h"

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
			m_TimeBeforeAngered = m_MaxAngerTime;
		}

		void NormalState::Update()
		{
			//update current and previous tile, if the new current tile is different then get a new direction
			m_CurrentCheckingPos = GetStateMachine()->GetParent()->GetTransform().GetLocalPosition() + glm::vec2(m_pSpriteSheet->GetSpriteSize().x / 2, m_pSpriteSheet->GetSpriteSize().y / 3);
			glm::ivec2 newTile = m_pTileMap->LocalToTile(m_CurrentCheckingPos);
			if (newTile != m_CurrentTile)
			{
				m_PreviousTile = m_CurrentTile;
				m_CurrentTile = newTile;
				m_ShouldCheckTile = true;
			}
			if (m_ShouldCheckTile)
			{
				if (m_CurrentDecisionTime >= m_DecisionTime)
				{
					GetNextDir();
					m_CurrentDecisionTime = 0;
					m_ShouldCheckTile = false;
				}
				else
				{
					m_CurrentDecisionTime += GameData::GetInstance().GetDeltaTime();
				}
			}
			m_TimeBeforeAngered -= GameData::GetInstance().GetDeltaTime();
			if (m_TimeBeforeAngered <= 0)
			{
				GetStateMachine()->SetState(static_cast<int>(EnemyState::Angered));
			}
			GetStateMachine()->GetParent()->GetTransform().Translate(m_Dir * m_Speed * GameData::GetInstance().GetDeltaTime());
		}

		void NormalState::GetNextDir()
		{
			//get the tile above, below to the left and right of the enemy and see if they are empty
			const std::vector<glm::ivec2> directions{ {0,-1},{0,1},{-1,0},{1,0} };
			std::vector<bool> validDirections(4, false);
			bool foundValidDir{};
			for (size_t i{}; i < directions.size(); ++i)
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
				if (rand()%5 != 0)
				{

					float dist{ FLT_MAX };
					glm::vec2 closestPlayerPos = GetStateMachine()->GetParent()->GetComponent<EnemyComponent>()->GetClosestPlayerPos();
					for (size_t i{}; i < directions.size(); ++i)
					{
						float newDist{ distance2(GetStateMachine()->GetParent()->GetTransform().GetLocalPosition() + glm::vec2(directions[i] * 8), closestPlayerPos) };
						if (newDist < dist && validDirections[i])
						{
							dist = newDist;
							m_Dir = directions[i];
						}
					}
				} else
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

		void AngeredState::Init()
		{
			m_pTileMap = GetStateMachine()->GetParent()->GetPatriarch()->GetComponent<TileMapComponent>();
			m_pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			m_CurrentTile = m_pTileMap->LocalToTile(GetStateMachine()->GetParent()->GetTransform().GetLocalPosition());
			m_pEnemyComponent = GetStateMachine()->GetParent()->GetComponent<EnemyComponent>();
			GetNextDir();
		}

		void AngeredState::OnEnter()
		{
			m_pSpriteSheet->SetSprite({ 0,1 });
			m_TimeBeforeCalmed = m_MaxCalmTime;
		}

		void AngeredState::Update()
		{
			m_CurrentCheckingPos = GetStateMachine()->GetParent()->GetTransform().GetLocalPosition() + glm::vec2(m_pSpriteSheet->GetSpriteSize().x / 2, m_pSpriteSheet->GetSpriteSize().y / 3);
			glm::ivec2 newTile = m_pTileMap->LocalToTile(m_CurrentCheckingPos);
			if (newTile != m_CurrentTile)
			{
				m_PreviousTile = m_CurrentTile;
				m_CurrentTile = newTile;
				m_ShouldCheckTile = true;
			}
			if (m_ShouldCheckTile)
			{
				if (m_CurrentDecisionTime >= m_DecisionTime)
				{
					GetNextDir();
					GetStateMachine()->GetParent()->GetComponent<EnemyComponent>()->Dig(m_MoveDir);
					m_CurrentDecisionTime = 0;
					m_ShouldCheckTile = false;
				}
				else
				{
					m_CurrentDecisionTime += GameData::GetInstance().GetDeltaTime();
				}
			}
			m_TimeBeforeCalmed -= GameData::GetInstance().GetDeltaTime();
			if (m_TimeBeforeCalmed <= 0)
			{
				GetStateMachine()->SetState(static_cast<int>(EnemyState::Angered));
			}
			GetStateMachine()->GetParent()->GetTransform().Translate(m_MoveDir * m_Speed * GameData::GetInstance().GetDeltaTime());
		}

		void AngeredState::GetNextDir()
		{
			const glm::vec2 closestPlayerPos = m_pEnemyComponent->GetClosestPlayerPos();
			const glm::vec2 playerDir = closestPlayerPos - GetStateMachine()->GetParent()->GetTransform().GetLocalPosition();
			if (abs(playerDir.x) > abs(playerDir.y))
			{
				m_MoveDir = { playerDir.x / abs(playerDir.x),0 };
			}
			else
			{
				m_MoveDir = { 0,playerDir.y / abs(playerDir.y) };
			}
		}
	}



	EnemyComponent::EnemyComponent(GameObject* pParent, std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal, Signal<>* onPLayerDeath)
		:
		DiggingCharacterComponent(pParent),
		m_pAnyEnemyKilledSignal(std::move(pAnyEnemyKilledSignal))
	{
		m_pStateMachine = std::make_unique<FiniteStateMachine>(pParent);
		m_pStateMachine->AddState(std::make_unique < enemyStates::NormalState>());
		m_pStateMachine->AddState(std::make_unique < enemyStates::AngeredState>());

		onPLayerDeath->AddListener([this]() {OnPlayerDeath();});
	}

	auto EnemyComponent::Init() -> void
	{
		m_pStateMachine->Init();
		m_pStateMachine->SetState(static_cast<int>(enemyStates::EnemyState::Normal));
		if (CollisionRectComponent* pCollisionRect = GetParent()->GetComponent<CollisionRectComponent>())
		{
			pCollisionRect->m_OnEnter.AddListener([this](CollisionRectComponent* pOther) {OnCollision(pOther); });
		}
	}

	void EnemyComponent::Update()
	{
		m_pStateMachine->Update();
	}

	void EnemyComponent::Render() const
	{
		m_pStateMachine->Render();
	}

	void EnemyComponent::Dig(glm::vec2 dir)
	{
		DiggingCharacterComponent::Dig(dir);
	}

	glm::vec2 EnemyComponent::GetClosestPlayerPos() const
	{
		float closestDist{ FLT_MAX };
		glm::vec2 closestPos{};
		for (const PlayerComponent* pPlayerComponent : GetParent()->GetPatriarch()->GetComponents<PlayerComponent>())
		{
			float dist = glm::distance2(pPlayerComponent->GetParent()->GetTransform().GetLocalPosition(), GetParent()->GetTransform().GetLocalPosition());
			if (dist < closestDist)
			{
				closestDist = dist;
				closestPos = pPlayerComponent->GetParent()->GetTransform().GetLocalPosition();
			}
		}
		return closestPos;

	}

	void EnemyComponent::OnPlayerDeath()
	{
		if (GetParent())
		{
			GetParent()->Destroy();
		}
	}

	void EnemyComponent::OnCollision(CollisionRectComponent* pOther)
	{

		if (pOther->ExistsOn(static_cast<int>(CollisionLayers::EnemyDamage)))
		{
			GetParent()->Destroy();
			if (FireBallComponent* pFireball = pOther->GetParent()->GetComponent<FireBallComponent>())
			{
				pOther->GetParent()->Destroy();
				m_pAnyEnemyKilledSignal->Emit(pFireball->GetPlayerObject());
				return;
			}

			m_pAnyEnemyKilledSignal->Emit(pOther->GetParent());


		}
	}

}
