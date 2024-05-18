#pragma once
#include "GameObjectCommand.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "SpriteSheetComponent.h"

namespace dae
{
	class PlayerComponent;

	class MoveCommand : public GameObjectCommand
	{
	public:
		explicit MoveCommand(GameObject* pGameObject, glm::ivec2 dir) : GameObjectCommand(pGameObject), m_Dir{ dir } {};
	private:
		glm::vec2 m_Dir;
		void Execute() override
		{
			m_pGameObject->GetComponent<SpriteSheetComponent>()->SetSprite({ 0,0 });
			if (abs(m_Dir.x) > glm::epsilon<float>())
			{
				m_pGameObject->GetTransform().SetLocalRotation(0);
				if (m_Dir.x < 0)
				{
					m_pGameObject->GetComponent<SpriteSheetComponent>()->SetSprite({ 0,1 });
				}
			}
			else
			{
				m_pGameObject->GetTransform().SetLocalRotation(glm::pi<float>()/ 2.f * m_Dir.y);
			}
			m_pGameObject->GetComponent<PlayerComponent>()->SetDir(m_Dir);
		}
	};

	class AddScoreCommand : public GameObjectCommand
	{
	public:
		explicit AddScoreCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject), m_pPlayerController(pGameObject->GetComponent<PlayerComponent>()) {};
	private:
		PlayerComponent* m_pPlayerController;
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
		KillCommand(GameObject* pGameObject) : GameObjectCommand(pGameObject), m_pPlayerController(pGameObject->GetComponent<PlayerComponent>()) {};
	private:
		PlayerComponent* m_pPlayerController;
		void Execute() override
		{
			auto& soundSystem = ServiceLocator::GetSoundSystem();
			soundSystem.PlaySound(0, 0.5f, SoundSystem::SoundType::SoundEffect);
		}
	};


}
