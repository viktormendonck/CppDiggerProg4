#pragma once
#include "Component.h"
#include "TileMapComponent.h"

namespace dae
{
	class FireBallComponent final : public Component
	{
	public:
		FireBallComponent(GameObject* pParent, glm::ivec2 dir,GameObject* pPlayerObject);
		void Update() override;;
		void OnHit();
		GameObject* GetPlayerObject() const { return m_pPlayerObject; }
	private:
		GameObject* m_pPlayerObject;
		glm::ivec2 m_Dir{};
		const float m_Speed{ 64.f };
		TileMapComponent* GetTileMapComponent();

	};
}
