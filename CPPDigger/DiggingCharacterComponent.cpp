#include "DiggingCharacterComponent.h"

dae::DiggingCharacterComponent::DiggingCharacterComponent(GameObject* pParent): Component(pParent)
{}

void dae::DiggingCharacterComponent::Dig(glm::ivec2 currentDir) const
{
	int dir{ -1 };
	for (int i{}; i < 4; i++) { if (m_DirToInt[i] == currentDir) dir = i; }
	TileMapComponent* pTileMap = GetParent()->GetPatriarch()->GetComponent<TileMapComponent>();
	for (int yIdx{ -1 }; yIdx <= 1; ++yIdx)
	{
		for (int xIdx{ -1 }; xIdx <= 1; ++xIdx)
		{
			glm::ivec2 spriteSize{};
			if (SpriteSheetComponent* spriteSheet = GetParent()->GetComponent<SpriteSheetComponent>())
			{
				spriteSize = spriteSheet->GetSpriteSize();
			} else
			{
				spriteSize = GetParent()->GetComponent<TextureComponent>()->GetTexSize();
			}
			glm::ivec2 currentTile = pTileMap->LocalToTile(GetParent()->GetTransform().GetLocalPosition() + glm::vec2{ spriteSize.x / 2,spriteSize.y / 2 }) + currentDir;
			//if the tile you are trying to dig is outside the map, skip
			if (currentTile.x + xIdx < 0 || currentTile.x + xIdx >= pTileMap->GetWorldSize().x ||
				currentTile.y + yIdx < 0 || currentTile.y + yIdx >= pTileMap->GetWorldSize().y) continue;

			const int idx = (currentTile.y + yIdx) * pTileMap->GetWorldSize().x + currentTile.x + xIdx;
			const int templateIdx = (yIdx + 1) * 3 + (xIdx + 1);

			const int newTile = MapData::g_DigPatterns[dir][templateIdx];
			const int oldTile = pTileMap->GetTileSprite(idx);
			if (newTile == -1 || oldTile == 10) continue;
			pTileMap->SetTileSprite(idx, static_cast<int>(MapData::CompareTiles(static_cast<MapData::TileType>(newTile), static_cast<MapData::TileType>(oldTile))));
		}
	}
}