#pragma once
#include <SDL_scancode.h>
#include <vector>
#include <glm/vec2.hpp>

#include "BiMap.h"
#include "CollisionRectComponent.h"
#include "EnemySpawnerComponent.h"
#include "GameObject.h"
#include "KeyboardDevice.h"
#include "json.hpp"

namespace dae
{
	class KeyboardDevice;
	class ControllerDevice;
	class EnemySpawnerComponent;
	class CollisionRectComponent;

	enum class CollisionLayers : uint16_t
	{
		Pickup = 1 << 0,
		PlayerDamage = 1 << 1,
		EnemyDamage = 1 << 2,
		Push = 1 << 3,
	};
	namespace mapData
	{
		//Most of this data should be in a json loader but hey, ig it works
		inline uint16_t BoolVecToUInt16(const std::vector<bool>& vec) {
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
			{BoolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					1,1,1,1,
					1,1,1,1
				}),TileType::Full},
			{BoolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				}),TileType::Empty},
			{BoolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					1,1,0,0,
					1,1,0,0
				}),TileType::LeftWall},
			{BoolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					0,0,0,0,
					0,0,0,0
				}),TileType::TopWall},
			{BoolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					1,1,1,1,
					1,1,1,1,
				}),TileType::BottomWall},
			{BoolVecToUInt16(
				{
					0,0,1,1,
					0,0,1,1,
					0,0,1,1,
					0,0,1,1
				}),TileType::RightWall},
			{BoolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					1,1,0,0,
					1,1,0,0
				}),TileType::TopLeftRoundOff},
			{BoolVecToUInt16(
				{
					1,1,1,1,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				}),TileType::TopMiddleRoundOff},
			{BoolVecToUInt16(
				{
					1,1,1,1,
					1,1,1,1,
					0,0,1,1,
					0,0,1,1
				}),TileType::TopRightRoundOff},
			{BoolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,1,1,
					0,0,1,1
				}),TileType::BottomRightCorner},
			{BoolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					1,1,0,0,
					1,1,0,0
				}),TileType::BottomLeftCorner},
			{BoolVecToUInt16(
				{
					0,0,1,1,
					0,0,1,1,
					0,0,0,0,
					0,0,0,0,
				}),TileType::TopRightCorner},
			{BoolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					0,0,0,0,
					0,0,0,0,
				}),TileType::TopLeftCorner},
			{BoolVecToUInt16(
				{
					1,0,0,0,
					1,0,0,0,
					1,0,0,0,
					1,0,0,0
				}),TileType::MiddleLeftRoundOff},
			{BoolVecToUInt16(
				{
					0,0,0,1,
					0,0,0,1,
					0,0,0,1,
					0,0,0,1
				}),TileType::MiddleRightRoundOff},
			{BoolVecToUInt16(
				{
					1,1,0,0,
					1,1,0,0,
					1,1,1,1,
					1,1,1,1
				}),TileType::BottomLeftRoundOff},
			{BoolVecToUInt16(
				{
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					1,1,1,1
				}),TileType::BottomMiddleRoundOff},
			{ BoolVecToUInt16(
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

	namespace levelLoader
	{
		enum class GameMode {
			SinglePlayer,
			CoOp,
			Versus
		};
		
		void AddGem(dae::GameObject* pParent, std::shared_ptr<dae::Signal<dae::GameObject*>> pAnyGemPickedUpSignal, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos);

		void AddGoldBag(dae::GameObject* pParent, std::shared_ptr<dae::Signal<dae::GameObject*>> pAnyGoldPickedUpSignal, const std::shared_ptr<dae::Texture2D>& pTexture, glm::vec2 pos);

		struct PlayerRequirements
		{
			GameObject* pParent;
			std::shared_ptr<Texture2D> pPlayerTexture;
			std::shared_ptr<Texture2D> pFireBallTex;
			glm::vec2 pos;
			KeyboardDevice* pKeyboard;
			ControllerDevice* pController;
			ControllerDevice* pControllerTwo;
			Signal<GameObject*>* pGemPickedUpSignal;
			Signal<GameObject*>* pGoldPickedUpSignal;
			Signal<GameObject*>* pEnemyKilledSignal;
			std::unique_ptr<Signal<int>> pScoreChangedSignal;
			std::unique_ptr<Signal<int>> pLivesChangedSignal;
			std::unique_ptr<Signal<int>> pPlayerTwoLivesChangedSignal;
			GameMode gameMode;
			int playerIndex{};
		};

		void AddPlayer(PlayerRequirements& requirements);

		void AddEnemySpawner(dae::GameObject* pParent, glm::vec2 pos, std::shared_ptr<dae::Signal<dae::GameObject*>> pAnyEnemyKilledSignal, std::shared_ptr<dae::Texture2D> pEnemyTex, int charges, float timeBetweenSpawns);

		void StartGame(GameMode gameMode);

		void OpenMenu();
		void OpenMenu(const std::string& name, int score);
	}
}

