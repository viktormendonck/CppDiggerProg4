#pragma once
#include "GameObjectCommand.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "GameTime.h"
namespace dae
{
	class PlayerController;

	class MoveCommand : public GameObjectCommand
	{
	public:
		explicit MoveCommand(GameObject* pGameObject, glm::vec2 dir) : GameObjectCommand(pGameObject), m_Dir{ dir } {};
	private:
		glm::vec2 m_Dir;
		void Execute() override
		{

			m_pGameObject->GetTransform().Translate(m_Dir * GameTime::GetInstance().GetDeltaTime());
		};
	};

	class AddScoreCommand : public GameObjectCommand
	{
	public:
		explicit AddScoreCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject), m_pPlayerController(pGameObject->GetComponent<PlayerController>()) {};
	private:
		PlayerController* m_pPlayerController;
		void Execute() override
		{
			if (m_pPlayerController)
			{
				m_pPlayerController->IncreaseScore(100);
			}
		};
	};

	class KillCommand : public GameObjectCommand
	{
	public:
		KillCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject), m_pPlayerController(pGameObject->GetComponent<PlayerController>()) {};
	private:
		PlayerController* m_pPlayerController;
		void Execute() override
		{
			if (m_pPlayerController)
			{
				m_pPlayerController->Kill();
			}
		}
	};


}
