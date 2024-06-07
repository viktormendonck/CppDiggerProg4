#pragma once
#include <string>

#include "Component.h"
#include "PlainTextComponent.h"

namespace dae
{
	class TextInputField final : public Component
	{
	public:
		TextInputField(GameObject* pParent, std::shared_ptr<Font> pFont);
		void Update() override;
		void Render() const override;
		std::string GetText() const { return m_Text; }
	private:
		std::unique_ptr<PlainTextComponent> m_pTextComponent;
		std::string m_Text;

	};

}
