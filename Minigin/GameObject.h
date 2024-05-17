#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		void Update();
		void Init();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void ImGuiUpdate();

		Transform& GetTransform() { return m_Transform; }
		GameObject* GetParent() const { return m_pParent; }

		std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_Children; }
		

		void AddComponent(std::unique_ptr<Component> pComponent);
		void RemoveComponent(const Component* pComponent);

		void SetParent(GameObject* pParent,bool keepWorldPos);


		template<std::derived_from<dae::Component> T>
		T* GetComponent() const 
		{
			for (const auto& component : m_Components)
			{
				if (T* comp{ dynamic_cast<T*>(component.get()) })
				{
					return comp;
				}
			}
			return nullptr;
		}
		template<std::derived_from<dae::Component> T>
		std::vector<T*> GetComponents() const
		{
			std::vector<T*> components{};
			for (const auto& component : m_Components)
			{
				if (T* comp{ dynamic_cast<T*>(component.get()) })
				{
					components.push_back(comp);
				}
			}
			for (const auto& child : m_Children)
			{
				std::vector<T*> childComponents{ child->GetComponents<T>() };
				components.insert(components.end(), childComponents.begin(), childComponents.end());
			}
			return components;
		}
		
	private:
		void DetachChild(GameObject* go, bool keepWorldPosition);
		void AttachChild(std::shared_ptr<dae::GameObject> go, bool keepParentWorldPosition);
		bool IsInChildTree(dae::GameObject* go) const;

		Transform m_Transform;
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<std::shared_ptr<GameObject>> m_Children{};
		GameObject* m_pParent{};
	};
}

