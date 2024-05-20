#include "GoldBagComponent.h"

#include <iostream>
#include <SDL_syswm.h>
#include <stdexcept>
#include <glm/vec2.hpp>

#include "GameData.h"
#include "MapData.h"
#include "memory"
#include "TileMapComponent.h"
#include "FiniteStateMachine.h"
#include "CollisionRectComponent.h"
#include "PlayerComponent.h"
#include "Renderer.h"
#include "SpriteSheetComponent.h"

namespace dae
{
	class PlayerComponent;

	namespace goldBagInfo
	{
		void IdleState::Init()
		{
			m_pGoldBag = GetStateMachine()->GetParent();
			m_pTileMap = m_pGoldBag->GetPatriarch()->GetComponent<TileMapComponent>();
			m_pSpriteSheet = m_pGoldBag->GetComponent<SpriteSheetComponent>();
			
		}

		void IdleState::OnEnter()
		{
			if (m_pSpriteSheet) m_pSpriteSheet->SetSprite(glm::ivec2{ 2, 1 }); //startSprite for idle
		}

		void IdleState::Update()
		{
			glm::vec2 OriginalSpriteSize{ m_pSpriteSheet->GetSpriteSize()};
			glm::ivec2 pos = m_pTileMap->LocalToTile(m_pGoldBag->GetTransform().GetLocalPosition()+glm::vec2(OriginalSpriteSize.x/2, OriginalSpriteSize.y/2));
			MapData::TileType checkTile = static_cast<MapData::TileType>(m_pTileMap->GetTileSprite({ pos.x, pos.y + 1 }));
			if (checkTile == MapData::TileType::TopWall || checkTile == MapData::TileType::Empty || checkTile == MapData::TileType::TopMiddleRoundOff)
			{
				GetStateMachine()->SetState(static_cast<int>(GoldBagStates::Wiggle));
			}
		}

		void WiggleState::OnEnter()
		{
			m_OriginalPos = GetStateMachine()->GetParent()->GetTransform().GetLocalPosition();
			m_pSpriteSheet = GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>();
			m_CurrentWiggleTime = 0;
		}

		void WiggleState::Update()
		{
			float dt = GameData::GetInstance().GetDeltaTime();
			if (m_CurrentWiggleTime <= m_MaxWiggleTime)
			{
				m_CurrentWiggleTime += dt;
				m_pSpriteSheet->SetRenderOffset(glm::vec2{ cosf(m_CurrentWiggleTime * 15) * 3,0 });
			} else
			{
				GetStateMachine()->SetState(static_cast<int>(GoldBagStates::Falling));
			}
			
		}
		void WiggleState::OnExit()
		{
			m_pSpriteSheet->SetRenderOffset(glm::vec2{ 0,0 });
		}

		void FallingState::Init()
		{
			m_pGoldBag = GetStateMachine()->GetParent();
			m_pTileMap = m_pGoldBag->GetPatriarch()->GetComponent<TileMapComponent>();
			m_pSpriteSheet = m_pGoldBag->GetComponent<SpriteSheetComponent>();
		}

		void FallingState::OnEnter()
		{
			m_pSpriteSheet->SetSprite(glm::ivec2{ 0, 0 }); //startSprite for falling
			//change the collision layers so that the goldbag can damage players and enemies
			std::cout << m_pGoldBag->GetComponent<CollisionRectComponent>()->GetSendingCollisionLayers() << std::endl;
			m_pGoldBag->GetComponent<CollisionRectComponent>()->GetSendingCollisionLayers() |= uint16_t{static_cast<uint16_t>(CollisionLayers::PlayerDamage) | static_cast<uint16_t>(CollisionLayers::EnemyDamage)};
			std::cout << m_pGoldBag->GetComponent<CollisionRectComponent>()->GetSendingCollisionLayers() << std::endl;
		}

		void FallingState::Update()
		{
			const float dt = GameData::GetInstance().GetDeltaTime();
			glm::ivec2 pos = m_pTileMap->LocalToTile(m_pGoldBag->GetTransform().GetLocalPosition() + (m_pSpriteSheet->GetSpriteSize().x / 2, m_pSpriteSheet->GetSpriteSize().y/4));
			MapData::TileType checkTile = static_cast<MapData::TileType>(m_pTileMap->GetTileSprite(pos));
			//the tile underneath is BottomRightCorner, BottomLeftCorner, bottomMiddleRoundoff or BottomWall then stop falling and it if has fallen for more than 3 tiles then go to goldState
			if (m_FallDist > 1 &&
				(checkTile == MapData::TileType::BottomRightCorner	|| 
				checkTile == MapData::TileType::BottomLeftCorner	|| 
				checkTile == MapData::TileType::BottomMiddleRoundOff|| 
				checkTile == MapData::TileType::BottomWall))
			{
				if (m_FallDist >= m_MinBreakDist)
				{
					GetStateMachine()->SetState(static_cast<int>(GoldBagStates::Gold));
				}
				else
				{
					GetStateMachine()->SetState(static_cast<int>(GoldBagStates::Idle));
				}
			}
			else
			{
				m_pGoldBag->GetTransform().Translate(glm::vec2(0, m_FallSpeed * dt));
				if (pos.y != m_LastTilePos.y)
				{
					m_LastTilePos = pos;
					m_FallDist++;
				}
			}
		}

		void FallingState::OnExit()
		{
			m_LastTilePos= glm::ivec2{ 0,0 };
			m_FallDist = 0;
		}
		void GoldState::Init()
		{
			GetStateMachine()->GetParent()->GetComponent<CollisionRectComponent>()->m_OnEnter.AddListener([this](GameObject* other) {OnPlayerCollision(other); });
		}

		void GoldState::OnEnter()
		{
			GetStateMachine()->GetParent()->GetComponent<SpriteSheetComponent>()->SetSprite(glm::ivec2{0,2});//startSprite for gold
		}

		void GoldState::OnPlayerCollision(GameObject* pOther)
		{
			if (pOther->GetComponent<PlayerComponent>() != nullptr )
			{
				if (GetStateMachine()->GetState() ==this)
				{
					GetStateMachine()->SetState(static_cast<int>(GoldBagStates::Taken));
				}
			}

			//TODO: add score (currently don't have a score system)
		}

		void TakenState::OnEnter()
		{
			GetStateMachine()->GetParent()->Destroy();
		}
	}


	GoldBagComponent::GoldBagComponent(GameObject* pParent)
		: Component(pParent)
	{
		m_pFSM = std::make_unique<FiniteStateMachine>(pParent);
	}

	void GoldBagComponent::Init()
	{
		std::unique_ptr<goldBagInfo::IdleState> pIdleState = std::make_unique<goldBagInfo::IdleState>();
		m_pFSM->AddState(std::move(pIdleState));
		std::unique_ptr<goldBagInfo::WiggleState> pWiggleState = std::make_unique<goldBagInfo::WiggleState>();
		m_pFSM->AddState(std::move(pWiggleState));
		std::unique_ptr<goldBagInfo::FallingState> pFallingState = std::make_unique<goldBagInfo::FallingState>();
		m_pFSM->AddState(std::move(pFallingState));
		std::unique_ptr<goldBagInfo::GoldState> pGoldState = std::make_unique<goldBagInfo::GoldState>();
		m_pFSM->AddState(std::move(pGoldState));
		std::unique_ptr<goldBagInfo::TakenState> pTakenState = std::make_unique<goldBagInfo::TakenState>();
		m_pFSM->AddState(std::move(pTakenState));
		m_pFSM->SetState(static_cast<int>(goldBagInfo::GoldBagStates::Idle));
		m_pFSM->Init();
	}

	void GoldBagComponent::Update()
	{
		m_pFSM->Update();
	}
}
