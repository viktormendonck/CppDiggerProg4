#pragma once
#include <memory>
#include <string>

#include "Component.h"
#include "Font.h"
#include "SingleScoreDisplay.h"
#include "Texture2D.h"


namespace dae
{
	class HighScoreDisplayComponent final : public Component
	{
	public:
		HighScoreDisplayComponent(GameObject* pParent, const std::string& HighScoreFilePath,int NewPlayerScore, const std::string& newPlayerName, const std::shared_ptr<Font>& font, const std::shared_ptr<Texture2D>& numberTexture);

		void Init() override;
		void Render() const override;
	private:
		int m_NewPlayerScore;
		std::string m_NewPlayerName;
		std::string m_HighScoreFilePath;

		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pNumberTexture;
		std::vector<std::unique_ptr<SingleScoreDisplay>> m_SingleScoreDisplays;
	};

}





