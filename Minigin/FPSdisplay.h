#pragma once
#include "Component.h"
namespace dae
{
	struct MissingComponentException {};

	class PlainTextComponent;
	class FPSDisplay final : public Component
	{
	public:
		FPSDisplay(GameObject* pParent, float updateTime);
		void Update() override;
	private:
		PlainTextComponent* m_pPlainTextComponent{};

		const float m_UpdateTime{};
		int m_DeltasBetweenUpdates{};
		float m_AccumulatedDeltas{};
	};
}