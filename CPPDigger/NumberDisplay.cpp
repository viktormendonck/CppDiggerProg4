#include "NumberDisplay.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Transform.h"

namespace dae
{
	NumberDisplay::NumberDisplay(GameObject* pParent, Signal<int>* scoreUpdatedSignal,std::shared_ptr<Texture2D> pTexture, glm::vec2 renderScale, glm::ivec2 renderOffset)
		: Component(pParent),
		m_pTexture{ std::move(pTexture) },
		m_RenderScale{ renderScale },
		m_RenderOffset{ renderOffset }
	{
		scoreUpdatedSignal->AddListener([this](int num) { UpdateNumber(num); });
		UpdateNumber(0);
		m_SpriteSize = m_pTexture->GetSize();
		m_SpriteSize.x /= 10;
	}

	void NumberDisplay::Render() const
	{
		Transform& transform = GetParent()->GetTransform();
		glm::vec2 renderPos{ transform.GetWorldPosition().x + static_cast<float>(m_RenderOffset.x),transform.GetWorldPosition().y + static_cast<float>(m_RenderOffset.y) };
		for (const uint8_t num : m_Numbers)
		{
			Renderer::GetInstance().RenderSprite(*m_pTexture, glm::ivec2(num, 0), renderPos, 0, m_SpriteSize, m_RenderScale);
			renderPos.x += static_cast<float>(m_SpriteSize.x) * m_RenderScale.x;
		}
	}

	void NumberDisplay::UpdateNumber(int num)
	{
		m_Numbers.clear();
		const int length = static_cast<int>(log10(num)) + 1;
		for (int i = 0; i < length; i++) {
			m_Numbers.emplace_back(static_cast<uint8_t>(num % 10));
			num /= 10;
		}
		std::reverse(m_Numbers.begin(), m_Numbers.end());
	}
}
