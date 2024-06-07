#pragma once
#include <memory>

#include "Component.h"
#include "Signal.h"
#include "Texture2D.h"


namespace dae
{
	class LivesDisplay final : public Component
	{
	public:
		LivesDisplay(GameObject* pParent, const std::shared_ptr<Texture2D>& pTex,Signal<int>* pLivesChangedSignal);
		void Render() const override;
	private:
		void OnLivesChanged(int lives);
		std::shared_ptr<Texture2D> m_pTex;
		int m_Lives;

	};

}
