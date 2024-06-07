#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <fstream>

#include "Minigin.h"

#include "MapData.h"
#include "NumberDisplay.h"
#include "json.hpp"

void load()
{
    std::vector<std::pair<std::string, int>> scores{
        std::make_pair("Player1", 1000),
        std::make_pair("Player", 500),
        std::make_pair("Player3", 200),
        std::make_pair("Player4", 150),
    };
    nlohmann::json j;

    for (const auto& score : scores) {
        nlohmann::json scoreJson;
        scoreJson["player"] = score.first;
        scoreJson["score"] = score.second;
        j.push_back(scoreJson);
    }

    std::ofstream file("Data/high_scores.json");
    file << j.dump(4);



	dae::levelLoader::StartGame(dae::levelLoader::GameMode::SinglePlayer);
}

int main(int, char* []) {

	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}