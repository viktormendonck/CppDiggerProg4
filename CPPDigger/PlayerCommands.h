#pragma once
#include <chrono>

#include "CollisionRectComponent.h"
#include "FireBallComponent.h"
#include "GameObjectCommand.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "SpriteSheetComponent.h"

namespace dae
{
	class PlayerComponent;

	class MoveCommand final : public GameObjectCommand
	{
	public:
		explicit MoveCommand(GameObject* pGameObject, glm::ivec2 dir) : GameObjectCommand(pGameObject), m_Dir{ dir } {};
	private:
		glm::vec2 m_Dir{};
		void Execute() override
		{
			m_pGameObject->GetComponent<PlayerComponent>()->SetDir(m_Dir);
		}
	};

	class ShootCommand final : public GameObjectCommand
	{
		public:
		explicit ShootCommand(GameObject* pGameObject, const std::shared_ptr<Texture2D>& fireBallTex) : GameObjectCommand(pGameObject), m_pFireBallTex(fireBallTex){};

		void Execute() override
		{
			PlayerComponent* pPlayer = m_pGameObject->GetComponent<PlayerComponent>();
			if (pPlayer->GetCurrentPlayerStateIdx() == static_cast<int>(playerStates::PlayerState::Idle))
			{
				SpawnFireBall(pPlayer->GetDir());
				pPlayer->SetReloading(true);
				m_LastFireTime = std::chrono::high_resolution_clock::now();
			}
		}

	private:
		
		void SpawnFireBall(glm::ivec2 dir)
		{
			auto pTileMap = m_pGameObject->GetParent()->GetComponent<TileMapComponent>();

			const std::shared_ptr<GameObject> pFireBall = std::make_shared<GameObject>();
			pFireBall->GetTransform().SetLocalPosition(pTileMap->TileToLocal(pTileMap->LocalToTile(m_pGameObject->GetTransform().GetLocalPosition()) + glm::ivec2{ 0,1 } + dir) + glm::vec2{ abs(dir.y) * 5,0 });
			std::unique_ptr<SpriteSheetComponent> pSpriteSheet = std::make_unique<SpriteSheetComponent>(pFireBall.get(), m_pFireBallTex, glm::ivec2{3,1}, false, 0.2f, true, true);
			pSpriteSheet->SetRenderScale(glm::vec2(2, 2));
			pFireBall->AddComponent(std::make_unique<CollisionRectComponent>(pFireBall.get(), pSpriteSheet->GetSpriteSize(), glm::vec2{ 0,0 },
				static_cast<uint16_t>(0),
				static_cast<uint16_t>(CollisionLayers::EnemyDamage)
			));
			pFireBall->AddComponent(std::move(pSpriteSheet));
			pFireBall->AddComponent(std::make_unique<FireBallComponent>(pFireBall.get(), dir,m_pGameObject));
			pFireBall->SetParent(m_pGameObject->GetParent(), false);

		}
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFireTime;
		std::shared_ptr<Texture2D> m_pFireBallTex;
		const int m_TimeBetweenShotsInMs{ 6000 };

	};


}
