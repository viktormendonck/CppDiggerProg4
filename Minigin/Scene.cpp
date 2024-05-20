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
	for (auto& object : m_Objects)
	{
		object->ImGuiUpdate();
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
	auto children = object->GetChildren();
	if (children.empty()) return;
	for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
	{
		if (!children[i]->GetChildren().empty())
		{
			HandleObjectDestruction(children[i]);
		}
		if (!children[i]->IsMarkedForDestruction()) continue;
		object->Erase(children[i]);
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for (auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Init()
{
	for (auto& object : m_Objects)
	{
		object->Init();
	}
}

void dae::Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

