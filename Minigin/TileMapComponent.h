#pragma once
#include "Component.h"
#pragma once
#include <string>
#include <glm/vec2.hpp>
#include "Component.h"
#include "memory"
#include "unordered_map"
namespace dae
{
	class Texture2D;
	class TileMapComponent final : public Component
	{
	public:
		enum class Tiles
		{
			Full,					//0
			LeftVertWall,			//1
			RightVertWal,			//2
			TopEndLeft,				//3
			TopEndMid,				//4
			TopEndRight,			//5
			BottomRightCorner,		//6
			BottomLeftCorner,		//7
			TopHorWall,				//8
			SideEndLeft,			//9
			Empty,					//10
			SideEndRight,			//11
			TopRightCorner,			//12
			TopLeftCorner,			//13
			BottomHorWall,			//14
			BottomEndLeft,			//15
			BottomEndMid,			//16
			BottomEndRight,			//17
			SideTopEndWallLeft,		//18
			SideTopEndWallRight,	//19
			SideTopContinuationLeft,	//20
			SideTopContinuationRight,	//21
			SideBottomContinuationLeft,	//22
			SideBottomContinuationRight,//23
			TopContinuationLeft,		//24
			TopContinuationRight,		//25
			BottomContinuationLeft,		//26
			BottomContinuationRight,	//27

		};
#pragma region class
		explicit TileMapComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture, int level);

		void Render() const override;
		void SetTileSprite(glm::ivec2 tileId, Tiles tile) { m_TileMap[GetTileIdx(tileId)] = static_cast<int>(tile); }
		
	protected:
		const std::shared_ptr<Texture2D> m_pTexture;
		glm::ivec2 m_Size{ 6,5 };
		glm::ivec2 m_WorldSize{ 40,25 };
		glm::ivec2 m_TileSize;
		int m_Level;

		std::vector<int> m_TileMap{m_WorldSize.x*m_WorldSize.y};

		glm::ivec2 GetSprite(Tiles tile) const {return glm::ivec2{ static_cast<int>(tile) % m_Size.x ,static_cast<int>(tile) / m_Size.x }; }
		glm::ivec2 GetSprite(int tile) const {return glm::ivec2{ tile % m_Size.x ,tile / m_Size.x }; }

		int GetTileIdx(glm::ivec2 tileId) const { return tileId.y * m_WorldSize.x + tileId.x; }
		glm::ivec2 GetTilePos(int idx) const { return glm::ivec2{ idx % static_cast<int>(m_WorldSize.x), idx / static_cast<int>(m_WorldSize.x) }; }
#pragma endregion
		const std::vector<int> m_Level1
		{
			 0, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 9,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,22, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,24, 4, 5, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,20,10,10,14,14,14,14,14,14,14,14,14,14,14,26,16,17, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 5, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,22, 8, 0, 8,23, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10, 0,10,11, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,14, 0,14,17, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10,12, 8, 8,23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0,
			 0, 9,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,16,17, 0, 0, 0,
			 0,15,14,14,10,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 1,10,12, 8, 8, 8, 8,23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 9,10,10,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0,15,14,14,14,14,10,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10,12, 8, 8, 8, 8, 8, 8, 8, 8, 8,13,10,19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10,10,10,10,10,10,10,10,10,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0,15,14,14,14,14,14,14,14,14,14,14,14,14,14,17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};
	};
}

