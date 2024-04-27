#include "TileMapComponent.h"

#include <stdexcept>

#include "GameData.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"


namespace dae
{
	TileMapComponent::TileMapComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture,glm::ivec2 tileMapSize,glm::ivec2 mapSize,int level)
		: Component(pParent),
		m_pTexture(pTexture),
		m_TileMapSize(tileMapSize),
		m_WorldSize(mapSize),
		m_TileSize(pTexture->GetSize().x / m_TileMapSize.x, pTexture->GetSize().y / m_TileMapSize.y),
		m_Level(level)
	{
		m_TileMap = MapData::m_Levels[level];
	}

	void TileMapComponent::Render() const
	{
		if (m_pTexture)
		{
			Transform& transform = GetParent()->GetTransform();
			for (size_t i{}; i < m_TileMap.size(); ++i)
			{
				const glm::ivec2 pos{ i % static_cast<int>(m_WorldSize.x), i / static_cast<int>(m_WorldSize.x) };
				const glm::vec2 posOffset{ pos.x * m_TileSize.x * transform.GetLocalScale().x, pos.y * m_TileSize.y * transform.GetLocalScale().y };
				Renderer::GetInstance().RenderSprite(*m_pTexture, GetSprite(m_TileMap[i]), transform.GetWorldPosition() + posOffset , 0, m_TileSize, transform.GetLocalScale());
			}
		}
	}

	glm::ivec2 TileMapComponent::WorldToTile(const glm::vec2& worldPos) const
	{
		const glm::vec2 localPos = worldPos - GetParent()->GetTransform().GetWorldPosition();
		return LocalToTile(localPos);
	}

	glm::ivec2 TileMapComponent::LocalToTile(const glm::vec2& localPos) const
	{
		if (localPos.x < 0 || localPos.y < 0 || localPos.x >= (m_WorldSize.x - 1) * m_TileSize.x || localPos.y >= (m_WorldSize.y - 1) * m_TileSize.y)
		{
			throw std::runtime_error("not inside of the tilemap");
		}
		const glm::ivec2 tilePos{ static_cast<int>(localPos.x / m_TileSize.x), static_cast<int>(localPos.y / m_TileSize.y) };

		return tilePos;
	}

	glm::vec2 TileMapComponent::TileToLocal(const glm::ivec2& tilePos) const
	{
		if (tilePos.x < 0 || tilePos.y < 0 || tilePos.x >= m_WorldSize.x || tilePos.y >= m_WorldSize.y)
		{
			throw std::runtime_error("not inside of the tilemap");
		}
		const glm::vec2 localPos{ tilePos.x * m_TileSize.x, tilePos.y * m_TileSize.y };

		return localPos;
	}

	glm::vec2 TileMapComponent::TileToWorld(const glm::ivec2& tilePos) const
	{
		const glm::vec2 localPos = TileToLocal(tilePos);
		const glm::vec2 worldPos = GetParent()->GetTransform().GetWorldPosition() + localPos;

		return worldPos;
	}
}
