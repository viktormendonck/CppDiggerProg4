#include "MapData.h"


uint16_t dae::MapData::GetTileRule(TileType type)
{
	constexpr uint16_t empty = 0b0000000000000000;
	return g_TileRules.GetValue(type, empty);
}

dae::MapData::TileType dae::MapData::GetTileType(uint16_t tile)
{
	return g_TileRules.GetValue(tile, TileType::Empty);
}

dae::MapData::TileType dae::MapData::CompareTiles(TileType newTile, TileType oldTile)
{
	return GetTileType(GetTileRule(newTile) & GetTileRule(oldTile));
}
