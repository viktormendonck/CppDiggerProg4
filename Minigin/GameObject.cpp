#include "GameObject.h"

#include "Component.h"


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component->Update();
	}
}
void dae::GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> pComponent)
{
	m_Components.push_back(std::move(pComponent));
}

void dae::GameObject::RemoveComponent(const Component * pComponent)
{
	std::erase_if(m_Components, [&](const std::unique_ptr<Component>& pCurrentComp) -> bool
	{
		return pCurrentComp.get() == pComponent;
	});

}
