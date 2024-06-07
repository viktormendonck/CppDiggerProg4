#pragma once
#include <memory>
#include <string>
#include <glm/vec2.hpp>

#include "Component.h"
#include "Font.h"
#include "NumberDisplay.h"
#include "PlainTextComponent.h"
#include "Renderer.h"

namespace dae
{
	class SingleScoreDisplay final : public Component
	{
	public:
		SingleScoreDisplay(GameObject* pParent,const glm::vec2 renderOffset, const std::string& name,int score, const std::shared_ptr<Font>& font, const std::shared_ptr<Texture2D>& numberTexture);

		void Render() const override;
		void SetName(const std::string& name);
		void SetScore(int score) { m_Score = score; } 
	private:

		glm::vec2 m_RenderOffset;
		std::string m_Name;
		int m_Score;
		std::vector <uint8_t> m_Numbers;

		std::shared_ptr<PlainTextComponent> m_pTextRenderer;

		std::shared_ptr<Font> m_pFont;
		std::unique_ptr<NumberDisplay> m_pNumberDisplay;
		void RenderNumber() const;

		void UpdateNumber(int num);
		
		std::shared_ptr<Texture2D> m_pNumberTexture;
		glm::ivec2 m_SpriteSize{};
		glm::vec2 m_RenderScale{};
	};
};
