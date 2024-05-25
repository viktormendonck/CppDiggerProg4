#pragma once
#include "Component.h"
#include "FiniteStateMachine.h"
#include "Signal.h"
#include "TileMapComponent.h"

namespace dae
{
	class CollisionRectComponent;
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
			SpriteSheetComponent* m_pSpriteSheet{};
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
			const int m_MinBreakDist{ 6 };
			const float m_FallSpeed{ 50.f };
			int m_FallDist{};
		};
		class GoldState final : public State
		{
		public:
			explicit GoldState(std::shared_ptr<Signal<GameObject*>> pAnyGoldPickedUpSignal)
				: pAnyGoldPickedUpSignal(std::move(pAnyGoldPickedUpSignal))
			{}
			void Init() override;
			void OnEnter() override;
			void OnPlayerCollision(CollisionRectComponent* pOther);
		private:
			std::shared_ptr<Signal<GameObject*>> pAnyGoldPickedUpSignal{};
		};
	}

	class GoldBagComponent final : public Component
	{
	public:
		GoldBagComponent(GameObject* pParent,std::shared_ptr<Signal<GameObject*>> pAnyGoldPickedUpSignal);
		void Init() override;
		void Update() override;
		
	private:
		std::unique_ptr<FiniteStateMachine> m_pFSM{};


	};
}

