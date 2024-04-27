#pragma once
#include <memory>

#include "Component.h"

namespace dae
{
	class PlainTextComponent;
	class PlayerComponent;
	class ScoreDisplay final : public Component
	{
	public:
		explicit ScoreDisplay(GameObject* pParent, PlayerComponent* pController);
	private:
		PlainTextComponent* m_pPlainTextComponent{};
		PlayerComponent* m_pPlayerController{};
		
		void UpdateDisplay();
	};

	class LivesDisplay final : public Component
	{
	public:
		explicit LivesDisplay(GameObject* pParent, PlayerComponent* pController);
	private:
		PlainTextComponent* m_pPlainTextComponent{};
		PlayerComponent* m_pPlayerController{};

		void UpdateDisplay();
	};
}
