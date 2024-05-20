#pragma once
#include <map>
#include <vector>
#include <glm/vec2.hpp>
#include "BiMap.h"
#include "CollisionRectComponent.h"
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
		void AddCollisionRect(CollisionRectComponent* pRect) { m_CollisionRects.push_back(pRect); }
		void RemoveCollisionRect(CollisionRectComponent* pRect) {
			if (m_CollisionRects.empty()) return;
			std::erase(m_CollisionRects, pRect);
		}
		const std::vector<CollisionRectComponent*>& GetCollisionRects() const { return m_CollisionRects; }
		

	private:

		std::vector<CollisionRectComponent*> m_CollisionRects{};
		float m_DeltaTime = 1.f/60.f;

	};
}



























