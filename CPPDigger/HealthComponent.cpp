#include "HealthComponent.h"

#include "GameObject.h"
#include "PlayerComponent.h"


dae::HealthComponent::HealthComponent(GameObject* pParent, std::unique_ptr<Signal<int>> pLivesChangedSignal)
	: Component(pParent)
	, livesChangedSignal(std::move(pLivesChangedSignal))
{
}

void dae::HealthComponent::Init()
{
	auto player = GetParent()->GetComponent<PlayerComponent>();
	livesChangedSignal->Emit(player->GetLives());
	player->onDeath.AddListener([this]() {this->OnPlayerDeath(); });
}

void dae::HealthComponent::OnPlayerDeath()
{
	livesChangedSignal->Emit(GetParent()->GetComponent<PlayerComponent>()->GetLives());
}

