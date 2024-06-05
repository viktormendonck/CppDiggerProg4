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
		uint16_t scansForLayers{};
		uint16_t existsOnLayers{};

		bool Intersect(const CollisionRectInfo& other) const;
	};


	class CollisionRectComponent final : public Component
	{
	public:
		CollisionRectComponent(GameObject* pParent,const glm::vec2 size, const glm::vec2 offset, uint16_t existsOnLayers, uint16_t scansForLayers);
		~CollisionRectComponent() override;
		void Update() override;
		void Render() const override;
		const CollisionRectInfo GetCollisionRect() const;

		uint16_t GetReceivingCollisionLayers() const {return m_Rect.scansForLayers;}
		uint16_t GetSendingCollisionLayers() const {return m_Rect.existsOnLayers;}
		void AddReceivingLayer(uint16_t layers)	{m_Rect.scansForLayers |= layers;}
		void AddSendingLayer(uint16_t layers)	{m_Rect.existsOnLayers |= layers;}
		void RemoveReceivingLayer(uint16_t layers)	{m_Rect.scansForLayers &= ~layers;}
		void RemoveSendingLayer(uint16_t layers)	{m_Rect.existsOnLayers &= ~layers;}

		void Rotate();
		bool ExistsOn(uint16_t layers) const;

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
