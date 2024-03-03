#pragma once
#include <memory>
#include <SDL_pixels.h>
#include <string>

#include "Component.h"

namespace dae
{
	class Texture2D;
	class Font;
	class PlainTextComponent : public Component
	{
	public:
		explicit PlainTextComponent(GameObject* pParent, std::string text , std::shared_ptr<Font> m_pFont, SDL_Color color);

		void Update() override;
		void Render() const override;
		void SetText(const std::string& text);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<Font> m_pFont;
		std::string m_Text;
		SDL_Color m_Color;
		bool m_Dirty = true;

		void CreateTexture();
	};
}

