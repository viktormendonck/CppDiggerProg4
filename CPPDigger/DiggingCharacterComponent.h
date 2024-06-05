#pragma once
#include <glm/vec2.hpp>

#include "Component.h"
#include "GameObject.h"

namespace dae
{

	class DiggingCharacterComponent : public Component
	{
	public:
		DiggingCharacterComponent(GameObject* pParent);

	protected:
		const glm::ivec2 m_DirToInt[4]{
			glm::ivec2{ 0,-1},
			glm::ivec2{ 1, 0},
			glm::ivec2{ 0, 1},
			glm::ivec2{-1, 0}
		};
		void Dig(glm::ivec2 currentDir) const;
	};

}
