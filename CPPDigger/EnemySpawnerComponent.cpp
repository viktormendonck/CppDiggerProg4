#include "EnemySpawnerComponent.h"

#include "EnemyComponent.h"
#include "GameData.h"
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
		if (m_RemainingCharges <= 0)
		{
			GetParent()->Destroy();
			return;
		}
		m_RemainingCharges--;
		m_CurrentTime = 0.f;
		auto pEnemy = std::make_shared<GameObject>();
		auto pEnemyComp = std::make_unique<EnemyComponent>(pEnemy.get(), m_pAnyEnemyKilledSignal);
		pEnemy->AddComponent(std::move(pEnemyComp));
		auto pSpriteSheet = std::make_unique<SpriteSheetComponent>(pEnemy.get(), std::shared_ptr(m_pEnemyTexture), glm::ivec2{ 4,3 }, false, 0.2f);
		pSpriteSheet->SetRenderScale(glm::vec2(2, 2));
		pSpriteSheet->SetRenderOffset(glm::vec2(-5, 5));

		pEnemy->AddComponent(std::move(pSpriteSheet));
		pEnemy->GetTransform().SetLocalPosition((GetParent()->GetTransform().GetLocalPosition()));
		pEnemy->SetParent(GetParent()->GetParent(), false);
		pEnemy->Init();
	}
}
