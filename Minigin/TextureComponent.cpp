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
{
}

void dae::TextureComponent::Render() const
{
	if (!m_pTexture)
		throw (std::string("TextureRenderer::Render() > No texture found!"));
	Transform& transform = GetParent()->GetTransform();
	if (m_CanRotate)
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, transform.GetWorldPosition(), transform.GetWorldRotation(), transform.GetWorldScale());
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, transform.GetWorldPosition(),0, transform.GetWorldScale());
	}
}

