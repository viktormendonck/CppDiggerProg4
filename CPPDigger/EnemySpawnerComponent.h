#pragma once
#include <memory>

#include "Component.h"
#include "Signal.h"
#include "Texture2D.h"

namespace dae
{
	class EnemySpawnerComponent final : public Component
	{
	public:
		EnemySpawnerComponent(GameObject* pParent, std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal, std::shared_ptr<Texture2D> pEnemyTex, int charges, float timeBetweenSpawns);

		void Update() override;
		int GetRemainingCharges() const { return m_RemainingCharges; }

	private:
		std::shared_ptr<Signal<GameObject*>> m_pAnyEnemyKilledSignal{};
		std::shared_ptr<Texture2D> m_pEnemyTexture{};
		int m_RemainingCharges{};
		const float m_TimeBetweenSpawns{};
		float m_CurrentTime{};

		void SpawnEnemy();
	};

}