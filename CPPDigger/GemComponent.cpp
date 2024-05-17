#include "GemComponent.h"

#include "CollisionRectComponent.h"
#include "PlayerComponent.h"
#include "TextureComponent.h"

void dae::GemStates::GemState::Init()
{
	GetStateMachine()->GetParent()->GetComponent<CollisionRectComponent>()->m_OnEnter.AddListener([this](GameObject* other) {OnPlayerCollision(other); });
}

void dae::GemStates::GemState::OnPlayerCollision(GameObject* pOther)
{
	if (pOther->GetComponent<PlayerComponent>() == nullptr)

	//TODO: add score (currently don't have a score system)
	GetStateMachine()->SetState(1);
}

void dae::GemStates::TakenState::OnEnter()
{
	GetStateMachine()->GetParent()->GetComponent<TextureComponent>()->SetVisibility(false);
}

void dae::GemComponent::Update()
{
	m_FSM->Update();
}
