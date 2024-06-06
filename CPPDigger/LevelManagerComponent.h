#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "Texture2D.h"

namespace dae
{

	class LevelManagerComponent final : public Component
	{
	public:
		LevelManagerComponent


	private:
		const std::vector<std::string> m_Levels{
			"../Data/Level1.json",
			"../Data/Level2.json",
			"../Data/Level3.json"
		};
		std::shared_ptr<Texture2D> m_pPlayerTexture;
		std::shared_ptr<Texture2D> m_pEnemyTexture;
		std::shared_ptr<Texture2D> m_pGoldBagTexture;
		std::shared_ptr<Texture2D> m_pGemTexture;
	};

}
