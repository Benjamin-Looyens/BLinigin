#include "DerivedComponents.h"

#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Timer.h"
#include <sstream>

#pragma region TextComponent
dae::TextComponent::TextComponent(const dae::GameObject * pParent, const std::string& text, std::shared_ptr<Font> font)
	: Component(pParent), m_NeedsUpdate(true), m_Text(text), m_pFont(std::move(font)), m_pTextTexture(nullptr)
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
dae::TextureComponent::TextureComponent(const dae::GameObject* pParent, const std::string& filename)
	: Component(pParent)
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

#pragma region FPSComponent
dae::FPSComponent::FPSComponent(const dae::GameObject* pParent, float displayUpdateInterval)
	: Component(pParent), m_CounterInterval{ displayUpdateInterval }
{
	m_pTextLocation = pParent->GetComponent<dae::TextComponent>();
	if (!m_pTextLocation) {
		std::cerr << "Parent for added FPSComponent does not contain TextComponent! (has no place to output fps data to!)\n";
		Remove(); // Mark self as removed as no TextComponent was found
	}
}

void dae::FPSComponent::Update()
{
	m_Counter += Timer::GetInstance().GetDeltaTime();

	if (m_pTextLocation && m_Counter >= 0.3f) {
		std::stringstream fpsString;
		fpsString << std::fixed << "FPS: " << std::setprecision(2) << Timer::GetInstance().GetFPS();

		m_pTextLocation->SetText(fpsString.str());
		m_Counter = 0.0f;
	}
}
#pragma endregion FPSComponent