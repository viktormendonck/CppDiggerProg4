#pragma once
#include <string>
#include <glm/vec2.hpp>

#include "Component.h"
#include "memory"
namespace dae
{
	class Texture2D;
	class SpriteSheetComponent : public Component
	{
	public:

		explicit SpriteSheetComponent(GameObject* pParent, std::shared_ptr<Texture2D> pTexture, glm::ivec2 size, bool canRotate, float timePerFrame, bool animated= true, bool repeating= true ,glm::ivec2 startPos = glm::ivec2(0,0));

		void Update() override;
		void Render() const override;
		void SetSprite(glm::ivec2 sprite) { m_CurrentSprite = sprite; }
		void SetRenderScale(glm::vec2 scale) { m_RenderScale = scale; }
		glm::ivec2 GetSprite() const { return m_CurrentSprite;}
		glm::ivec2 GetSheetSize() const { return m_SpriteSheetSize;}
		glm::vec2 GetSpriteSize() const { return m_SpriteSize; }
		glm::vec2 GetRenderScale() const { return m_RenderScale; }
		glm::vec2 GetRenderOffset() const { return m_RenderOffset; }
		void SetRenderOffset(glm::vec2 offset) { m_RenderOffset = offset; }

		
		void ShouldRepeat(bool repeat) { m_IsRepeating = repeat; }
		void SetVisible(bool isVisible) { m_IsVisible = isVisible; }
	protected:
		const std::shared_ptr<Texture2D> m_pTexture;
		const glm::ivec2 m_SpriteSheetSize;
		const glm::vec2 m_SpriteSize;
		glm::vec2 m_RenderScale{ 1,1 };
		glm::vec2 m_RenderOffset{0.f,0.f};
		glm::ivec2 m_CurrentSprite;
		bool m_CanRotate{};
		float m_TimePerFrame{};
		float m_CurrentTime{};
		bool m_IsAnimated{};
		bool m_IsRepeating{};
		bool m_IsVisible{ true };
		
	};
}
