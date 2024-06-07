#include "LivesDisplay.h"

#include "PlayerComponent.h"
#include "Renderer.h"

dae::LivesDisplay::LivesDisplay(GameObject* pParent, const std::shared_ptr<Texture2D>& pTex, Signal<int>* pLivesChangedSignal)
	: Component(pParent)
	, m_pTex(pTex)
	,m_Lives(3)
{
	pLivesChangedSignal->AddListener([this](int lives) {this->OnLivesChanged(lives); });
}

void dae::LivesDisplay::Render() const
{
	glm::vec2 renderPos = GetParent()->GetTransform().GetWorldPosition();
	for (int i = 0; i < m_Lives; ++i)
	{
		Renderer::GetInstance().RenderTexture(*m_pTex, i * (m_pTex->GetSize().x + 10) + renderPos.x, renderPos.y);
	}

}

void dae::LivesDisplay::OnLivesChanged(int lives)
{
	m_Lives = lives;
}
