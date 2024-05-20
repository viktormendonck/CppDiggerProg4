#pragma once
#include <memory>
#include <glm/vec2.hpp>

#include "Component.h"
#include "Texture2D.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture, bool rotate = true);
		
		void Render() const override;
		void SetVisibility(bool isVisible) { m_IsVisible = isVisible; }
		void SetRenderScale(glm::vec2 scale) { m_RenderScale = scale ;}
		glm::ivec2 GetTexSize()const  { return m_pTexture->GetSize(); }
	private:
		std::shared_ptr<Texture2D> m_pTexture;
		bool m_CanRotate{};
		bool m_IsVisible{true};
		glm::vec2 m_RenderScale{1,1};
	};
}

