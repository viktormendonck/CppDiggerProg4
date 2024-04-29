#pragma once
#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include <unordered_map>
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
		//index of tilerules should be a static_cast<int>(TileType)
		const static std::map<uint16_t, TileType> TileRules{
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

		inline size_t PairHashFunction (const TileType x, const TileType y) {
			const auto h1 = std::hash<int>{}(static_cast<int>(x));
			const auto h2 = std::hash<int>{}(static_cast<int>(y));
			// Very simple hasher that is in no way optimal
			return h1 ^ h2;
		}
		;
		//const static std::unordered_multimap<size_t, TileType> TileDecisionMap{
		//	{PairHashFunction(TileType::LeftWall,TileType::TopWall),TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::LeftWall,TileType::TopRightRoundOff),TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::LeftWall,TileType::BottomWall),TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::LeftWall,TileType::BottomRightRoundOff),TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::LeftWall,TileType::TopMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::BottomMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::MiddleRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::RightWall),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::BottomRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::TopRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::LeftWall,TileType::TopLeftRoundOff),TileType::LeftWall},
		//	{PairHashFunction(TileType::LeftWall,TileType::BottomLeftRoundOff),TileType::LeftWall},
		//	{PairHashFunction(TileType::RightWall,TileType::TopWall),TileType::TopRightCorner},
		//	{PairHashFunction(TileType::RightWall,TileType::TopLeftRoundOff),TileType::TopRightCorner},
		//	{PairHashFunction(TileType::RightWall,TileType::BottomWall),TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::RightWall,TileType::BottomLeftRoundOff),TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::RightWall,TileType::TopMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::BottomMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::MiddleLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::LeftWall),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::BottomLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::TopLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::RightWall,TileType::TopRightRoundOff),TileType::RightWall},
		//	{PairHashFunction(TileType::RightWall,TileType::BottomRightRoundOff),TileType::RightWall},
		//	{PairHashFunction(TileType::TopWall,TileType::LeftWall),TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomLeftRoundOff),TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomRightRoundOff),TileType::TopRightCorner},
		//	{PairHashFunction(TileType::TopWall,TileType::RightWall),TileType::TopRightCorner},
		//	{PairHashFunction(TileType::TopWall,TileType::MiddleLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::MiddleRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomWall),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::BottomLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopWall,TileType::TopLeftRoundOff),TileType::TopWall},
		//	{PairHashFunction(TileType::TopWall,TileType::TopRightRoundOff),TileType::TopWall},
		//	{PairHashFunction(TileType::BottomWall,TileType::LeftWall),TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopLeftRoundOff),TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopRightRoundOff),TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::BottomWall,TileType::RightWall),TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::BottomWall,TileType::MiddleLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::MiddleRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopWall),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::TopLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomWall,TileType::BottomLeftRoundOff),TileType::BottomWall},
		//	{PairHashFunction(TileType::BottomWall,TileType::BottomRightRoundOff),TileType::BottomWall},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::LeftWall),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::RightWall),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::BottomRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::BottomLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::MiddleLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::MiddleRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopWall),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::TopRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::BottomLeftRoundOff),TileType::BottomMiddleRoundOff},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::BottomWall),TileType::BottomMiddleRoundOff},
		//	{PairHashFunction(TileType::BottomMiddleRoundOff,TileType::BottomRightRoundOff),TileType::BottomMiddleRoundOff},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::LeftWall),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::RightWall),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::MiddleLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::MiddleRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::TopRightCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::TopLeftCorner),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomWall),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomLeftRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomMiddleRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::BottomRightRoundOff),TileType::Empty},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::TopLeftRoundOff),TileType::TopMiddleRoundOff},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::TopWall),TileType::TopMiddleRoundOff},
		//	{PairHashFunction(TileType::TopMiddleRoundOff,TileType::TopRightRoundOff),TileType::TopMiddleRoundOff},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::MiddleRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::RightWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::LeftWall), TileType::MiddleLeftRoundOff},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::TopLeftRoundOff), TileType::MiddleLeftRoundOff},
		//	{PairHashFunction(TileType::MiddleLeftRoundOff,TileType::BottomLeftRoundOff), TileType::MiddleLeftRoundOff},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::MiddleLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::LeftWall), TileType::Empty},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::RightWall), TileType::MiddleRightRoundOff},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::TopRightRoundOff), TileType::MiddleRightRoundOff},
		//	{PairHashFunction(TileType::MiddleRightRoundOff,TileType::BottomRightRoundOff), TileType::MiddleRightRoundOff},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::RightWall), TileType::TopRightCorner},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::BottomWall), TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::TopRightRoundOff), TileType::TopWall},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::BottomLeftRoundOff), TileType::LeftWall},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::BottomRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::BottomMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::MiddleRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopLeftRoundOff,TileType::BottomRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::LeftWall), TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::BottomWall), TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::BottomRightRoundOff), TileType::RightWall},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::TopLeftRoundOff), TileType::TopWall},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::BottomLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::BottomLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::MiddleLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::BottomMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::TopRightRoundOff,TileType::MiddleRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::RightWall), TileType::BottomRightCorner},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::TopWall), TileType::TopLeftCorner},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::BottomRightRoundOff), TileType::BottomWall},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::TopLeftRoundOff), TileType::LeftWall},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::TopRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::TopMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::MiddleRightRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomLeftRoundOff,TileType::TopRightCorner), TileType::Empty},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::TopWall), TileType::TopRightCorner},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::LeftWall), TileType::BottomLeftCorner},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::TopRightRoundOff), TileType::RightWall},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::BottomLeftRoundOff), TileType::BottomWall},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::TopLeftCorner), TileType::Empty},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::TopLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::MiddleLeftRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::TopMiddleRoundOff), TileType::Empty},
		//	{PairHashFunction(TileType::BottomRightRoundOff,TileType::MiddleRightRoundOff), TileType::Empty},
		//};
	}
}



























