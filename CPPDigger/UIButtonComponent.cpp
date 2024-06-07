#include "UIButtonComponent.h"

#include <SDL_mouse.h>

#include "GameObject.h"

dae::UIButtonComponent::UIButtonComponent(GameObject* pParent, glm::vec2 size)
	: Component(pParent)
	, m_Size(size)
{
}

void dae::UIButtonComponent::Update()
{
	int x, y;
	Uint32 clickState = SDL_GetMouseState(&x, &y);
	glm::vec2 pos = GetParent()->GetTransform().GetWorldPosition();
	if (x >= pos.x && x <= pos.x + m_Size.x && y >= pos.y && y <= pos.y + m_Size.y)
	{
		if (clickState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (!m_IsClicked)
			{
				m_IsClicked = true;
				onClick.Emit();
			}
		}
		else
		{
			m_IsClicked = false;
		}
	}
	else
	{
		m_IsClicked = false;
	}
}
