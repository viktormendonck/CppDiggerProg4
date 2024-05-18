#include "TextureComponent.h"

#include <stdexcept>

#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"

dae::TextureComponent::TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture, bool canRotate)
	: Component{ pParent }
	, m_pTexture{ std::move(pTexture) }
	, m_CanRotate{ canRotate }
	, m_IsVisible{true}
{
}

void dae::TextureComponent::Render() const
{
	if (!m_IsVisible)
		return;
	if (!m_pTexture)
		throw (std::string("TextureRenderer::Render() > No texture found!"));
	Transform& transform = GetParent()->GetTransform();
	glm::vec2 texSize{ m_pTexture->GetSize() };
	glm::vec2 renderPos{transform.GetWorldPosition().x - texSize.x / 2, transform.GetWorldPosition().y - texSize.y / 2};
	if (m_CanRotate)
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, renderPos, transform.GetLocalRotation(), m_RenderScale);
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, renderPos,0, m_RenderScale);
	}
}

