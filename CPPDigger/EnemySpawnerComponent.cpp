#include "EnemySpawnerComponent.h"

#include "EnemyComponent.h"
#include "GameData.h"
#include "MapData.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"

namespace dae
{
	EnemySpawnerComponent::EnemySpawnerComponent(GameObject* pParent,
		std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal, std::shared_ptr<Texture2D> pEnemyTex, int charges,
		float timeBetweenSpawns) : Component(pParent)
		, m_pAnyEnemyKilledSignal{ std::move(pAnyEnemyKilledSignal) }
		, m_pEnemyTexture{ std::move(pEnemyTex) }
		, m_RemainingCharges{ charges }
		, m_TimeBetweenSpawns{ timeBetweenSpawns }
	{}

	void EnemySpawnerComponent::Update()
	{
		if (m_CurrentTime >= m_TimeBetweenSpawns)
		{
			SpawnEnemy();
		}
		else
		{
			m_CurrentTime += GameData::GetInstance().GetDeltaTime();
		}
	}

	void EnemySpawnerComponent::SpawnEnemy()
	{
		Signal<>* pPlayerDeathSignal{};
		for (const std::shared_ptr<GameObject>& child : GetParent()->GetPatriarch()->GetChildren())
		{
			if (PlayerComponent* player = child->GetComponent<PlayerComponent>())
			{
				pPlayerDeathSignal = &player->onDeath;
			}
		}
		if (m_RemainingCharges <= 0)
		{
			GetParent()->Destroy();
			return;
		}
		m_RemainingCharges--;
		m_CurrentTime = 0.f;
		std::shared_ptr<GameObject> pEnemy = std::make_shared<GameObject>();
		std::unique_ptr<EnemyComponent> pEnemyComp = std::make_unique<EnemyComponent>(
			pEnemy.get(), m_pAnyEnemyKilledSignal,pPlayerDeathSignal);
		pEnemy->AddComponent(std::move(pEnemyComp));
		std::unique_ptr<SpriteSheetComponent>pSpriteSheet = std::make_unique<SpriteSheetComponent>(
			pEnemy.get(), std::shared_ptr(m_pEnemyTexture), glm::ivec2{4, 3}, false, 0.2f);
		pSpriteSheet->SetRenderScale(glm::vec2(1.5f, 1.5f));
		pEnemy->AddComponent(std::make_unique<CollisionRectComponent>(pEnemy.get(), pSpriteSheet->GetSpriteSize(), glm::vec2(0, 2), 
			uint16_t{ static_cast<uint16_t>(CollisionLayers::EnemyDamage)},
			uint16_t{ static_cast<uint16_t>(CollisionLayers::Pickup) | static_cast<uint16_t>(CollisionLayers::PlayerDamage)}
		));

		pEnemy->AddComponent(std::move(pSpriteSheet));
		pEnemy->GetTransform().SetLocalPosition((GetParent()->GetTransform().GetLocalPosition()));
		pEnemy->SetParent(GetParent()->GetParent(), false);
		pEnemy->Init();
	}
}
