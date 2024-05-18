#pragma once
#include <glm/vec2.hpp>

#include "Component.h"
#include "Signal.h"

namespace dae
{
	struct rect
	{
		glm::vec2 pos;
		glm::vec2 size;
		bool Intersect(const rect& other) const;
	};


	class CollisionRectComponent final : public Component
	{
	public:
		CollisionRectComponent(GameObject* pParent,const glm::vec2 size, const glm::vec2 offset);
		void Update() override;
		void Render() const override;
		const rect GetCollisionRect() const;
		
		void Rotate();
		Signal<GameObject*> m_OnExit{};
		Signal<GameObject*> m_OnEnter{};
		Signal<GameObject*> m_OnInside{};
	private:
		rect m_Rect{};
		bool m_ExitSignalCalled{};
		bool m_EnterSignalCalled{};
		void CallOnExit();
		void CallOnEnter();
		GameObject* m_IntersectedObject{};
	};
}
