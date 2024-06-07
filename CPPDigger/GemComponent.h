#pragma once
#include <Component.h>

#include "CollisionRectComponent.h"
#include "FiniteStateMachine.h"
#include "Signal.h"

namespace dae
{
	class GemComponent final:  public Component
	{
	public:
		GemComponent(GameObject* pParent, std::shared_ptr<Signal<GameObject*>> pAnyGemPickedUpSignal);
		void Init() override;
	private:
		void OnPlayerInteraction(CollisionRectComponent* pOther);
		std::shared_ptr<Signal<GameObject*>> m_pAnyGemPickedUpSignal{};
		bool m_IsPickedUp{ false };
	};

}

