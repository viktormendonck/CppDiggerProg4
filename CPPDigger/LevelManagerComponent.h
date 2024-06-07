#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Component.h"
#include "ControllerDevice.h"
#include "KeyboardDevice.h"
#include "MapData.h"
#include "Texture2D.h"

namespace dae
{

	class LevelManagerComponent final : public Component
	{
	public:
		
		LevelManagerComponent(GameObject* pParent,levelLoader::GameMode gameMode, std::unique_ptr<Signal<int>> pScoreChangedSignal, std::unique_ptr<Signal<int>> pLivesChangedSignal, KeyboardDevice* pKeyboard, ControllerDevice* pController, ControllerDevice* pControllerTwo);
		void Init() override; // make player Object and load first level
		void Update() override;

		void NextLevel();

	private:
		void LoadNextLevel();
		void CloseLevel();

		const std::vector<std::string> m_Levels{
			"Data/Level1.json",
			"Data/Level2.json",
			"Data/Level3.json"
		};
		std::shared_ptr<Texture2D> m_pPlayerTexture;
		std::shared_ptr<Texture2D> m_pEnemyTexture;
		std::shared_ptr<Texture2D> m_pFireBallTexture;
		std::shared_ptr<Texture2D> m_pGoldBagTexture;
		std::shared_ptr<Texture2D> m_pGemTexture;
		KeyboardDevice* m_pKeyboard;
		ControllerDevice* m_pController;
		ControllerDevice* m_pControllerTwo;

		std::shared_ptr<Signal<GameObject*>> m_pAnyGoldPickedUpSignal;
		std::shared_ptr<Signal<GameObject*>> m_pAnyGemPickedUpSignal;
		std::shared_ptr<Signal<GameObject*>> m_pAnyEnemyKilledSignal;

		std::shared_ptr<GameObject> m_pPlayer;

		std::unique_ptr<Signal<int>> m_pScoreChangedSignal;
		std::unique_ptr<Signal<int>> m_pLivesChangedSignal;
		levelLoader::GameMode m_GameMode;
		int m_Level{0};
		bool m_IsInitialized{};

		bool m_ShouldLoadNextLevel{};
		std::string m_PlayerName{"Test"};
	};

}
