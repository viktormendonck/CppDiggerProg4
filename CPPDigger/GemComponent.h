#pragma once
#include <Component.h>

#include "FiniteStateMachine.h"

namespace dae
{
	class CollisionRectComponent;

	namespace GemStates
	{
		class GemState final : public State
		{
		public:
			GemState() = default;
			void Init();
			void OnEnter() override{}
			void OnPlayerCollision(GameObject* pOther);
		private:
			int m_Score{100};
		};
		class TakenState final: public State
		{
		public:
			TakenState() = default;
		private:
			void OnEnter() override;
		};
	}
	class GemComponent final:  public Component
	{
	public:
		GemComponent(GameObject* pParent)
			: Component(pParent),
			m_FSM(std::make_unique<FiniteStateMachine>(pParent))
		{
		}
		void Init() override;
		void Update() override;
	private:
		std::unique_ptr<FiniteStateMachine> m_FSM;
	};

	inline void GemComponent::Init()
	{
		std::unique_ptr<GemStates::GemState> gemState = std::make_unique<GemStates::GemState>();
		m_FSM->AddState(std::move(gemState));
		std::unique_ptr<GemStates::TakenState> takenState = std::make_unique<GemStates::TakenState>();
		m_FSM->AddState(std::move(takenState));
		m_FSM->SetState(0);
		m_FSM->Init();
	}
}

