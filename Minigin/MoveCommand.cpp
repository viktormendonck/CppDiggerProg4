#include "MoveCommand.h"
#include "GameTime.h"

void MoveCommand::Execute()
{
	glm::vec2 pos = {m_Dir.x * dae::GameTime::GetInstance().GetDeltaTime(),m_Dir.y * dae::GameTime::GetInstance().GetDeltaTime() };
	m_pGameObject->GetTransform().Translate(pos);
}
