#include "RotationComponent.h"
#include "GameObject.h"
#include "GameTime.h"

dae::RotationComponent::RotationComponent(GameObject* pParent, float rotationSpeed)
	: Component(pParent),m_RotationSpeed(rotationSpeed)
{
}

void dae::RotationComponent::Update()
{
	//test transform
	GetParent()->GetTransform().Translate({ m_RotationSpeed * GameTime::GetInstance().GetDeltaTime(),0 });
	//GetParent()->GetTransform().Rotate(m_RotationSpeed * GameTime::GetInstance().GetDeltaTime());
}
