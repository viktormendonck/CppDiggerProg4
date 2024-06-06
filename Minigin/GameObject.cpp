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
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Update();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->Update();
	}
	
}

void dae::GameObject::Init()
{
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Init();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->Init();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->FixedUpdate();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->FixedUpdate();
	}
}
void dae::GameObject::LateUpdate()
{
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->LateUpdate();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->LateUpdate();
	}
}
void dae::GameObject::Render() const
{
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->Render();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->Render();
	}
}

void dae::GameObject::ImGuiUpdate()
{
	for (size_t i{}; i < m_Components.size(); ++i)
	{
		m_Components[i]->ImGuiUpdate();
	}
	for (size_t i{}; i < m_Children.size(); ++i)
	{
		m_Children[i]->ImGuiUpdate();
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

	for (const std::shared_ptr<GameObject>& child : m_Children)
	{
		if (child.get() == go) return true;
		if (child->m_Children.empty())
		{
			if (child->IsInChildTree(go)) return true;
		}
	}
	return false;
}
	

