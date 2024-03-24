#pragma once
#include "Component.h"
#include "Signal.h"

namespace dae
{
	class PlayerController final : public Component
	{
	public:
		explicit PlayerController(dae::GameObject* pParent) : Component(pParent){}

		//add needed signals
		Signal<> OnDeath;
		Signal<> OnScoreChanged;

		// add funcs
		int GetScore() const { return m_Score; }
		void IncreaseScore(int amount) { m_Score += amount; OnScoreChanged.Emit(); }

		int GetLives() const { return m_Lives; }
		void Kill() { --m_Lives; OnDeath.Emit(); }
	private:
		int m_Score{};
		int m_Lives{ 3 };
	};
}
