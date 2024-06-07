#include "TextInputField.h"

#include "InputManager.h"

dae::TextInputField::TextInputField(GameObject* pParent, std::shared_ptr<Font> pFont)
	: Component(pParent)
{
	m_pTextComponent = std::make_unique<PlainTextComponent>(pParent, "---", pFont, SDL_Color{ 255,255,255 });
	m_pTextComponent->SetRenderOffset({ 10,10 });
	InputManager::GetInstance().textInput.AddListener([this](const std::string& text)
	{
		m_Text += text;
		m_pTextComponent->SetText(m_Text);
	});
}

void dae::TextInputField::Update()
{
	m_pTextComponent->Update();
}

void dae::TextInputField::Render() const
{
	m_pTextComponent->Render();
}
