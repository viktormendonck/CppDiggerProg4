#pragma once
#include <memory>
#include <vector>

#include "GameObject.h"

namespace dae
{
	class FiniteStateMachine;
	class State
	{
	public:
		virtual ~State() = default;
		virtual void OnEnter(){}
		virtual void Init(){}
		virtual void Update(){}
		virtual void OnExit(){}
		void SetStateMachine(FiniteStateMachine* pStateMachine) { m_pStateMachine = pStateMachine; }
		FiniteStateMachine* GetStateMachine() const { return m_pStateMachine; }
	private:
		FiniteStateMachine* m_pStateMachine{};
	};
	
	class FiniteStateMachine
	{
	public:
		FiniteStateMachine(GameObject* pParent) : m_pParent{ pParent } {};
		void Update()
		{
			if (!m_States.empty() && m_CurrentStateIdx != -1)
			{
				m_States[m_CurrentStateIdx]->Update();
			}
		}
		void SetState(int stateIdx)
		{
			if (stateIdx < 0 || stateIdx >= static_cast<int>(m_States.size()))return; //check for validity of idx
			if(m_CurrentStateIdx != -1)
			{
				m_States[m_CurrentStateIdx]->OnExit();
			}
			m_CurrentStateIdx = stateIdx;
			m_States[m_CurrentStateIdx]->OnEnter();
		}
		int AddState(std::unique_ptr<State> pState)
		{
			m_States.push_back(std::move(pState));
			const int id = static_cast<int>(m_States.size()) - 1;
			m_States[id]->SetStateMachine(this);
			return id;
		}
		void Init()
		{
			for (auto& state : m_States)
			{
				state->Init();
			}
		}
		GameObject* GetParent() const { return m_pParent; }
	private:
		GameObject* m_pParent;
		std::vector<std::unique_ptr<State>> m_States;
		int m_CurrentStateIdx{-1};
	};
}