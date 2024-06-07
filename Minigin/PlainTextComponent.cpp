#include "PlainTextComponent.h"

#include <SDL_ttf.h>
#include <stdexcept>
#include <utility>

#include "Font.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "GameObject.h"



dae::PlainTextComponent::PlainTextComponent(GameObject* pParent,std::string text, std::shared_ptr<Font> m_pFont, SDL_Color color)
	: Component{ pParent }
	, m_Text{text}
	, m_pFont{ m_pFont }
	, m_Color{ color }
{
	CreateTexture();
}

void dae::PlainTextComponent::Update()
{
	if (m_Dirty)
	{
		CreateTexture();
		m_Dirty = false;
	}
}

void dae::PlainTextComponent::Render() const
{
	if (!m_pTexture)
		throw (std::string("PlainTextComponent::Render() > No texture found!"));

	const glm::vec2& pos = GetParent()->GetTransform().GetWorldPosition() + m_RenderOffset;
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::PlainTextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_Dirty = true;
}

void dae::PlainTextComponent::SetRenderOffset(const glm::vec2& offset)
{
	m_RenderOffset = offset;
}


void dae::PlainTextComponent::CreateTexture()
{
	SDL_Surface* surface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	if (surface == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(),surface) };
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surface);

	m_pTexture = std::make_shared<Texture2D>(texture);

}

