#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		explicit Component(GameObject* pParent);
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;
		
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}
		virtual void Render() const {}

		GameObject* GetParent() const { return m_pParent; }
	private:
		GameObject* m_pParent; 
	};
}

