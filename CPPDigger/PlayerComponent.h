#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "Component.h"
#include "DiggingCharacterComponent.h"
#include "Signal.h"
#include "GameObject.h"
#include "TileMapComponent.h"

namespace dae
{
	class PlayerComponent final : public DiggingCharacterComponent
	{
	public:
		explicit PlayerComponent(dae::GameObject* pParent);

		//add needed signals
		Signal<> OnDeath;
		Signal<> OnScoreChanged;

		// add funcs
		int GetScore() const { return m_Score; }
		void IncreaseScore(int amount) { m_Score += amount; OnScoreChanged.Emit(); }

		int GetLives() const { return m_Lives; }
		void Kill() { --m_Lives; OnDeath.Emit(); }

		void Update() override;
		void SetDir(glm::ivec2 dir);
		glm::ivec2 GetDir() const { return m_CurrentDir; }

	private:
		int m_Score{};
		int m_Lives{ 3 };
		float m_Speed{ 32.f };

		glm::ivec2 m_CurrentDir{};

		glm::vec2 m_TargetPos{ 0,0 };
		bool m_CanChangeDir{ true };
		TileMapComponent* GetTileMap() const
		{
			return GetParent()->GetParent()->GetComponent<TileMapComponent>();
		}
		
	};
}
