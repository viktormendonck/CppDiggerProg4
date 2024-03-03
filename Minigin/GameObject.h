#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		Transform& GetTransform() { return m_Transform; }
		GameObject* GetParent() const { return m_pParent; }

		std::vector<std::shared_ptr<GameObject>> GetChildren() const { return m_Children; }
		

		void AddComponent(std::unique_ptr<Component> pComponent);
		void RemoveComponent(const Component* pComponent);

		void SetParent(GameObject* pParent);

		std::shared_ptr<dae::GameObject> DetachChild(GameObject* go, bool keepWorldPosition);
		void AttachChild(std::shared_ptr<dae::GameObject> go, bool keepWorldPosition);

		template<std::derived_from<dae::Component> T>

		T* GetComponent() const // currently not using, but could be nice to have
		{
			for (const auto& component : m_Components)
			{
				T* comp = dynamic_cast<T*>(component.get());
				if (comp)
				{
					return comp;
				}
			}
			return nullptr;
		}
	private:
		Transform m_Transform;
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<std::shared_ptr<GameObject>> m_Children{};
		GameObject* m_pParent{};
	};
}

