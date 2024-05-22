#include "PlayerComponent.h"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/vec2.hpp>
#include "GameData.h"
#include "GemComponent.h"
#include "MapData.h"
#include "SpriteSheetComponent.h"
#include "TileMapComponent.h"


namespace dae
{
	PlayerComponent::PlayerComponent(dae::GameObject* pParent)
		: DiggingCharacterComponent(pParent)
	{
	}


	void PlayerComponent::Update()
	{
		if (m_CanChangeDir ==false)
		{
			const glm::vec2 pos = GetParent()->GetTransform().GetLocalPosition();
			const glm::vec2 dir = glm::normalize(glm::vec2{ m_TargetPos.x - pos.x,m_TargetPos.y - pos.y });
			GetParent()->GetTransform().SetLocalPosition(GetParent()->GetTransform().GetLocalPosition() + dir * m_Speed * GameData::GetInstance().GetDeltaTime());
			if (glm::distance(pos, m_TargetPos) < 0.1f)
			{
				GetParent()->GetTransform().SetLocalPosition(m_TargetPos);
				m_CanChangeDir = true;
			}
		}
	}


	void PlayerComponent::SetDir(glm::ivec2 dir)
	{
		TileMapComponent* tileMap = GetTileMap();
		dir = glm::normalize(glm::vec2(dir.x,dir.y));
		const glm::vec2 currentTile = tileMap->LocalToTile(GetParent()->GetTransform().GetLocalPosition());
		glm::ivec2 goalTile = dir + glm::ivec2(static_cast<int>(currentTile.x), static_cast<int>(currentTile.y));
		m_TargetPos = tileMap->TileToLocal(goalTile);
		m_CanChangeDir = false;
		m_CurrentDir = dir;
		Dig(dir);
	}
}
