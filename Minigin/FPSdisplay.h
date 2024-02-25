#pragma once
#include "PlainTextComponent.h"

namespace dae
{
	class FPSDisplay final : public PlainTextComponent
	{
	public:
		FPSDisplay(GameObject* pParent, std::shared_ptr<Font> font, SDL_Color color, float updateTime);
		void Update() override;
	private:
		float m_updateTime{ };
		float m_deltasBetweenUpdates{};
		float m_accumulatedDeltas{};
	};
}