#pragma once
#include <glm/vec2.hpp>

struct SDL_Texture;
namespace dae
{
	class Texture2D final
	{
	public:
		explicit Texture2D(SDL_Texture* texture);

		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) noexcept = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&&) noexcept = delete;

		glm::ivec2 GetSize() const;
		SDL_Texture* GetSDLTexture() const { return m_pTexture; }

	private:
		SDL_Texture* m_pTexture;
	};
}
