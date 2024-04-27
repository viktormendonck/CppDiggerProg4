#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include "memory"
#include "vector"
namespace dae
{
	class Texture2D;
	class TileMapComponent final : public Component
	{
	public:
		explicit TileMapComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture, glm::ivec2 tileMapSize, glm::ivec2 mapSize, int level);

		void Render() const override;
		void SetTileSprite(glm::ivec2 tileId, int tile) { m_TileMap[GetTileIdx(tileId)] = tile; }
		void SetTileSprite(int idx, int tile) { m_TileMap[idx] = tile; }

		int GetTileSprite(glm::ivec2 tileId) const { return m_TileMap[GetTileIdx(tileId)]; }
		int GetTileSprite(int idx) const { return m_TileMap[idx]; }
		
		glm::ivec2 GetTileMapSize() const { return m_TileMapSize; }
		glm::ivec2 WorldToTile(const glm::vec2& worldPos) const;
		glm::ivec2 LocalToTile(const glm::vec2& localPos) const;
		glm::vec2 TileToLocal(const glm::ivec2& tilePos) const;
		glm::vec2 TileToWorld(const glm::ivec2& tilePos) const;
	protected:
		//temporary test variables
		float m_TimeBetweenDigs{ 0.5f };
		float m_CurrentTime{};
		glm::ivec2 m_CurrentTile{ 15,5 };

		const std::shared_ptr<Texture2D> m_pTexture;
		glm::ivec2 m_TileMapSize{ 6,5 };
		glm::ivec2 m_WorldSize{ 40,25 };
		glm::ivec2 m_TileSize;
		int m_Level;

		std::vector<int> m_TileMap{m_WorldSize.x*m_WorldSize.y};

		glm::ivec2 GetSprite(int tile) const {return glm::ivec2{ tile % m_TileMapSize.x ,tile / m_TileMapSize.x }; }

		int GetTileIdx(glm::ivec2 tileId) const { return tileId.y * m_WorldSize.x + tileId.x; }
		glm::ivec2 GetTilePos(int idx) const { return glm::ivec2{ idx % static_cast<int>(m_WorldSize.x), idx / static_cast<int>(m_WorldSize.x) }; }
	};
}

