#pragma once
#include "GameObjectCommand.h"
#include "GameObject.h"

class MoveCommand : public dae::GameObjectCommand
{
public:
	MoveCommand(dae::GameObject* pGameObject, glm::vec2 dir) : GameObjectCommand(pGameObject), m_Dir{ dir }{};
private:
	glm::vec2 m_Dir;
	void Execute() override;
};

