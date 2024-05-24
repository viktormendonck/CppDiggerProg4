#include "GemComponent.h"

#include "CollisionRectComponent.h"
#include "PlayerComponent.h"
#include "TextureComponent.h"


dae::GemComponent::GemComponent(GameObject* pParent,std::shared_ptr<Signal<GameObject*>> pAnyGemPickedUpSignal)
	: Component(pParent),
m_pAnyGemPickedUpSignal{std::move(pAnyGemPickedUpSignal)}
{
}

void dae::GemComponent::Init()
{
	GetParent()->GetComponent<CollisionRectComponent>()->m_OnEnter.AddListener([this](GameObject* other) {OnPlayerInteraction(other); });
}

void dae::GemComponent::OnPlayerInteraction(GameObject* pOther)
{
	if (pOther->GetComponent<PlayerComponent>() == nullptr) return;
	m_pAnyGemPickedUpSignal->Emit(pOther);
	GetParent()->Destroy();
}
