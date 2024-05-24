#pragma once
#include <Component.h>

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
		void OnPlayerInteraction(GameObject* pOther);
		std::shared_ptr<Signal<GameObject*>> m_pAnyGemPickedUpSignal{};
	};

}

