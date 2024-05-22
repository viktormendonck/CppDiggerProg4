#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "BiMap.h"


namespace dae
{
	enum class CollisionLayers : uint16_t
	{
		Pickup = 1 << 0,
		PlayerDamage = 1 << 1,
		EnemyDamage = 1 << 2,
		Push = 1 << 3,
	};
	namespace MapData
	{
		//Most of this data should be in a json loader but hey, ig it works
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

		static BiMap<uint16_t, TileType> g_TileRules{
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
		const static std::vector<std::vector<int>> g_DigPatterns{
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

	}
}

