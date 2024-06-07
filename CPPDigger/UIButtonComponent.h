#pragma once
#include <glm/vec2.hpp>

#include "Component.h"
#include "Signal.h"

namespace dae
{
	class UIButtonComponent final : public Component
	{
	public:
		UIButtonComponent(GameObject* pParent,glm::vec2 size);
		void Update() override;
		Signal<> onClick;
	private:
		glm::vec2 m_Size;
		bool m_IsClicked;
	};

}
