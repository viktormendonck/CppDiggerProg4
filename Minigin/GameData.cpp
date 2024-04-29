#include "GameData.h"

uint16_t dae::MapData::GetTileRule(TileType type)
{
	auto it = reversedTileRules.find(type);
	if (it != reversedTileRules.end())
	{
		return it->second;
	}
	return 0;
}

dae::MapData::TileType dae::MapData::GetTileType(uint16_t tile)
{
	auto it = TileRules.find(tile);
	if (it != TileRules.end())
	{
		return it->second;
	}
	return TileType::Empty;
}

dae::MapData::TileType dae::MapData::CompareTiles(TileType newTile, TileType oldTile)
{
	return GetTileType(GetTileRule(newTile) & GetTileRule(oldTile));
}
