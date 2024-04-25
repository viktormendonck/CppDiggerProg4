#include "RotationComponent.h"
#include "GameObject.h"
#include "GameData.h"
#include <iostream>

dae::RotationComponent::RotationComponent(GameObject* pParent, float rotationSpeed)
	: Component(pParent),m_RotationSpeed(rotationSpeed)
{
}

void dae::RotationComponent::Update()
{
	//test transform
	GetParent()->GetTransform().Rotate(m_RotationSpeed * GameData::GetInstance().GetDeltaTime());
}
