#pragma once
#include "GameObjectCommand.h"
#include "LevelManagerComponent.h"
namespace dae
{

	class SkipLevelCommand final : public GameObjectCommand
	{
	public:
		explicit SkipLevelCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject) {};
	private:
		void Execute() override
		{
			m_pGameObject->GetComponent<LevelManagerComponent>()->NextLevel();
		}
	};
}
