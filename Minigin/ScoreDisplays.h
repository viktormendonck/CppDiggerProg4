#pragma once
#include <memory>

#include "Component.h"

namespace dae
{
	class PlainTextComponent;
	class PlayerController;
	class ScoreDisplay final : public Component
	{
	public:
		explicit ScoreDisplay(GameObject* pParent, PlayerController* pController);
	private:
		PlainTextComponent* m_pPlainTextComponent{};
		PlayerController* m_pPlayerController{};
		
		void UpdateDisplay();
	};

	class LivesDisplay final : public Component
	{
	public:
		explicit LivesDisplay(GameObject* pParent, PlayerController* pController);
	private:
		PlainTextComponent* m_pPlainTextComponent{};
		PlayerController* m_pPlayerController{};

		void UpdateDisplay();
	};
}
