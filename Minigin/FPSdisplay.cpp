#include "FPSdisplay.h"
#include "GameData.h"
#include "GameObject.h"
#include "PlainTextComponent.h"
#include <sstream>
#include <iomanip>



dae::FPSDisplay::FPSDisplay(GameObject* pParent, float updateTime)
	: Component{ pParent }, m_UpdateTime{ updateTime }
{
	m_pPlainTextComponent = pParent->GetComponent<PlainTextComponent>();

	if (!m_pPlainTextComponent) 
	{
		throw MissingComponentException{};
	}
}


void dae::FPSDisplay::Update()
{
	if (m_AccumulatedDeltas >= m_UpdateTime)
	{
		std::stringstream fpsStream;

		fpsStream <<"FPS:" << std::fixed << std::setprecision(2) << 1 / ( m_AccumulatedDeltas / m_DeltasBetweenUpdates);

		m_pPlainTextComponent->SetText(fpsStream.str());
		m_AccumulatedDeltas = 0;
		m_DeltasBetweenUpdates = 0;
	}
	else {
		m_DeltasBetweenUpdates++;
		m_AccumulatedDeltas += GameData::GetInstance().GetDeltaTime();
	}
	
}

