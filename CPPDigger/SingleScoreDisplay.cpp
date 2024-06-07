#include "SingleScoreDisplay.h"

#include "GameObject.h"
#include "Texture2D.h"


dae::SingleScoreDisplay::SingleScoreDisplay(GameObject* pParent, const glm::vec2 renderOffset, const std::string& name,
                                            int score, const std::shared_ptr<Font>& font, const std::shared_ptr<Texture2D>& numberTexture)
	: Component(pParent)
{
	m_pTextRenderer = std::make_unique<PlainTextComponent>(pParent, name, font, SDL_Color{204,116,0,255});
	m_pTextRenderer->SetRenderOffset(renderOffset);
	m_pNumberTexture = numberTexture;
	m_RenderOffset = renderOffset;
	m_SpriteSize = numberTexture->GetSize();
	m_SpriteSize.x /= 10;
	m_RenderScale = glm::vec2{1.5f,1.5f};
	SetName(name);
	UpdateNumber(score);
}

void dae::SingleScoreDisplay::Render() const
{
	m_pTextRenderer->Render();
	RenderNumber();
}

void dae::SingleScoreDisplay::SetName(const std::string& name)
{
	m_Name = name;
	m_pTextRenderer->SetText(name);
}

void dae::SingleScoreDisplay::RenderNumber() const
{
	Transform& transform = GetParent()->GetTransform();
	glm::vec2 renderPos{ transform.GetWorldPosition().x + static_cast<float>(m_RenderOffset.x)+180,transform.GetWorldPosition().y + static_cast<float>(m_RenderOffset.y)+10 };
	for (const uint8_t num : m_Numbers)
	{
		Renderer::GetInstance().RenderSprite(*m_pNumberTexture, glm::ivec2(num, 0), renderPos, 0, m_SpriteSize, m_RenderScale);
		renderPos.x += static_cast<float>(m_SpriteSize.x) * m_RenderScale.x;
	}
}

void dae::SingleScoreDisplay::UpdateNumber(int num)
{
	m_Numbers.clear();
	if (num == 0)
	{
		m_Numbers.emplace_back(Uint8{ 0 });
		return;
	}
	const int length = static_cast<int>(log10(num)) + 1;
	for (int i = 0; i < length; i++) {
		m_Numbers.emplace_back(static_cast<uint8_t>(num % 10));
		num /= 10;
	}
	std::reverse(m_Numbers.begin(), m_Numbers.end());
}
