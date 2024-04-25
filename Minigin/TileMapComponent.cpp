#include "TileMapComponent.h"

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
		m_TileMap = GameData::GetInstance().GetLevel(level);
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
}
