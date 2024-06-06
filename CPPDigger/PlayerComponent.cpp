#include "PlayerComponent.h"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/vec2.hpp>
#include "GameData.h"
#include "GemComponent.h"
#include "MapData.h"
#include "SpriteSheetComponent.h"
#include "TileMapComponent.h"


namespace dae
{
	namespace playerStates
	{

		void IdleState::OnEnter()
		{
			SpriteSheetComponent* pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			pSpriteSheet->SetSprite({ 0,0 });
			GetStateMachine()->GetParent()->GetTransform().SetLocalRotation(0);
		}

		void IdleState::Update()
		{
			if (GetStateMachine()->GetParent()->GetComponent<PlayerComponent>()->IsReloading())
			{
				GetStateMachine()->SetState(static_cast<int>(PlayerState::Reloading));
			}
		}

		void IdleState::OnExit()
		{
			SpriteSheetComponent* pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			glm::ivec2 currentSprite = pSpriteSheet->GetSprite();
			pSpriteSheet->SetSprite({ currentSprite.x,currentSprite.y + 2 });
		}

		void ReloadingState::Update()
		{
			if (m_CurrentTime >= m_ReloadTime)
			{
				GetStateMachine()->SetState(static_cast<int>(PlayerState::Idle));
				m_CurrentTime = 0;
				GetStateMachine()->GetParent()->GetComponent<PlayerComponent>()->SetReloading(false);
			}
			else
			{
				m_CurrentTime += GameData::GetInstance().GetDeltaTime();
			}
		}

		void ReloadingState::OnExit()
		{
			SpriteSheetComponent* pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			glm::ivec2 currentSprite = pSpriteSheet->GetSprite();
			pSpriteSheet->SetSprite({ currentSprite.x,currentSprite.y - 2 });
		}


		void DeadState::Init()
		{
			m_pPlayer = GetStateMachine()->GetParent();
			m_pTileMap = m_pPlayer->GetPatriarch()->GetComponent<TileMapComponent>();
			m_pSpriteSheet = m_pPlayer->GetComponent<SpriteSheetComponent>();
		}

		void DeadState::OnEnter()
		{
			GetStateMachine()->GetParent()->GetTransform().SetLocalRotation(0);
			const glm::ivec2 deathSprite{ 3,4 };
			m_pSpriteSheet->SetSprite(deathSprite);
			m_pSpriteSheet->ShouldRepeat(false);
		}

		void DeadState::Update()
		{
			const float dt = GameData::GetInstance().GetDeltaTime();
			const glm::ivec2 checkPos = m_pTileMap->LocalToTile(m_pPlayer->GetTransform().GetLocalPosition() + glm::vec2(m_pSpriteSheet->GetSpriteSize().x / 2, 0));
			MapData::TileType checkTile{};
			//check for validity of the position
			bool shouldLand{ false };
			if (checkPos.x < 0 || checkPos.x >= m_pTileMap->GetWorldSize().x - 1 || checkPos.y < 0 || checkPos.y >= m_pTileMap->GetWorldSize().y - 1)
			{
				shouldLand = true;
			}
			else
			{
				checkTile = static_cast<MapData::TileType>(m_pTileMap->GetTileSprite(checkPos));
			}

			for (MapData::TileType tile : m_LandingTiles)
			{
				if (checkTile == tile)
				{
					shouldLand = true;
				}
			}
			if (shouldLand)
			{
				m_pPlayer->GetTransform().SetLocalPosition(m_pPlayer->GetComponent<PlayerComponent>()->GetRespawnPos());
				GetStateMachine()->SetState(static_cast<int>(PlayerState::Idle));

			}
			else
			{
				m_pPlayer->GetTransform().SetLocalPosition(m_pPlayer->GetTransform().GetLocalPosition() + glm::vec2{ 0, m_FallSpeed * dt });
			}

		}

		void DeadState::OnExit()
		{
			const glm::ivec2 defaultSprite{ 0,0 };
			m_pSpriteSheet->SetSprite(defaultSprite);
			m_pSpriteSheet->ShouldRepeat(true);
		}

	}
	PlayerComponent::PlayerComponent(dae::GameObject* pParent)
		: DiggingCharacterComponent(pParent)
	{
		m_pStateMachine = std::make_unique<FiniteStateMachine>(pParent);
		m_pStateMachine->AddState(std::make_unique<playerStates::IdleState>());
		m_pStateMachine->AddState(std::make_unique<playerStates::ReloadingState>());
		m_pStateMachine->AddState(std::make_unique<playerStates::DeadState>());
		m_pStateMachine->SetState(static_cast<int>(playerStates::PlayerState::Idle));
	}


	void PlayerComponent::Init()
	{
		if (CollisionRectComponent* pCollision = GetParent()->GetComponent<CollisionRectComponent>())
		{
			pCollision->m_OnEnter.AddListener([this](CollisionRectComponent* pOther) {OnCollision(pOther); });
		}
		m_pStateMachine->Init();
		m_RespawnPos = GetParent()->GetTransform().GetLocalPosition();
	}

	void PlayerComponent::Update()
	{
		m_pStateMachine->Update();
		if (m_pStateMachine->GetCurrentStateIdx() == static_cast<int>(playerStates::PlayerState::Dead)) return;
		if (m_CanMove == false)
		{
			const glm::vec2 pos = GetParent()->GetTransform().GetLocalPosition();
			const glm::vec2 dir = glm::normalize(glm::vec2{ m_TargetPos.x - pos.x,m_TargetPos.y - pos.y });
			GetParent()->GetTransform().SetLocalPosition(GetParent()->GetTransform().GetLocalPosition() + dir * m_Speed * GameData::GetInstance().GetDeltaTime());
			if (glm::distance(pos, m_TargetPos) < 0.1f)
			{
				GetParent()->GetTransform().SetLocalPosition(m_TargetPos);
				m_CanMove = true;
			}
		}
	}


	void PlayerComponent::SetDir(glm::ivec2 dir)
	{
		if (!m_CanMove) return;
		int shootYOffset{ static_cast<int>(m_IsReloading) * 2 };
		int currentSpriteX = GetParent()->GetComponent<SpriteSheetComponent>()->GetSprite().x;
		GetParent()->GetComponent<SpriteSheetComponent>()->SetSprite({ currentSpriteX,0 + shootYOffset });
		if (abs(dir.x) > glm::epsilon<float>())
		{
			GetParent()->GetTransform().SetLocalRotation(0);
			if (dir.x < 0)
			{
				GetParent()->GetComponent<SpriteSheetComponent>()->SetSprite({ currentSpriteX,1 + shootYOffset });
			}
		}
		else
		{
			GetParent()->GetTransform().SetLocalRotation(glm::pi<float>() / 2.f * dir.y);
		}


		if (m_pStateMachine->GetCurrentStateIdx() == static_cast<int>(playerStates::PlayerState::Dead)) return;
		TileMapComponent* pTileMap = GetTileMap();
		dir = glm::normalize(glm::vec2(dir.x, dir.y));
		const glm::vec2 currentTile = pTileMap->LocalToTile(GetParent()->GetTransform().GetLocalPosition());
		const glm::ivec2 goalTile = dir + glm::ivec2(static_cast<int>(currentTile.x), static_cast<int>(currentTile.y));
		const glm::ivec2 worldSize{ pTileMap->GetWorldSize() };
		if (goalTile.x < 1 || goalTile.x >= worldSize.x - 2 || goalTile.y < 1 || goalTile.y >= worldSize.y - 2) return;
		m_TargetPos = pTileMap->TileToLocal(goalTile);
		m_CanMove = false;
		m_CurrentDir = dir;
		Dig(dir);
	}

	void PlayerComponent::HitWall()
	{
		m_TargetPos = GetParent()->GetTransform().GetLocalPosition();
		m_CanMove = true;
	}

	void PlayerComponent::OnCollision(const CollisionRectComponent* pOther)
	{
		if (pOther->ExistsOn(static_cast<uint16_t>(CollisionLayers::PlayerDamage)) &&
			m_pStateMachine->GetCurrentStateIdx() != static_cast<int>(playerStates::PlayerState::Dead))
		{
			pOther->GetParent()->Destroy();
			if (m_Lives > 0)
			{
				StartRespawn();
			}

			//GetParent()->Destroy();

		}
	}

	void PlayerComponent::StartRespawn()
	{
		m_pStateMachine->SetState(static_cast<int>(playerStates::PlayerState::Dead));
		m_Lives--;
		onDeath.Emit();
	}
}
