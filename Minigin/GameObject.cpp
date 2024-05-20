#include "GameObject.h"

#include "Component.h"
#include <stdexcept>

#include "CollisionRectComponent.h"
#include "GameData.h"


dae::GameObject::GameObject()
	:m_Transform{ Transform(this) }
{
}

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

void dae::GameObject::Init()
{
	for (const auto& component : m_Components)
	{
		component->Init();
	}
	for (const auto& child : m_Children)
	{
		child->Init();
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

void dae::GameObject::ImGuiUpdate()
{
	for (const auto& component : m_Components)
	{
		component->ImGuiUpdate();
	}
	for (const auto& child : m_Children)
	{
		child->ImGuiUpdate();
	}

}

void dae::GameObject::Erase(std::shared_ptr<GameObject> go)
{
	std::erase(m_Children, go);
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> pComponent)
{
	if (!pComponent) 
	{
		throw std::runtime_error("GameObject::AddComponent > Component is nullptr");
	}
	m_Components.push_back(std::move(pComponent));
}

void dae::GameObject::RemoveComponent(const Component* pComponent)
{
	std::erase_if(m_Components, [&](const std::unique_ptr<Component>& pCurrentComp) -> bool
	{
		return pCurrentComp.get() == pComponent;
	});

}


void dae::GameObject::SetParent(GameObject* pParent,bool keepWorldPos)
{
	//if parent is already parent or if it creates a loop, return
	if (m_pParent == pParent || IsInChildTree(pParent)) return;
	
	//remove itself from previous parent
	if (m_pParent)
	{
		m_pParent->DetachChild(this,keepWorldPos);
	}
	//add itself to new parent
	if (pParent)
	{
		pParent->AttachChild(shared_from_this(), keepWorldPos);
	}
	//set new parent
	m_pParent = pParent;
	GetTransform().SetDirty();
}


void dae::GameObject::DetachChild(GameObject* go, bool keepWorldPosition)
{
	//use the find_if algorithm from ranges to find the child
	const auto it = std::ranges::find_if(m_Children, [&](auto pChild) -> bool
	{
		return pChild.get() == go;
	});
	if (it == m_Children.end()) return;
	const std::shared_ptr<GameObject> childDetached{ std::move(*it) };
	if (keepWorldPosition)
	{
		childDetached->GetTransform().SetLocalPosition(childDetached->GetTransform().GetWorldPosition() + childDetached->GetTransform().GetWorldPosition());
	}
	m_Children.erase(it);
}

void dae::GameObject::AttachChild(std::shared_ptr<dae::GameObject> go, bool keepParentWorldPosition)
{
	if (keepParentWorldPosition)
	{
		go->GetTransform().SetLocalPosition(go->GetTransform().GetLocalPosition() - go->GetTransform().GetWorldPosition());
	}
	else
	{
		go->GetTransform().SetLocalPosition(go->GetTransform().GetLocalPosition());
	}

	m_Children.emplace_back(std::move(go));
}

bool dae::GameObject::IsInChildTree(dae::GameObject* go) const
{

	for (const auto& child : m_Children)
	{
		if (child.get() == go) return true;
		if (child->m_Children.empty())
		{
			if (child->IsInChildTree(go)) return true;
		}
	}
	return false;
}
	

