#pragma once
#include "Component.h"
#include "FiniteStateMachine.h"
#include "TileMapComponent.h"

namespace dae
{
	class SpriteSheetComponent;

	namespace goldBagInfo
	{
		enum class GoldBagAnimations
		{
			Falling,
			Idle,
			Gold
		};
		enum class GoldBagStates
		{
			Idle,
			Wiggle,
			Falling,
			Gold,
			Taken
		};

		class IdleState final : public State
		{
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;

		private:
			SpriteSheetComponent* m_pSpriteSheet{};
			TileMapComponent* m_pTileMap{};
			GameObject* m_pGoldBag{};
		};
		class WiggleState final : public State
		{
		public:
			void OnEnter() override;
			void Update() override;
			void OnExit() override;
		private:
			const float m_MaxWiggleTime{1.5f};
			float m_CurrentWiggleTime{};
			glm::vec2 m_OriginalPos{};
		};
		class FallingState final : public State
		{
		public:
			void Init() override;
			void OnEnter() override;
			void Update() override;
			void OnExit() override;
		private:
			GameObject* m_pGoldBag{};
			SpriteSheetComponent* m_pSpriteSheet{};
			TileMapComponent* m_pTileMap{};
			glm::ivec2 m_LastTilePos{};
			const int m_MinBreakDist{ 3 };
			const float m_FallSpeed{ 50.f };
			int m_FallDist{};
		};
		class GoldState final : public State
		{
		public:
			GoldState() = default;
			void Init() override;
			void OnEnter() override;
			void OnPlayerCollision(GameObject* pOther);
		private:
			int m_Score{ 100 };
		};
		class TakenState final : public State
		{
		public:
			void OnEnter() override;
		private:

		};
	}

	class GoldBagComponent final : public Component
	{
	public:
		GoldBagComponent(GameObject* pParent);
		void Init() override;
		void Update() override;
		
	private:
		std::unique_ptr<FiniteStateMachine> m_pFSM{};


	};
}

