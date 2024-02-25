#include "FPSdisplay.h"
#include "GameTime.h"
#include <sstream>
#include <iomanip>

dae::FPSDisplay::FPSDisplay(GameObject* pParent, std::shared_ptr<Font> font, SDL_Color color, float updateTime)
	: PlainTextComponent{ pParent,"FPS: 0", font, color }, m_updateTime{ updateTime }
{
}

void dae::FPSDisplay::Update()
{
	if (m_accumulatedDeltas >= m_updateTime)
	{
		std::stringstream fpsStream;

		fpsStream <<"FPS:" << std::fixed << std::setprecision(2) << 1 / ( m_accumulatedDeltas / m_deltasBetweenUpdates);


		SetText(fpsStream.str());
		m_accumulatedDeltas = 0;
		m_deltasBetweenUpdates = 0;
	}
	else {
		m_deltasBetweenUpdates++;
		m_accumulatedDeltas += GameTime::GetInstance().GetDeltaTime();
	}
	
}
