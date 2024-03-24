#include "ScoreDisplays.h"
#include <sstream>

#include "PlainTextComponent.h"
#include "PlayerController.h"
#include "GameObject.h"

dae::ScoreDisplay::ScoreDisplay(GameObject* pParent, PlayerController* pController)
	: Component(pParent), m_pPlayerController{ pController }
{
	m_pPlainTextComponent = pParent->GetComponent<PlainTextComponent>();
	m_pPlayerController->OnScoreChanged.AddListener(this, &ScoreDisplay::UpdateDisplay);
}

void dae::ScoreDisplay::UpdateDisplay()
{
	std::stringstream scoreStream{};
	scoreStream << "Score: " << m_pPlayerController->GetScore();
	m_pPlainTextComponent->SetText(scoreStream.str());
}

dae::LivesDisplay::LivesDisplay(GameObject* pParent, PlayerController* pController)
	: Component(pParent), m_pPlayerController{ pController }
{
	m_pPlainTextComponent = pParent->GetComponent<PlainTextComponent>();
	m_pPlayerController = pController;
	m_pPlayerController->OnDeath.AddListener(this, &LivesDisplay::UpdateDisplay);
}

void dae::LivesDisplay::UpdateDisplay()
{
	std::stringstream livesStream{};
	livesStream << "Lives: " << m_pPlayerController->GetLives();
	m_pPlainTextComponent->SetText(livesStream.str());
}
