#include "TextureComponents.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "GameObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>

#pragma region TextureComponent
dae::TextureComponent::TextureComponent(dae::GameObject* pParent, const std::string& filename)
	: Component(pParent)
{
	SetTexture(filename);
}

float dae::TextureComponent::GetTextureWidth() const
{
	return static_cast<float>(m_pTexture->GetSize().x);
}

float dae::TextureComponent::GetTextureHeight() const
{
	return static_cast<float>(m_pTexture->GetSize().y);
}


glm::vec2 dae::TextureComponent::GetTextureSize() const
{
	glm::ivec2 iVec = m_pTexture->GetSize();

	return { static_cast<float>(iVec.x), static_cast<float>(iVec.y) };
}

void dae::TextureComponent::Render() const
{
	// TODO : add rotation compatibility
	const auto& transform = m_pParent->GetWorldTransform();
	const auto& pos = transform.GetTranslation();
	const auto& scale = transform.GetScale();
	const auto& widthHeight = m_pTexture->GetSize();
	const auto& scaledWidthHeight = static_cast<glm::ivec2>(static_cast<glm::vec2>(widthHeight) * scale); // HACK : holy static_cast 
	//const auto& iPos = static_cast<glm::ivec2>(pos);

	//BL_RENDERER.RenderTexture(*m_pTexture, pos.x, pos.y);
	BL_RENDERER.RenderTexture(*m_pTexture, pos.x, pos.y, static_cast<float>(scaledWidthHeight.x), static_cast<float>(scaledWidthHeight.y));
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = BL_RESOURCE.LoadTexture(filename);
}
#pragma endregion TextureComponent

#pragma region TextComponent
dae::TextComponent::TextComponent(dae::GameObject* pParent, const std::string& text, Font* font, const std::string& internalIdentifier)
	: Component(pParent), m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTextTexture(nullptr), m_ResourceFile{ internalIdentifier }
{}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate) {
		auto& resourceManager = BL_RESOURCE;

		if (resourceManager.IsTextureLoaded(m_ResourceFile)) resourceManager.UnLoadTexture(m_ResourceFile);

		const SDL_Color color = { 255,255,255,255 }; // TODO : Implement colors
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(BL_RENDERER.GetSDLRenderer(), surf);
		if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		m_pTextTexture = resourceManager.LoadTexture(texture, m_ResourceFile);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_pTextTexture != nullptr)
	{
		const auto& pos = m_pParent->GetWorldPosition();
		BL_RENDERER.RenderTexture(*m_pTextTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
#pragma endregion TextComponent

#pragma region AnimatedTextureComponent
dae::AnimatedTextureComponent::AnimatedTextureComponent(dae::GameObject* pParent, const std::string& fileName, int numFrames, float fps, int numLayers)
	: dae::TextureComponent(pParent, fileName), m_NumFrames{ numFrames }, m_FPS{ fps }, m_NumLayers{ numLayers }
{
	m_FrameWidthHeight = { GetTextureSize().x / numFrames, GetTextureSize().y / numLayers };
}

void dae::AnimatedTextureComponent::Update()
{
	if (!m_DoAnimation) return;

	float deltaTime = BL_TIMER.GetDeltaTime();

	m_ElapsedTime += deltaTime;
	const float frameAdvanceTime = static_cast<float>(m_NumFrames) / m_FPS;

	if (frameAdvanceTime <= m_ElapsedTime) {
		m_CurFrame = (m_CurFrame + 1) % m_NumFrames;
		m_ElapsedTime = m_ElapsedTime - frameAdvanceTime;
	}
}

void dae::AnimatedTextureComponent::Render() const
{
	const auto& transform = m_pParent->GetWorldTransform();
	const auto& pos = transform.GetTranslation();
	const auto& scale = transform.GetScale();
	const auto& iPos = static_cast<glm::ivec2>(pos);

	SDL_Rect srcRect = { m_CurFrame * m_FrameWidthHeight.x, m_CurLayer * m_FrameWidthHeight.y, m_FrameWidthHeight.x, m_FrameWidthHeight.y };
	SDL_Rect dstRect = { iPos.x, iPos.y,
						 static_cast<int>(m_FrameWidthHeight.x * scale.x), static_cast<int>(m_FrameWidthHeight.y * scale.y) };

	BL_RENDERER.RenderTexture(*m_pTexture, srcRect, dstRect);
}

glm::vec2 dae::AnimatedTextureComponent::GetAppliedTextureSize() const
{
	glm::vec2 fullSize = GetTextureSize();
	glm::vec2 appliedSize = { fullSize.x / m_NumFrames, fullSize.y / m_NumLayers };

	return appliedSize;
}

float dae::AnimatedTextureComponent::GetTimeForLoop() const
{
	return (static_cast<float>(m_NumFrames) / m_FPS) * static_cast<float>(m_NumFrames);
}

void dae::AnimatedTextureComponent::NextLayer()
{
	m_CurLayer = (m_CurLayer + 1) % m_NumLayers;
}
#pragma endregion AnimatedTextureComponent