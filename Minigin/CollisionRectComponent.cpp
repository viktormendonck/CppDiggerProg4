#include "CollisionRectComponent.h"

#include "GameData.h"
#include "GameObject.h"
#include "Renderer.h"

namespace dae
{
	bool rect::Intersect(const rect& other) const
	{
		return (pos.x < other.pos.x + other.size.x &&
			pos.x + size.x > other.pos.x &&
			pos.y < other.pos.y + other.size.y &&
			pos.y + size.y > other.pos.y);
	}

	CollisionRectComponent::CollisionRectComponent(GameObject* pParent,const glm::vec2 size, const glm::vec2 offset)
		: Component{ pParent },
		m_Rect{ offset, size }
	{
		GameData::GetInstance().AddCollisionRect(this);
	}

	void CollisionRectComponent::Update()
	{
		bool intersect = false;
		for (const auto& other : GameData::GetInstance().GetCollisionRects())
		{
			if (other == this)	continue;
			intersect |= GetCollisionRect().Intersect(other->GetCollisionRect());
		}

		if (intersect)
		{
			if (!m_EnterSignalCalled)
			{
				CallOnEnter();
			} else
			{
				m_OnInside.Emit(GetParent());
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
		glm::vec2 pos = m_Rect.pos + GetParent()->GetTransform().GetWorldPosition();
		Renderer::GetInstance().RenderRect(pos, m_Rect.size,SDL_Color(255,0,0,255));
	}

	const rect CollisionRectComponent::GetCollisionRect() const
	{
		glm::vec2 pos = m_Rect.pos + GetParent()->GetTransform().GetWorldPosition();
		return { pos,m_Rect.size };
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
		m_OnExit.Emit(GetParent());
	}

	void CollisionRectComponent::CallOnEnter()
	{
		m_EnterSignalCalled = true;
		m_ExitSignalCalled = false;
		m_OnEnter.Emit(GetParent());
	}
}
