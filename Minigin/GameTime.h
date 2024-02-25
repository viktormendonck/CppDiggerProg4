#pragma once
#include "Singleton.h"

namespace dae
{
	class GameTime : public Singleton<GameTime>
	{
		public:
			float GetDeltaTime() const { return m_DeltaTime; }
			void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
			static constexpr float FIXED_TIMESTEP = 1.f / 50.f;
		private:
		float m_DeltaTime = 1.f/60.f;
	};

}

