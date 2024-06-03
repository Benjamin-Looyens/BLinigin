#include "PlayerComponents.h"
#include "ServiceLocator.h"
#include "DerivedComponents.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "DerivedEvents.h"
#include "Font.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>

dae::PlayerLogic::PlayerLogic(GameObject* pParent)
	: Component(pParent)
{}

void dae::PlayerLogic::Update()
{
}

void dae::PlayerLogic::LateUpdate()
{
	// TODO : Remove destroyed darts
}

void dae::PlayerLogic::Fire()
{
	dae::Dart dart{ 500.0f, m_pParent->GetWorldPosition(), { 0.0f, -1.0f }, "PlayerDart.tga" };
	
	auto pDart = dart.AddToScene(m_pParent->GetContainingScene());

	m_pDarts.push_back(pDart);

	auto& soundSys = ServiceLocator::GetSoundSystem();
	soundSys.PlaySound(13, 0.3f);
}

#pragma region LivesComponent
dae::LivesComponent::LivesComponent(GameObject* pParent, int lives, glm::vec2 displayPosition, glm::vec2 livesStackDirection)
	: ObservingComponent(pParent), m_NumLives{ lives }, m_DisplayPos{ displayPosition }, m_LivesStackDir{ livesStackDirection }
{
	if (pParent) {
		auto pTextureComp = pParent->GetComponent<dae::TextureComponent>();

		if (pTextureComp) {
			m_pLifeTexture = pTextureComp->GetTexture();
		}
	}
}

void dae::LivesComponent::GainLife()
{
	m_NumLives++;
}

void dae::LivesComponent::Render() const
{
	if (m_pLifeTexture == nullptr) return;

	const auto& widthHeight = m_pLifeTexture->GetSize();

	glm::vec2 offset{ (s_SPACER_BETWEEN_LIVES + static_cast<float>(widthHeight.x)),
						(s_SPACER_BETWEEN_LIVES + static_cast<float>(widthHeight.y))
	};

	for (int index{ }; index < m_NumLives; ++index) {
		Renderer::GetInstance().RenderTexture(*m_pLifeTexture, m_DisplayPos.x + (offset.x * static_cast<float>(index) * m_LivesStackDir.x),
			m_DisplayPos.y + (offset.y * static_cast<float>(index) * m_LivesStackDir.y),
			static_cast<float>(widthHeight.x), static_cast<float>(widthHeight.y));
	}
}

void dae::LivesComponent::SetLifeTexture(Texture2D* pNewTexture)
{
	m_pLifeTexture = pNewTexture;
}

void dae::LivesComponent::OnNotify(const ISubject*, Event event)
{
	switch (event) {
	case Event::PLAYER_DIED:
		LoseLife();
		// TODO : Play sound
		break;
	}
}

void dae::LivesComponent::LoseLife()
{
	m_NumLives--;
}
#pragma endregion LivesComponent

#pragma region ScoreComponent
dae::ScoreComponent::ScoreComponent(dae::GameObject* pParent, glm::vec2 displayPosition, dae::Font* pFont, std::string resourceName)
	: ObservingComponent(pParent), m_DisplayPos{ displayPosition }, m_pFont{ pFont }, m_ResourceFileStr{ resourceName }
{}

void dae::ScoreComponent::AddPoints(int pointsToAdd)
{
	m_Score += pointsToAdd;

	m_TextDirty = true;
}

void dae::ScoreComponent::Update()
{
	// TODO : Use Text Component instead but ughfnhghng
	if (m_TextDirty) {
		auto& resourceManager = ResourceManager::GetInstance();

		if (resourceManager.IsTextureLoaded(m_ResourceFileStr)) resourceManager.UnLoadTexture(m_ResourceFileStr);

		std::string text{ std::to_string(m_Score) };

		const SDL_Color color = { 255,255,255,255 }; // TODO : Implement colors
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), color);
		if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		m_pTextTexture = resourceManager.LoadTexture(texture, m_ResourceFileStr);
		m_TextDirty = false;
	}
}

void dae::ScoreComponent::Render() const
{
	if (m_pTextTexture != nullptr) Renderer::GetInstance().RenderTexture(*m_pTextTexture, m_DisplayPos.x, m_DisplayPos.y);
}

void dae::ScoreComponent::OnNotify(const ISubject*, Event event)
{
	switch (event) {
	case Event::BEE_DIED_FORMATION:
		AddPoints(50);
		break;
	case Event::BEE_DIED_KAMIKAZE:
		AddPoints(100);
		break;
	case Event::BUTTERFLY_DIED_FORMATION:
		AddPoints(80);
		break;
	case Event::BUTTERFLY_DIED_KAMIKAZE:
		AddPoints(160);
		break;
	case Event::BOSS_DIED_FORMATION:
		AddPoints(150);
		break;
	case Event::BOSS_DIED_KAMIKAZE:
		AddPoints(400);
		break;
	}
}
#pragma endregion ScoreComponent