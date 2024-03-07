#include "DerivedComponents.h"

#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"

#pragma region TextComponent
dae::TextComponent::TextComponent(const dae::GameObject * parent, const std::string& text, std::shared_ptr<Font> font)
	: Component(parent), m_NeedsUpdate(true), m_Text(text), m_pFont(std::move(font)), m_pTextTexture(nullptr)
{
	m_ComponentName = "TextComponent";
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	const auto& transform = m_pParent->GetTransform();

	if (m_pTextTexture != nullptr)
	{
		const auto& pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
#pragma endregion TextComponent

#pragma region TextureComponent
dae::TextureComponent::TextureComponent(const dae::GameObject* parent, const std::string& filename)
	: Component(parent)
{
	SetTexture(filename);
	m_ComponentName = "TextureComponent";
}

void dae::TextureComponent::Render() const
{
	const auto& transform = m_pParent->GetTransform();
	const auto& pos = transform.GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
#pragma endregion TextureComponent
