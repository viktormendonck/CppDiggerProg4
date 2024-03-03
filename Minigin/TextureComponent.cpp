#include "TextureComponent.h"

#include <stdexcept>

#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture)
	: Component{ pParent }
	, m_pTexture{std::move(pTexture)}
{
}

void dae::TextureComponent::Render() const
{
	if (!m_pTexture)
		throw (std::string("TextureRenderer::Render() > No texture found!"));
	const glm::vec2 pos = GetParent()->GetTransform().GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

