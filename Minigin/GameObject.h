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
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
		
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		void SetPosition(float x, float y);

		Transform& GetTransform() { return m_Transform; }

		void AddComponent(std::unique_ptr<Component> pComponent);
		void RemoveComponent(const Component* pComponent);

	private:
		Transform m_Transform{};
		std::vector<std::unique_ptr<Component>> m_Components{};
	};
}
