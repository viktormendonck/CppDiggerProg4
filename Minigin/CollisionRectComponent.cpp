#include "CollisionRectComponent.h"

#include "GameData.h"
#include "GameObject.h"
#include "Renderer.h"

namespace dae
{
	bool CollisionRectInfo::Intersect(const CollisionRectInfo& other) const
	{
		const bool isIntersecting (pos.x < other.pos.x + other.size.x &&
			pos.x + size.x > other.pos.x &&
			pos.y < other.pos.y + other.size.y &&
			pos.y + size.y > other.pos.y);
		const bool sharesCollisionLayer = (other.sendingCollisionLayers & receivingCollisionLayers) != 0;
		return isIntersecting && sharesCollisionLayer;
	}

	CollisionRectComponent::CollisionRectComponent(GameObject* pParent,const glm::vec2 size, const glm::vec2 offset,uint16_t receivingCollisionLayers,uint16_t sendingCollisionLayers)
		: Component{ pParent },
		m_Rect{ offset, size,receivingCollisionLayers,sendingCollisionLayers }
	{
		GameData::GetInstance().AddCollisionRect(this);
	}

	CollisionRectComponent::~CollisionRectComponent()
	{
		GameData::GetInstance().RemoveCollisionRect(this);
		Component::~Component();
	}

	void CollisionRectComponent::Update()
	{
		bool intersect = false;
		for (const auto& other : GameData::GetInstance().GetCollisionRects())
		{
			if (other == this)	continue;
			if (bool temp = GetCollisionRect().Intersect(other->GetCollisionRect()); temp)
			{
				intersect = temp;
				m_IntersectedObject = other->GetParent();
				break;
			}

		}

		if (intersect)
		{
			if (!m_EnterSignalCalled)
			{
				CallOnEnter();
			} else
			{
				m_OnInside.Emit(m_IntersectedObject);
			}
		}
		else
		{
			if (!m_ExitSignalCalled)
			{
				CallOnExit();
			}
		}
	}

	void CollisionRectComponent::Render() const
	{
#ifdef _DEBUG
		glm::vec2 pos = m_Rect.pos + GetParent()->GetTransform().GetWorldPosition();
		Renderer::GetInstance().RenderRect(pos, m_Rect.size,SDL_Color(255,0,0,255));
#endif
	}

	const CollisionRectInfo CollisionRectComponent::GetCollisionRect() const
	{
		glm::vec2 pos = m_Rect.pos + GetParent()->GetTransform().GetWorldPosition();
		return { pos,m_Rect.size,m_Rect.receivingCollisionLayers,m_Rect.sendingCollisionLayers };
	}

	void CollisionRectComponent::Rotate()
	{
		glm::vec2 size = {m_Rect.size.x, m_Rect.size.y};
		m_Rect.size.x = size.y;
		m_Rect.size.y = size.x;
	}

	void CollisionRectComponent::CallOnExit()
	{
		m_ExitSignalCalled = true;
		m_EnterSignalCalled = false;
		m_OnExit.Emit(m_IntersectedObject);
	}

	void CollisionRectComponent::CallOnEnter()
	{
		m_EnterSignalCalled = true;
		m_ExitSignalCalled = false;
		m_OnEnter.Emit(m_IntersectedObject);
	}
}
