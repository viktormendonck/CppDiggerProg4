#pragma once
#include "Component.h"
#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "Component.h"
#include "GameData.h"
#include "memory"
#include "unordered_map"
namespace dae
{
	class Texture2D;
	class TileMapComponent final : public Component
	{
	public:
		
#pragma region class
		explicit TileMapComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture, glm::ivec2 tileMapSize, glm::ivec2 mapSize, int level);

		void Render() const override;
		void SetTileSprite(glm::ivec2 tileId, GameData::Tiles tile) { m_TileMap[GetTileIdx(tileId)] = static_cast<int>(tile); }
		
	protected:
		const std::shared_ptr<Texture2D> m_pTexture;
		glm::ivec2 m_TileMapSize{ 6,5 };
		glm::ivec2 m_WorldSize{ 40,25 };
		glm::ivec2 m_TileSize;
		int m_Level;

		std::vector<int> m_TileMap{m_WorldSize.x*m_WorldSize.y};

		glm::ivec2 GetSprite(GameData::Tiles tile) const {return glm::ivec2{ static_cast<int>(tile) % m_TileMapSize.x ,static_cast<int>(tile) / m_TileMapSize.x }; }
		glm::ivec2 GetSprite(int tile) const {return glm::ivec2{ tile % m_TileMapSize.x ,tile / m_TileMapSize.x }; }

		int GetTileIdx(glm::ivec2 tileId) const { return tileId.y * m_WorldSize.x + tileId.x; }
		glm::ivec2 GetTilePos(int idx) const { return glm::ivec2{ idx % static_cast<int>(m_WorldSize.x), idx / static_cast<int>(m_WorldSize.x) }; }
#pragma endregion
		
	};
}

