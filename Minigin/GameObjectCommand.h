#pragma once
#include "Command.h"
#include "GameObject.h"

namespace dae
{
	class GameObjectCommand : public Command
	{
	public:
		explicit  GameObjectCommand(GameObject* pGameObject) : m_pGameObject{ pGameObject } {};
		virtual ~GameObjectCommand() = default;

		GameObjectCommand(const GameObjectCommand& other) = delete;
		GameObjectCommand(GameObjectCommand&& other) = delete;
		GameObjectCommand& operator=(const GameObjectCommand& other) = delete;
		GameObjectCommand& operator=(GameObjectCommand&& other) = delete;

		virtual void Execute() = 0;
	protected:
		GameObject* m_pGameObject;
	};
}