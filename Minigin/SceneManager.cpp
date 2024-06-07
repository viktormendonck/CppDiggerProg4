#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_Scenes[m_ActiveSceneIndex]->Update();
}

void dae::SceneManager::Init()
{
	for (std::shared_ptr<Scene>& scene : m_Scenes)
	{
		scene->Init();
	}
}

void dae::SceneManager::FixedUpdate()
{
	m_Scenes[m_ActiveSceneIndex]->FixedUpdate();
}

void dae::SceneManager::LateUpdate()
{
	m_Scenes[m_ActiveSceneIndex]->LateUpdate();
}

void dae::SceneManager::Render()
{
	m_Scenes[m_ActiveSceneIndex]->Render();
}

void dae::SceneManager::ImGuiUpdate()
{
	m_Scenes[m_ActiveSceneIndex]->ImGuiUpdate();
}

void dae::SceneManager::HandleDestroy()
{
	
	m_Scenes[m_ActiveSceneIndex]->HandleDestruction();
	

}

bool dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			m_ActiveSceneIndex = static_cast<int>(i);
			return true;
		}
	}
	return false;
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name)
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			return scene.get();
		}
	}
	return nullptr;
}

void dae::SceneManager::RemoveScene(const std::string& name)
{
	for (int i = static_cast<int>(m_Scenes.size())-1; i >= 0; --i)
	{
		if (m_Scenes[i]->GetName() == name)
		{
			m_Scenes.erase(m_Scenes.begin() + i);
			return;
		}
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
