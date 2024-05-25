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

		uint16_t GetReceivingCollisionLayers() const {return m_Rect.receivingCollisionLayers;}
		uint16_t GetSendingCollisionLayers() const {return m_Rect.sendingCollisionLayers;}
		void AddReceivingLayer(uint16_t layers)	{m_Rect.receivingCollisionLayers |= layers;}
		void AddSendingLayer(uint16_t layers)	{m_Rect.sendingCollisionLayers |= layers;}
		void RemoveReceivingLayer(uint16_t layers)	{m_Rect.receivingCollisionLayers &= ~layers;}
		void RemoveSendingLayer(uint16_t layers)	{m_Rect.sendingCollisionLayers &= ~layers;}

		void Rotate();
		Signal<CollisionRectComponent*> m_OnExit{};
		Signal<CollisionRectComponent*> m_OnEnter{};
		Signal<CollisionRectComponent*> m_OnInside{};
	private:
		CollisionRectInfo m_Rect{};
		bool m_ExitSignalCalled{};
		bool m_EnterSignalCalled{};
		void CallOnExit();
		void CallOnEnter();
		CollisionRectComponent* m_IntersectedObject{};
	};
}
