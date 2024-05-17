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

		explicit SpriteSheetComponent(dae::GameObject* pParent,std::shared_ptr<Texture2D> pTexture, glm::ivec2 size, bool canRotate,float timePerFrame, bool animated, bool repeating);

		void Update() override;
		void Render() const override;
		void SetSprite(glm::ivec2 sprite) { m_CurrentSprite = sprite; }
		glm::ivec2 GetSprite() const { return m_CurrentSprite;}
		glm::ivec2 GetSheetSize() const { return m_SpriteSheetSize;}
		glm::vec2 GetSpriteSize() const { return m_SpriteSize;}
		void ShouldRepeat(bool repeat) { m_IsRepeating = repeat; }
	protected:
		const std::shared_ptr<Texture2D> m_pTexture;
		const glm::ivec2 m_SpriteSheetSize;
		const glm::vec2 m_SpriteSize;
		glm::ivec2 m_CurrentSprite;
		glm::ivec2 m_RenderOffset;
		bool m_CanRotate{};
		float m_TimePerFrame{};
		float m_CurrentTime{};
		bool m_IsAnimated{};
		bool m_IsRepeating{};
		
	};
}
