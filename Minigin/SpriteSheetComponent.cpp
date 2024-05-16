#include "SpriteSheetComponent.h"

#include "GameObject.h"
#include "GameData.h"
#include "Renderer.h"
#include "Texture2D.h"


dae::SpriteSheetComponent::SpriteSheetComponent(dae::GameObject* pParent, std::shared_ptr<Texture2D> pTexture, glm::ivec2 size, bool canRotate, float timePerFrame, bool animated)
	: Component(pParent),
	m_pTexture(pTexture),
	m_SpriteSheetSize(size),
	m_SpriteSize(m_pTexture->GetSize().x / size.x, m_pTexture->GetSize().y / size.y),
	m_CanRotate(canRotate),
	m_TimePerFrame(timePerFrame),
	m_IsAnimated(animated)
{
	m_CurrentSprite = { 0,0 };
}

void dae::SpriteSheetComponent::Update()
{
	if (!m_IsAnimated){return;}
	m_CurrentTime += GameData::GetInstance().GetDeltaTime();
	if (m_CurrentTime >= m_TimePerFrame)
	{
		m_CurrentTime -= m_TimePerFrame;
		m_CurrentSprite.x = ++m_CurrentSprite.x %m_SpriteSheetSize.x;
	}
}

void dae::SpriteSheetComponent::Render() const
{
	if (m_pTexture)
	{
		dae::Transform& transform = GetParent()->GetTransform();
		glm::vec2 renderPos{ transform.GetWorldPosition().x - (m_SpriteSize.x / 2),transform.GetWorldPosition().y + (m_SpriteSize.y / 2) };
		if (m_CanRotate)
		{
			Renderer::GetInstance().RenderSprite(*m_pTexture, m_CurrentSprite, renderPos, transform.GetWorldRotation(), m_SpriteSize,transform.GetLocalScale());
		}
		else
		{
			Renderer::GetInstance().RenderSprite(*m_pTexture, m_CurrentSprite, renderPos, 0, m_SpriteSize, transform.GetLocalScale());
		}
	}
}
