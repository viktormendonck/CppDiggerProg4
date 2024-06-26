#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		friend class GameObject;
		explicit Component(GameObject* pParent);
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept = delete;
		
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void LateUpdate() {}
		virtual void Render() const {}
		virtual void ImGuiUpdate() {}
		virtual void Init() {}

		GameObject* GetParent() const { return m_pParent; }
	protected:
		bool m_HasInitialized{};
	private:
		GameObject* m_pParent;
	};
}

