﻿#include "FireBallComponent.h"
#include "TileMapComponent.h"
#include "SpriteSheetComponent.h"
#include "GameObject.h"
#include "Component.h"
#include "GameData.h"
#include "MapData.h"


namespace dae

{

	FireBallComponent::FireBallComponent(GameObject* pParent, glm::ivec2 dir)
		: Component(pParent),
			m_Dir(dir)
	{
		GetParent()->GetComponent<CollisionRectComponent>()->m_OnEnter.AddListener([this](CollisionRectComponent*) {OnHit();});
	}

	void FireBallComponent::Update()
	{
		Transform& transform = GetParent()->GetTransform();
		glm::vec2 SpriteSize = GetParent()->GetComponent<SpriteSheetComponent>()->GetSpriteSize();
		glm::vec2 pos = transform.GetLocalPosition() + glm::vec2{SpriteSize.x/2,SpriteSize.y/2};
		TileMapComponent* tileMap = GetTileMapComponent();
		MapData::TileType tile{static_cast<MapData::TileType>(tileMap->GetTileSprite(tileMap->LocalToTile(pos)))};

		if (tile != MapData::TileType::Empty)
		{
			OnHit();
			return;
		}

		transform.Translate(glm::vec2(m_Dir.x,m_Dir.y) * m_Speed * GameData::GetInstance().GetDeltaTime());
	}

	void FireBallComponent::OnHit()
	{
		GetParent()->Destroy();
	}


	TileMapComponent* FireBallComponent::GetTileMapComponent()
	{
		return GetParent()->GetPatriarch()->GetComponent<TileMapComponent>();
	}
}
