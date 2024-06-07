#include "HighScoreDisplay.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

#include "GameObject.h"

dae::HighScoreDisplayComponent::HighScoreDisplayComponent(GameObject* pParent, const std::string& HighScoreFilePath,int NewPlayerScore, const std::string& newPlayerName, const std::shared_ptr<Font>& font,const std::shared_ptr<Texture2D>& numberTexture)
	: Component(pParent)
	, m_HighScoreFilePath(HighScoreFilePath)
	, m_NewPlayerScore(NewPlayerScore)
	, m_NewPlayerName(newPlayerName)
	, m_pFont(font)
	, m_pNumberTexture(numberTexture)
{
}

void dae::HighScoreDisplayComponent::Init()
{
	std::ifstream file(m_HighScoreFilePath);
	nlohmann::json j;
	file >> j;
	std::vector<std::pair<std::string, int>> scores;
	for (const auto& scoreJson : j) {
		
		std::string player = scoreJson["player"];
		int score = scoreJson["score"];
		std::pair<std::string, int> pair = std::make_pair(player, score);
		scores.push_back(pair);
	}
	scores.push_back(std::make_pair(m_NewPlayerName, m_NewPlayerScore));
	std::ranges::sort(scores, std::greater{}, &std::pair<std::string,int>::second);


	nlohmann::json write;
	for (int i = 0; i < 5;++i)
	{
		if (i >= scores.size())
		{
			return;
		}
		glm::vec2 renderOffset = { 0,i * 50 };
		m_SingleScoreDisplays.emplace_back(std::make_unique<SingleScoreDisplay>(GetParent(), renderOffset, scores[i].first, scores[i].second, m_pFont, m_pNumberTexture));

		nlohmann::json scoreJson;
		scoreJson["player"] = scores[i].first;
		scoreJson["score"] = scores[i].second;
		write.push_back(scoreJson);
	}
	std::ofstream outFile("Data/high_scores.json");
	outFile << write.dump(4);

}

void dae::HighScoreDisplayComponent::Render() const
{
	for (int i{}; i< m_SingleScoreDisplays.size();++i)
	{
		m_SingleScoreDisplays[i]->Render();
	}
}
