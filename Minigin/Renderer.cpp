#include <stdexcept>
#include "Renderer.h"

#include <glm/glm.hpp>

#include "SceneManager.h"
#include "Texture2D.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();


	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();

	const ImGuiIO& io{ ImGui::GetIO() };
	SceneManager::GetInstance().ImGuiUpdate();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, glm::vec2 pos, float rotation, glm::vec2 scale) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(pos.x);
	dst.y = static_cast<int>(pos.y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w = static_cast<int>(dst.w * scale.x);
	dst.h = static_cast<int>(dst.h * scale.y);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, glm::degrees(rotation), nullptr, SDL_FLIP_NONE);
}

/**
 * \brief render a part of a spriteSheet
 * \param texture 
 * \param spriteSheetPos 
 * \param pos 
 * \param rotation 
 * \param size 
 */
void dae::Renderer::RenderSprite(const Texture2D& texture, glm::ivec2 spriteSheetPos, glm::vec2 pos, float rotation,glm::ivec2 size,glm::vec2 scale) const
{
	SDL_Rect src{};
	src.x = spriteSheetPos.x * size.x;
	src.y = spriteSheetPos.y * size.y;
	
	src.w = static_cast<int>(size.x);
	src.h = static_cast<int>(size.y);

	SDL_Rect dst{};
	dst.x = static_cast<int>(pos.x);
	dst.y = static_cast<int>(pos.y);
	dst.w = static_cast<int>(src.w * scale.x);
	dst.h = static_cast<int>(src.h * scale.y);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, glm::degrees(rotation), nullptr, SDL_FLIP_NONE);

}


SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_Renderer; }
