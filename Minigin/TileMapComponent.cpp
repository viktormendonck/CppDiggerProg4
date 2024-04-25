#include "TileMapComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"


namespace dae
{
	TileMapComponent::TileMapComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture,int level)
		: Component(pParent),
		m_pTexture(pTexture),
		m_TileSize(pTexture->GetSize().x / m_Size.x, pTexture->GetSize().y / m_Size.y),
		m_Level(level)
	{
		m_TileMap = m_Level1;
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
