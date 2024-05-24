#pragma once
#include <functional>

#include "Signal.h"
#include "SpriteSheetComponent.h"
namespace dae
{
	class NumberDisplay final : public Component
	{
	public:
		NumberDisplay(GameObject* pParent,Signal<int>* scoreUpdatedSignal, std::shared_ptr<Texture2D> pTexture, glm::vec2 renderScale = glm::vec2(1,1),glm::ivec2 renderOffset = glm::ivec2(0,0));

		void Render() const override;
	private:
		void UpdateNumber(int num);
		std::vector<uint8_t> m_Numbers{ static_cast<uint8_t>(0)};

		std::shared_ptr<Texture2D> m_pTexture;
		glm::ivec2 m_SpriteSize{};
		glm::vec2 m_RenderScale{};
		glm::vec2 m_RenderOffset{ 0.f,0.f };
	};

}