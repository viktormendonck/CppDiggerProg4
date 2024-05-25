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
	GetParent()->GetComponent<CollisionRectComponent>()->m_OnEnter.AddListener([this](CollisionRectComponent* other) {OnPlayerInteraction(other); });
}

void dae::GemComponent::OnPlayerInteraction(CollisionRectComponent* pOther)
{
	if (pOther->GetSendingCollisionLayers() & static_cast<uint16_t>(CollisionLayers::Pickup))
	{
		m_pAnyGemPickedUpSignal->Emit(pOther->GetParent());
		GetParent()->Destroy();
	}
}
