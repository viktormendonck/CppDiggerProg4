#include "PlayerComponent.h"

#include <glm/ext/quaternion_geometric.hpp>

#include "GameData.h"

using dae::MapData::TileType;

namespace dae
{
	PlayerComponent::PlayerComponent(dae::GameObject* pParent, int level)
		: Component(pParent),
		m_Level(level),
		m_StartPos(MapData::m_LevelStartPositions[level])
	{
	}

	void PlayerComponent::Init()
	{
		GetParent()->GetTransform().SetLocalPosition(GetTileMap()->TileToLocal(m_StartPos));
	}

	void PlayerComponent::Update()
	{
		if (m_CanChangeDir ==false)
		{
			if (m_CurrentDigTime > m_DigTimer)
			{
				Dig();
				m_CurrentDigTime = 0;
			}
			else
			{
				m_CurrentDigTime += GameData::GetInstance().GetDeltaTime();
			}
			glm::vec2 pos = GetParent()->GetTransform().GetLocalPosition();
			glm::vec2 dir = glm::normalize(glm::vec2{ m_TargetPos.x - pos.x,m_TargetPos.y - pos.y });
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
		const glm::ivec2 goalTile = tileMap->LocalToTile(GetParent()->GetTransform().GetLocalPosition()) + glm::ivec2(dir.x, dir.y);
		m_TargetPos = tileMap->TileToLocal(goalTile);
		m_CanChangeDir = false;
		if (dir == glm::ivec2{ 0,-1 })
		{
			m_CurrentDir = 0;
		}
		else if (dir == glm::ivec2{ 1,0 })
		{
			m_CurrentDir = 1;
		}
		else if (dir == glm::ivec2{ 0,1 })
		{
			m_CurrentDir = 2;
		}
		else if (dir == glm::ivec2{ -1,0 })
		{
			m_CurrentDir = 3;
		}
	}

	void PlayerComponent::Dig()
	{
		if (m_CurrentDir == -1) return;
		int dir = m_CurrentDir;
		TileMapComponent* pTileMap = GetTileMap();
		for (int yIdx{ -1 }; yIdx <= 1; ++yIdx)
		{
			for (int xIdx{ -1 }; xIdx <= 1; ++xIdx)
			{
				const glm::ivec2 currentTile = pTileMap->LocalToTile(GetParent()->GetTransform().GetLocalPosition());
				
				const int idx = (currentTile.y + yIdx) * pTileMap->GetWorldSize().x + currentTile.x + xIdx;
				const int templateIdx = (yIdx + 1) * 3 + (xIdx + 1);
				const int newTile = MapData::m_DigPatterns[dir][templateIdx];
				const int oldTile = pTileMap->GetTileSprite(idx);
				if (newTile == -1 || oldTile == 10) continue;
				if (oldTile == 0)
				{
					pTileMap->SetTileSprite(idx, MapData::m_DigPatterns[dir][templateIdx]);

				}
				else
				{
					
					if (newTile == 10)
					{
						pTileMap->SetTileSprite(idx, 10);
						continue;
					}
					if (newTile == oldTile)
					{
						continue;
					}

					pTileMap->SetTileSprite(idx, MapData::CompareTiles(static_cast<TileType>(newTile), static_cast<TileType>(oldTile)));
				}
			}
		}
		m_CurrentDir = -1;
	}
	

}
