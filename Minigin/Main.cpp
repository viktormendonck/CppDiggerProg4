#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "FPSdisplay.h"
#include "PlainTextComponent.h"
#include "RotationComponent.h"
#include "TrashTheCache.h"

#include "GameObject.h"
#include "Scene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Fps");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//add the background
	auto pBackgroundObject = std::make_shared<dae::GameObject>();
	std::shared_ptr<dae::Texture2D> pBackground{ dae::ResourceManager::GetInstance().LoadTexture("background.tga") };
	pBackgroundObject->AddComponent(std::make_unique<dae::TextureComponent>(pBackgroundObject.get(), pBackground));
	scene.Add(pBackgroundObject);

	//add the logo
	auto pLogoObject = std::make_shared<dae::GameObject>();
	pLogoObject->GetTransform().SetLocalPosition({ 216, 180});
	std::shared_ptr<dae::Texture2D> pLogo{ dae::ResourceManager::GetInstance().LoadTexture("logo.tga") };
	pLogoObject->AddComponent(std::make_unique<dae::TextureComponent>( pLogoObject.get(), pLogo));
	scene.Add(pLogoObject);
	
	//add the title text
	auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetLocalPosition({ 80, 20});
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Programming 4 Assignment", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);
	
	//add the imGui object
	auto pImGuiObject = std::make_shared<dae::GameObject>();
	pImGuiObject->AddComponent(std::make_unique<dae::TrashTheCache>(pImGuiObject.get()));
	scene.Add(pImGuiObject);
	
}

int main(int, char*[]) {
	SDL_SetMainReady();
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}