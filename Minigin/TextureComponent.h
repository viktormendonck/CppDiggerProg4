#pragma once
#include <memory>

#include "Component.h"

namespace dae
{
	class Texture2D;
	class Font;
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture, bool rotate = true);
		
		void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		bool m_CanRotate;
	};
}

