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

void dae::Scene::ObjectDestruction(const std::shared_ptr<GameObject>& object)
{
	std::erase(m_Objects, object);
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

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
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

