#pragma once
#pragma once
#include <SDL_syswm.h>

#include "CollisionRectComponent.h"
#include "DiggingCharacterComponent.h"
#include "FiniteStateMachine.h"
#include "SpriteSheetComponent.h"
#include "TileMapComponent.h"


namespace dae
{
	namespace enemyStates
	{
		enum class EnemyState
		{
			Normal,
			Angered
		};
		class NormalState final : public State
		{
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;
		private:
			TileMapComponent* m_pTileMap{};
			SpriteSheetComponent* m_pSpriteSheet{};
			glm::ivec2 m_PreviousTile{-1,-1};
			glm::ivec2 m_CurrentTile{};

			glm::vec2 m_Dir{};

			float m_Speed = 30.f;

			void GetNextDir();
		};
		class AngeredState final : public State
		{
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;
			void OnExit() override;
		private:
			SpriteSheetComponent* m_pSpriteSheet{};
			TileMapComponent* m_pTileMap{};
			std::vector<GameObject*> m_pPlayers{};
		};
	}
	class EnemyComponent final : public DiggingCharacterComponent
	{
	public:
		EnemyComponent(GameObject* pParent,std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal);;

		void Init() override;
		void Update() override;

	private:
		std::unique_ptr<FiniteStateMachine> m_pStateMachine;
		std::shared_ptr<Signal<GameObject*>> m_pAnyEnemyKilledSignal{};

		void OnPlayerDeath();
		void OnCollision(CollisionRectComponent* pOther);
	};
}
