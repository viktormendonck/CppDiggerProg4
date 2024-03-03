#include "GameObject.h"

#include "Component.h"
#include <stdexcept>


dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (const auto& component : m_Components)
	{
		component->Update();
	}
	for (const auto& child : m_Children)
	{
		child->Update();
	}
	
}
void dae::GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
	{
		component->FixedUpdate();
	}
	for (const auto& child : m_Children)
	{
		child->FixedUpdate();
	}
}
void dae::GameObject::LateUpdate()
{
	for (const auto& component : m_Components)
	{
		component->LateUpdate();
	}
	for (const auto& child : m_Children)
	{
		child->LateUpdate();
	}
}
void dae::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}
	for (const auto& child : m_Children)
	{
		child->Render();
	}
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> pComponent)
{
	if (!pComponent) 
	{
		throw std::runtime_error("GameObject::AddComponent > Component is nullptr");
	}
	m_Components.push_back(std::move(pComponent));
}

void dae::GameObject::RemoveComponent(const Component * pComponent)
{
	std::erase_if(m_Components, [&](const std::unique_ptr<Component>& pCurrentComp) -> bool
	{
		return pCurrentComp.get() == pComponent;
	});

}


void dae::GameObject::SetParent(GameObject* pParent)
{
	m_pParent = pParent;
}

glm::vec2 dae::GameObject::GetWorldPos()
{
	if (m_pParent)
	{
		return m_pParent->GetWorldPos() + m_Transform.GetPosition();
	}
	else
	{
		return m_Transform.GetPosition();
	}
}

std::shared_ptr<dae::GameObject> dae::GameObject::DetachChild(GameObject* go, bool keepWorldPosition)
{
	auto it = std::find_if(m_Children.begin(), m_Children.end(), [&](auto pChild) -> bool
		{
			return pChild.get() == go;
		});

	std::shared_ptr<GameObject> childDetached{ std::move(*it) };
	childDetached->SetParent(nullptr);
	m_Children.erase(it);
	if (keepWorldPosition)
	{
		childDetached->GetTransform().SetPosition(childDetached->GetWorldPos() + childDetached->GetTransform().GetPosition());
	}
	return childDetached;
}

void dae::GameObject::AttachChild(std::shared_ptr<dae::GameObject> go, bool keepWorldPosition)
{
	go->SetParent(this);
	if (keepWorldPosition)
	{
		go->GetTransform().SetPosition(go->GetTransform().GetPosition() - GetWorldPos());
	}
	else
	{
		go->GetTransform().SetPosition(go->GetWorldPos());
	}

	m_Children.emplace_back(std::move(go));
}


