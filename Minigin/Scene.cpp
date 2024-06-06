#include "Scene.h"
#include "GameObject.h"
#include <algorithm>



using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
}


void Scene::ImGuiUpdate()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->ImGuiUpdate();
	}
}

void Scene::Remove(const std::shared_ptr<GameObject>& object)
{
	std::erase(m_Objects, object);
}


void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::HandleDestruction()
{
	if (m_Objects.empty()) return;
	for (int i = static_cast<int>(m_Objects.size()) - 1; i >= 0; --i)
	{
		if (!m_Objects[i]->GetChildren().empty())
		{
			HandleObjectDestruction(m_Objects[i]);
		}
		if (!m_Objects[i]->IsMarkedForDestruction()) continue;
		std::erase(m_Objects, m_Objects[i]);
	}
}

void Scene::HandleObjectDestruction(const std::shared_ptr<GameObject>& object)
{
	std::vector<std::shared_ptr<GameObject>> children = object->GetChildren();
	if (children.empty()) return;
	for (int i = static_cast<int>(children.size()) -1 ; i >= 0; --i)
	{
		if (!children[i]->GetChildren().empty())
		{
			HandleObjectDestruction(children[i]);
		}
		if (!children[i]->IsMarkedForDestruction()) continue;
		object->Erase(children[i]);
		//TODO: THIS IS SO FUCKED LIKE SOMETHING DIED IN HERE
	}
}

void dae::Scene::FixedUpdate()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->FixedUpdate();
	}
}

void Scene::Update()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->Update();
	}
}

void Scene::Init()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->Init();
	}
}

void dae::Scene::LateUpdate()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->LateUpdate();
	}
}

void Scene::Render() const
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->Render();
	}
}

