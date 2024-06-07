#pragma once
#include "GameObject.h"

class UITagComponent final : public dae::Component
{
public:
	UITagComponent(dae::GameObject* pParent) //this component is only to identify what gameObject is the root UI object
		: Component(pParent){}
};
