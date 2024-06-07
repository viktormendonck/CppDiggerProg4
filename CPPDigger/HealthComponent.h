#pragma once
#include <memory>

#include "Component.h"
#include "Signal.h"

namespace dae
{
	//i know the health should be kept here but that would take too much refactoring so this will be the current system
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* pParent, std::unique_ptr<Signal<int>> pLivesChangedSignal);
		std::unique_ptr<Signal<int>> livesChangedSignal;
		void Init() override;
	private:
		void OnPlayerDeath();
	};

}
