#pragma once
#include <memory>

#include "Component.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture, bool rotate = true);
		
		void Render() const override;
		void SetVisibility(bool isVisible) { m_IsVisible = isVisible; }

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		bool m_CanRotate{};
		bool m_IsVisible{true};
	};
}

