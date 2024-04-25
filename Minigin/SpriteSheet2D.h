#pragma once
#include <SDL_render.h>
#include <glm/vec2.hpp>

namespace dae
{
	class SpriteSheet2D
	{
	public:
		SpriteSheet2D(SDL_Texture* pTexture, glm::ivec2 spriteSheetSize)
			: m_pTexture(pTexture)
			, m_SpriteSheetSize(spriteSheetSize)
		{
			int w, h;
			SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
			m_SpriteSize = { w / spriteSheetSize.x,h / spriteSheetSize.y };
			m_CurrentSpriteID = { 0,0 };
		}
		const SDL_Texture* GetSDLTexture() const { return m_pTexture; };

		void SetSprite(glm::ivec2 sprite) { m_CurrentSpriteID = sprite; }
		glm::ivec2 GetSprite() {return m_CurrentSpriteID; }
		glm::ivec2 GetSize() const { return m_SpriteSize; }
		void IncrementFrame(){++m_CurrentSpriteID.x %= m_SpriteSheetSize.x;}
		void IncrementSet(){++m_CurrentSpriteID.y %= m_SpriteSheetSize.y;}
	private:
		const SDL_Texture* m_pTexture;

		const glm::ivec2 m_SpriteSheetSize;
		glm::ivec2 m_SpriteSize;
		glm::ivec2 m_CurrentSpriteID;
	};
}
