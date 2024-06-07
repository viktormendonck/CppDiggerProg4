#include "ScoreComponent.h"

#include "GameObject.h"


dae::ScoreComponent::ScoreComponent(GameObject* pParent, Signal<GameObject*>* pGemSignal, Signal<GameObject*>* pGoldBagSignal, Signal<GameObject*>* pEnemySignal, std::unique_ptr<Signal<int>> pScoreChangedSignal)
	: Component(pParent),
	m_pScoreChangedSignal(std::move(pScoreChangedSignal))
{
	pGemSignal->AddListener([this](GameObject* pPlayer) { OnGemPickedUp(pPlayer); });
	pGoldBagSignal->AddListener([this](GameObject* pPlayer) { OnGoldPickedUp(pPlayer); });
	pEnemySignal->AddListener([this](GameObject* pPlayer) { OnEnemyKill(pPlayer); });
}

void dae::ScoreComponent::OnEnemyKill(const GameObject* killingCharacter)
{
	if (GetParent() != killingCharacter && m_pSecondPlayerObject != killingCharacter)return;
	m_Score += m_EnemyKillScore;
	m_pScoreChangedSignal->Emit(m_Score);
}

void dae::ScoreComponent::OnGoldPickedUp(const GameObject* pPickupCharacter)
{
	if (GetParent() != pPickupCharacter && m_pSecondPlayerObject != pPickupCharacter)return;
	m_Score += m_GoldScore;
	m_pScoreChangedSignal->Emit(m_Score);
}


void dae::ScoreComponent::OnGemPickedUp(const GameObject* pPickupCharacter)
{
	using namespace std::chrono;
	if (GetParent() != pPickupCharacter && m_pSecondPlayerObject != pPickupCharacter)return;

	if (duration_cast<milliseconds>(high_resolution_clock::now() - m_LastGemPickup).count() > m_GemBonusMaxTimeMs)
	{
		m_NumConsecutiveGems = 0;
	}
	++m_NumConsecutiveGems;
	if (m_NumConsecutiveGems >= m_ConsecutiveGemsForBonus)
	{
		m_Score += m_ConsecutiveGemBonusScore;
		m_NumConsecutiveGems = 0;
	}
	m_Score += m_GemScore;
	m_LastGemPickup = high_resolution_clock::now();
	m_pScoreChangedSignal->Emit(m_Score);
}
