#pragma once
#include <glm/vec2.hpp>

#include "Component.h"
#include "Signal.h"

namespace dae
{
	
	struct CollisionRectInfo
	{
		glm::vec2 pos;
		glm::vec2 size;
		uint16_t receivingCollisionLayers{};
		uint16_t sendingCollisionLayers{};

		bool Intersect(const CollisionRectInfo& other) const;
	};


	class CollisionRectComponent final : public Component
	{
	public:
		CollisionRectComponent(GameObject* pParent,const glm::vec2 size, const glm::vec2 offset, uint16_t receivingCollisionLayers, uint16_t sendingCollisionLayers);
		~CollisionRectComponent() override;
		void Update() override;
		void Render() const override;
		const CollisionRectInfo GetCollisionRect() const;

		uint16_t& GetReceivingCollisionLayers()	{return m_Rect.receivingCollisionLayers;}
		uint16_t& GetSendingCollisionLayers()	{return m_Rect.sendingCollisionLayers;}

		void Rotate();
		Signal<GameObject*> m_OnExit{};
		Signal<GameObject*> m_OnEnter{};
		Signal<GameObject*> m_OnInside{};
	private:
		CollisionRectInfo m_Rect{};
		bool m_ExitSignalCalled{};
		bool m_EnterSignalCalled{};
		void CallOnExit();
		void CallOnEnter();
		GameObject* m_IntersectedObject{};
	};
}
