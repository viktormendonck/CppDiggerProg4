#pragma once
#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include "BiMap.h"
#include "Singleton.h"

namespace dae
{
	class GameData : public Singleton<GameData>
	{
	public:
		

		//time
		float GetDeltaTime() const { return m_DeltaTime; }
		void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
		static constexpr float FIXED_TIMESTEP = 1.f / 50.f;
		//tilemap

	private:


		float m_DeltaTime = 1.f/60.f;

		
	};
	namespace MapData
	{
		inline uint16_t boolVecToUInt16(const std::vector<bool>& vec) {
			uint16_t result = 0;
			for (size_t i{}; i < vec.size(); ++i) {
				result |= vec[i] << i;
			}
			return result;
		}
		enum class TileType
		{
			Full,
			LeftWall,
			RightWall,
			TopLeftRoundOff,
			TopMiddleRoundOff,
			TopRightRoundOff,
			BottomRightCorner,
			BottomLeftCorner,
			TopWall,
			MiddleLeftRoundOff,
			Empty,
			MiddleRightRoundOff,
			TopRightCorner,
			TopLeftCorner,
			BottomWall,
			BottomLeftRoundOff,
			BottomMiddleRoundOff,
			BottomRightRoundOff,
		};

		uint16_t GetTileRule(TileType type);

		TileType GetTileType(uint16_t tile);

		TileType CompareTiles(TileType newTile, TileType oldTile);
		static BiMap<uint16_t, TileType> TileRules{
		{
			{boolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					1,1,1,1,
					1,1,1,1
				}),TileType::Full},
			{boolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				}),TileType::Empty},
			{boolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					1,1,0,0,
					1,1,0,0
				}),TileType::LeftWall},
			{boolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					0,0,0,0,
					0,0,0,0
				}),TileType::TopWall},
			{boolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					1,1,1,1,
					1,1,1,1,
				}),TileType::BottomWall},
			{boolVecToUInt16(
				{
					0,0,1,1,
					0,0,1,1,
					0,0,1,1,
					0,0,1,1
				}),TileType::RightWall},
			{boolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					1,1,0,0,
					1,1,0,0
				}),TileType::TopLeftRoundOff},
			{boolVecToUInt16(
				{
					1,1,1,1,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				}),TileType::TopMiddleRoundOff},
			{boolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					0,0,1,1,
					0,0,1,1
				}),TileType::TopRightRoundOff},
			{boolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,1,1,
					0,0,1,1
				}),TileType::BottomRightCorner},
			{boolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					1,1,0,0,
					1,1,0,0
				}),TileType::BottomLeftCorner},
			{boolVecToUInt16(
				{
					0,0,1,1,
					0,0,1,1,
					0,0,0,0,
					0,0,0,0,
				}),TileType::TopRightCorner},
			{boolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					0,0,0,0,
					0,0,0,0,
				}),TileType::TopLeftCorner},
			{boolVecToUInt16(
				{
					1,0,0,0,
					1,0,0,0,
					1,0,0,0,
					1,0,0,0
				}),TileType::MiddleLeftRoundOff},
			{boolVecToUInt16(
				{
					0,0,0,1,
					0,0,0,1,
					0,0,0,1,
					0,0,0,1
				}),TileType::MiddleRightRoundOff},
			{boolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					1,1,1,1,
					1,1,1,1
				}),TileType::BottomLeftRoundOff},
			{boolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					1,1,1,1
				}),TileType::BottomMiddleRoundOff},
			{ boolVecToUInt16(
				{
					0,0,1,1,
					0,0,1,1,
					1,1,1,1,
					1,1,1,1
				}),TileType::BottomRightRoundOff },
			}
		};
		const static std::map<TileType, uint16_t> reversedTileRules{
	{TileType::Full, boolVecToUInt16(
		{
			1,1,1,1,
			1,1,1,1,
			1,1,1,1,
			1,1,1,1,
		})},
	{TileType::Empty, boolVecToUInt16(
		{
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
		})},
	{TileType::LeftWall, boolVecToUInt16(
		{
			1,1,0,0,
			1,1,0,0,
			1,1,0,0,
			1,1,0,0,
		})},
	{TileType::RightWall, boolVecToUInt16(
		{
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
		})},
		{TileType::TopWall, boolVecToUInt16(
		{
			1,1,1,1,
			1,1,1,1,
			0,0,0,0,
			0,0,0,0,
		})},
		{TileType::BottomWall, boolVecToUInt16(
		{
			0,0,0,0,
			0,0,0,0,
			1,1,1,1,
			1,1,1,1,
		})},
	{TileType::TopLeftRoundOff, boolVecToUInt16(
		{
			1,1,1,1,
			1,1,1,1,
			1,1,0,0,
			1,1,0,0,
		})},
	{TileType::TopMiddleRoundOff, boolVecToUInt16(
		{
			1,1,1,1,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
		})},
	{TileType::TopRightRoundOff, boolVecToUInt16(
		{
			1,1,1,1,
			1,1,1,1,
			0,0,1,1,
			0,0,1,1,
		})},
	{TileType::BottomRightCorner, boolVecToUInt16(
		{
			0,0,0,0,
			0,0,0,0,
			0,0,1,1,
			0,0,1,1,
		})},
	{TileType::BottomLeftCorner, boolVecToUInt16(
		{
			0,0,0,0,
			0,0,0,0,
			1,1,0,0,
			1,1,0,0,
		})},
	{TileType::TopRightCorner, boolVecToUInt16(
		{
			0,0,1,1,
			0,0,1,1,
			0,0,0,0,
			0,0,0,0,
		})},
	{TileType::TopLeftCorner, boolVecToUInt16(
		{
			1,1,0,0,
			1,1,0,0,
			0,0,0,0,
			0,0,0,0,
		})},
	{TileType::MiddleLeftRoundOff, boolVecToUInt16(
		{
			1,0,0,0,
			1,0,0,0,
			1,0,0,0,
			1,0,0,0,
		})},
	{TileType::MiddleRightRoundOff, boolVecToUInt16(
		{
			0,0,0,1,
			0,0,0,1,
			0,0,0,1,
			0,0,0,1,
		})},
	{TileType::BottomLeftRoundOff, boolVecToUInt16(
		{
			1,1,0,0,
			1,1,0,0,
			1,1,1,1,
			1,1,1,1,
		})},
	{TileType::BottomMiddleRoundOff, boolVecToUInt16(
		{
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			1,1,1,1,
		})},
	{TileType::BottomRightRoundOff, boolVecToUInt16(
		{
			0,0,1,1,
			0,0,1,1,
			1,1,1,1,
			1,1,1,1,
		})}
		};




		const static std::vector<std::vector<int>> m_DigPatterns{
			{ //up
				 3, 4, 5,
				 1,10, 2,
				-1,-1,-1,
			},
			{ //right
				-1, 8, 5,
				-1,10,11,
				-1,14,17,
			},
			{ //down
				-1,-1,-1,
				 1,10, 2,
				15,16,17
			},
			{ //left
				 3, 8,-1,
				 9,10,-1,
				15,14,-1
			}
		};
		const static std::vector<glm::ivec2> m_LevelStartPositions{
			{ 19, 22 },
			{ 1, 1 },
			{ 1, 1 },
			{ 1, 1 }
		};

		const static std::vector<std::vector<int>> m_Levels
		{
			{
			 0, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 9,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 5, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10,10,10,10,10,10,10,10,10,10,11, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10,10,14,14,14,14,14,14,14,16,17, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 1,10,12, 8, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 9,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0,15,14,14,10,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 1,10,12, 8, 8, 8, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 9,10,10,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0,15,14,14,14,14,10,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,10,12, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,13,10, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0,15,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			},
		};

		
	}
}



























