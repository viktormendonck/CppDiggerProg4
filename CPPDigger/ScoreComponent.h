#pragma once
#include <chrono>

#include "Component.h"
#include "Signal.h"

namespace dae
{
	class ScoreComponent final : public Component	
	{
	public:
		ScoreComponent(GameObject* pParent, Signal<GameObject*>* pGemSignal, Signal<GameObject*>* pGoldBagSignal, Signal<GameObject*>* pEnemySignal,std::unique_ptr<Signal<int>> pScoreChangedSignal);
		int GetScore() const { return m_Score; }
		void SetSecondPlayer(GameObject* player) { m_pSecondPlayerObject = player; }
	private:
		int m_Score{};

		void OnEnemyKill(const GameObject* killingCharacter);
		const int m_EnemyKillScore{ 250 };

		void OnGoldPickedUp(const GameObject* pPickupCharacter);
		const int m_GoldScore{500};

		void OnGemPickedUp(const GameObject* pPickupCharacter);
		int m_NumConsecutiveGems{};
		std::chrono::time_point<std::chrono::steady_clock> m_LastGemPickup = std::chrono::high_resolution_clock::now();
		const long long m_GemBonusMaxTimeMs{ 1000 };
		const int m_ConsecutiveGemsForBonus{ 8 };
		const int m_GemScore{ 25 };
		const int m_ConsecutiveGemBonusScore{ 250 };
		std::unique_ptr<Signal<int>> m_pScoreChangedSignal{};

		GameObject* m_pSecondPlayerObject{}; //needed for co-op
	};
}
