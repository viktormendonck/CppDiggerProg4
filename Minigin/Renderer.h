#pragma once
#include <SDL.h>
#include <glm/vec2.hpp>

#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, glm::vec2 pos, float rotation, glm::vec2 scale) const;
		void RenderSprite(const Texture2D& texture, glm::ivec2 spriteSheetPos, glm::vec2 pos,float rotation, glm::ivec2 size, glm::vec2 scale) const;
		void RenderRect(glm::vec2 pos, glm::vec2 size, SDL_Color color) const;
		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
	};
}