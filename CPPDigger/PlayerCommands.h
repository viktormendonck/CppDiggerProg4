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
			m_pGameObject->GetComponent<SpriteSheetComponent>()->SetSprite({ 0,0 });
			if (abs(m_Dir.x) > glm::epsilon<float>())
			{
				m_pGameObject->GetTransform().SetLocalRotation(0);
				if (m_Dir.x < 0)
				{
					m_pGameObject->GetComponent<SpriteSheetComponent>()->SetSprite({ 0,1 });
				}
			}
			else
			{
				m_pGameObject->GetTransform().SetLocalRotation(glm::pi<float>()/ 2.f * m_Dir.y);
			}
			m_pGameObject->GetComponent<PlayerComponent>()->SetDir(m_Dir);
		}
	};

	class ShootCommand final : public GameObjectCommand
	{
		public:
		explicit ShootCommand(GameObject* pGameObject, const std::shared_ptr<Texture2D>& fireBallTex) : GameObjectCommand(pGameObject), m_pFireBallTex(fireBallTex){};

		void Execute() override
		{
			auto pPlayer = m_pGameObject->GetComponent<PlayerComponent>();
			if (CanShoot())
			{
				SpawnFireBall(pPlayer->GetDir());
				m_LastFireTime = std::chrono::high_resolution_clock::now();
			}
		}

	private:
		bool CanShoot() const
		{
			const std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::high_resolution_clock::now();
			const long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_LastFireTime).count();
			return duration > m_TimeBetweenShotsInMs;
		}
		void SpawnFireBall(glm::ivec2 dir)
		{
			auto pTileMap = m_pGameObject->GetParent()->GetComponent<TileMapComponent>();

			const std::shared_ptr<GameObject> pFireBall = std::make_shared<GameObject>();
			pFireBall->GetTransform().SetLocalPosition(pTileMap->TileToLocal(pTileMap->LocalToTile(m_pGameObject->GetTransform().GetLocalPosition())+glm::ivec2{0,1}+ dir));
			std::unique_ptr<SpriteSheetComponent> pSpriteSheet = std::make_unique<SpriteSheetComponent>(pFireBall.get(), m_pFireBallTex, glm::ivec2{3,1}, false, 0.2f, true, true);
			pSpriteSheet->SetRenderScale(glm::vec2(2, 2));
			pFireBall->AddComponent(std::make_unique<CollisionRectComponent>(pFireBall.get(), pSpriteSheet->GetSpriteSize(), glm::vec2{ 0,0 },
				static_cast<uint16_t>(CollisionLayers::EnemyDamage),
				static_cast<uint16_t>(CollisionLayers::EnemyDamage)));
			pFireBall->AddComponent(std::move(pSpriteSheet));
			pFireBall->AddComponent(std::make_unique<FireBallComponent>(pFireBall.get(), dir));
			pFireBall->SetParent(m_pGameObject->GetParent(), false);

		}
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFireTime;
		std::shared_ptr<Texture2D> m_pFireBallTex;
		const int m_TimeBetweenShotsInMs{ 6000 };

	};


}
