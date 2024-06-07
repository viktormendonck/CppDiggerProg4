#pragma once
#include "CollisionRectComponent.h"
#include "DiggingCharacterComponent.h"
#include "FiniteStateMachine.h"
#include "SpriteSheetComponent.h"
#include "TileMapComponent.h"


namespace dae
{
	class EnemyComponent;

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

			glm::vec2 m_CurrentCheckingPos{};
			float m_Speed = 30.f;

			bool m_ShouldCheckTile{false};
			const float m_DecisionTime{0.1f};
			float m_CurrentDecisionTime{};

			const float m_MaxAngerTime{ 25.f };
			float m_TimeBeforeAngered{25.f};

			void GetNextDir();
		};
		class AngeredState final : public State
		{
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;
		private:
			SpriteSheetComponent* m_pSpriteSheet{};
			TileMapComponent* m_pTileMap{};
			EnemyComponent* m_pEnemyComponent{};
			glm::ivec2 m_PreviousTile{ -1,-1 };
			glm::ivec2 m_CurrentTile{};

			glm::vec2 m_CurrentCheckingPos{};
			float m_Speed = 15.f;
			glm::vec2 m_MoveDir{};

			bool m_ShouldCheckTile{ false };
			const float m_DecisionTime{ 0.1f };
			float m_CurrentDecisionTime{};

			const float m_MaxCalmTime{ 10.f };
			float m_TimeBeforeCalmed{ 10.f };

			void GetNextDir();
		};
	}
	class EnemyComponent final : public DiggingCharacterComponent
	{
	public:
		friend class enemyStates::AngeredState;
		friend class enemyStates::NormalState;
		EnemyComponent(GameObject* pParent,std::shared_ptr<Signal<GameObject*>> pAnyEnemyKilledSignal, Signal<>* onPLayerDeath);

		void Init() override;
		void Update() override;
		void Render() const override;
	protected:
		void Dig(glm::vec2 dir);
		glm::vec2 GetClosestPlayerPos() const;
	private:
		std::unique_ptr<FiniteStateMachine> m_pStateMachine;
		std::shared_ptr<Signal<GameObject*>> m_pAnyEnemyKilledSignal{};

		void OnPlayerDeath();
		void OnCollision(CollisionRectComponent* pOther);
		size_t m_ListenerId{};
		Signal<>* m_pOnPlayerDeath{}; // only used to stop listening to the player death signal
	};
}
