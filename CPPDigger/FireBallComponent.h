#pragma once
#include "Component.h"
#include "TileMapComponent.h"

namespace dae
{
	class FireBallComponent final : public Component
	{
	public:
		FireBallComponent(GameObject* pParent, glm::ivec2 dir);
		void Update() override;;
		void OnHit();
	private:
		glm::ivec2 m_Dir{};
		const float m_Speed{ 64.f };
		TileMapComponent* GetTileMapComponent();

	};
}
