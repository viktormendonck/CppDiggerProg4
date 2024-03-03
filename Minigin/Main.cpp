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
	pLogoObject->GetTransform().SetPosition({ 216, 180});
	std::shared_ptr<dae::Texture2D> pLogo{ dae::ResourceManager::GetInstance().LoadTexture("logo.tga") };
	pLogoObject->AddComponent(std::make_unique<dae::TextureComponent>( pLogoObject.get(), pLogo));
	scene.Add(pLogoObject);
	
	//add the title text
	auto pTitleObject = std::make_shared<dae::GameObject>();
	pTitleObject->GetTransform().SetPosition({ 80, 20});
	pTitleObject->AddComponent(std::make_unique<dae::PlainTextComponent>(pTitleObject.get(), "Programming 4 Assignment", font, SDL_Color{ 255,255,255,255 }));
	scene.Add(pTitleObject);
	
	//add the fps display
	auto pFpsDisplay = std::make_shared<dae::GameObject>();
	pFpsDisplay->GetTransform().SetPosition({ 10, 80 });
	pFpsDisplay->AddComponent(std::make_unique<dae::PlainTextComponent>(pFpsDisplay.get(), "FPS: 0", font, SDL_Color{255,255,255,255}));
	pFpsDisplay->AddComponent(std::make_unique<dae::FPSDisplay>(pFpsDisplay.get(),0.2f));
	scene.Add(pFpsDisplay);

	//add test for scene graph
	std::shared_ptr<dae::Texture2D> pTexture{ dae::ResourceManager::GetInstance().LoadTexture("secondtest.tga") };
	std::shared_ptr<dae::Texture2D> pSecondTexture{ dae::ResourceManager::GetInstance().LoadTexture("test.tga") };
	auto pTestObject = std::make_shared<dae::GameObject>();
	pTestObject->GetTransform().SetPosition({ 100, 100 });
	pTestObject->AddComponent(std::make_unique<dae::TextureComponent>(pTestObject.get(), pTexture));
	pTestObject->AddComponent(std::make_unique<dae::RotationComponent>(pTestObject.get(), 10.f));
	auto pSecondTest = std::make_shared<dae::GameObject>();
	pSecondTest->AddComponent(std::make_unique<dae::TextureComponent>(pSecondTest.get(), pSecondTexture));
	pTestObject->AttachChild(pSecondTest,true);
	pSecondTest->GetTransform().SetPosition({ 0, 100 });
	auto pThirdTest = std::make_shared<dae::GameObject>();
	pThirdTest->AddComponent(std::make_unique<dae::TextureComponent>(pThirdTest.get(), pTexture));
	pThirdTest->AddComponent(std::make_unique<dae::RotationComponent>(pTestObject.get(), 10.f));
	pSecondTest->AttachChild(pThirdTest, true);
	pThirdTest->GetTransform().SetPosition({ 50, 0 });


	scene.Add(pTestObject);



}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}