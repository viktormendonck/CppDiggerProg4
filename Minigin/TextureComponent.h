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
		explicit TextureComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture);
		
		void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		
	};
}

