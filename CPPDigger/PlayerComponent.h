#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "Component.h"
#include "DiggingCharacterComponent.h"
#include "FiniteStateMachine.h"
#include "Signal.h"
#include "GameObject.h"
#include "MapData.h"
#include "TileMapComponent.h"

//Player should be refactored to use its state machine more but hey, you can't have everything, there are more pressing matters
namespace dae
{
	class SpriteSheetComponent;
	class CollisionRectComponent;

	namespace playerStates
	{
		enum class PlayerState
		{
			Idle,
			Reloading,
			Dead,
		};
		class IdleState : public State
		{
			void OnEnter() override;
			void Update() override;
			void OnExit() override;
		};
		class ReloadingState : public State
		{
			//change the movement sprite on entry, wait 5 seconds, then change back to idle
		public:
			void Update() override;
			void OnExit() override;
			
		private:
			const float m_ReloadTime{ 5.f };
			float m_CurrentTime{};
		};

		class DeadState : public State
		{
			//change the sprite on entry, fall down until you hit anything, wait half a second, then respawn
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;
			void OnExit() override;
		private:
			TileMapComponent* m_pTileMap{};
			GameObject* m_pPlayer{};
			SpriteSheetComponent* m_pSpriteSheet{};
			std::vector<MapData::TileType> m_LandingTiles{
				MapData::TileType::BottomLeftCorner,
				MapData::TileType::BottomRightCorner,
				MapData::TileType::BottomWall,
				MapData::TileType::BottomMiddleRoundOff,
				MapData::TileType::BottomLeftRoundOff,
				MapData::TileType::BottomRightRoundOff
			};
			const float m_FallSpeed{ 50.f };
		};

	}

	class PlayerComponent final : public DiggingCharacterComponent
	{
	public:
		explicit PlayerComponent(dae::GameObject* pParent);

		//add needed signals
		Signal<> onDeath;

		void Init() override;
		void Update() override;

		// add funcs

		int GetLives() const { return m_Lives; }
		void SetDir(glm::ivec2 dir);
		glm::ivec2 GetDir() const { return m_CurrentDir; }
		void SetRespawnPos(glm::vec2 pos) { m_RespawnPos = pos; }
		glm::vec2 GetRespawnPos() const { return m_RespawnPos; }
		float GetSpeed() const { return m_Speed; }
		void HitWall();
		void SetReloading(bool isReloading) { m_IsReloading = isReloading; }
		bool IsReloading() const { return m_IsReloading; }
		int GetCurrentPlayerStateIdx() const { return m_pStateMachine->GetCurrentStateIdx(); }

	private:
		int m_Lives{ 3 };
		float m_Speed{ 32.f };

		std::unique_ptr<FiniteStateMachine> m_pStateMachine;
		glm::ivec2 m_CurrentDir{};

		glm::vec2 m_TargetPos{ 0,0 };
		glm::vec2 m_RespawnPos{ 0,0 };
		bool m_CanMove{ true };

		bool m_IsReloading{ false };
		void OnCollision(const CollisionRectComponent* pOther);
		void StartRespawn();

		TileMapComponent* GetTileMap() const
		{
			return GetParent()->GetParent()->GetComponent<TileMapComponent>();
		}
	};
}
