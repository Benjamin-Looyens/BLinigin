#include "PlayerComponents.h"
#include "ServiceLocator.h"
#include "DerivedComponents.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "DerivedEvents.h"
#include "Font.h"
#include "ResourceManager.h"
#include <SDL_ttf.h>
#include <algorithm>
#include "GameObject.h"
#include "DartLogic.h"
#include "TextureComponents.h"
#include "Timer.h"

dae::PlayerLogic::PlayerLogic(GameObject* pParent)
	: SubjectComponent(pParent)
{}

void dae::PlayerLogic::Update()
{
	if (m_FireCooldown >= 0) m_FireCooldown -= BL_TIMER.GetDeltaTime();
	if (m_InvulFrames >= 0)	 m_InvulFrames -= BL_TIMER.GetDeltaTime();
}

void dae::PlayerLogic::LateUpdate()
{

}

void dae::PlayerLogic::Fire()
{
	if (m_FireCooldown <= 0) {
		dae::Dart dart{ 500.0f, m_pParent->GetWorldPosition(), { 0.0f, -1.0f }, "PlayerDart.png" };
		
		auto pDart = dart.AddToScene(m_pParent->GetContainingScene());
		auto parentPos = m_pParent->GetWorldPosition();
		auto parentSize = m_pParent->GetComponent<dae::TextureComponent>()->GetTextureSize() * m_pParent->GetWorldScale();
		auto dartSize = pDart->GetComponent<dae::TextureComponent>()->GetTextureSize() * pDart->GetWorldScale();

		glm::vec2 dartPos{ (parentPos.x + ((parentSize.x - dartSize.x) / 2)),
						   (parentPos.y) };
		pDart->SetLocalTranslation(dartPos);

		// TODO : rework the observer assignment here to be less coupled
		auto pDartLogic = pDart->GetComponent<dae::DartLogic>();
		if (pDartLogic) {
			auto pScore = m_pParent->GetComponent<dae::ScoreComponent>();
			if (pScore) {
				pDartLogic->AddObserver(pScore);
			}
		}

		NotifyObservers(Event::PLAYER_SHOOT);

		BL_SOUND.PlaySound(13, 0.3f);

		m_DartsFired++;
		m_FireCooldown = s_MAX_FIRE_COOLDOWN;
	}
}

void dae::PlayerLogic::Die()
{
	if (m_InvulFrames <= 0) {
		NotifyObservers(Event::PLAYER_DIED);
		m_InvulFrames = s_MAX_INVUL_FRAMES;

#ifndef NDEBUG
		std::cout << "Player lost life!\n";
#endif
	}
}

#pragma region LivesComponent
dae::LivesComponent::LivesComponent(GameObject* pParent, int lives, glm::vec2 displayPosition, glm::vec2 livesStackDirection)
	: ObservingSubjectComponent(pParent), m_NumLives{ lives }, m_DisplayPos{ displayPosition }, m_LivesStackDir{ livesStackDirection }
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
		BL_RENDERER.RenderTexture(*m_pLifeTexture, m_DisplayPos.x + (offset.x * static_cast<float>(index) * m_LivesStackDir.x),
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
		BL_SOUND.PlaySound(9, 0.4f);
		break;
	case Event::ONE_UP:
		GainLife();
		BL_SOUND.PlaySound(8, 0.4f);
	}
}

void dae::LivesComponent::LoseLife()
{
	m_NumLives--;

	// TODO : add check for other player
	if (m_NumLives == -1) {
		NotifyObservers(Event::GAME_OVER);

	}
}
#pragma endregion LivesComponent

#pragma region ScoreComponent
const std::string dae::ScoreComponent::s_ONE_UP_TEXT = "1UP";
dae::Texture2D* dae::ScoreComponent::s_pOneUpTextTexture = nullptr;
float dae::ScoreComponent::s_OneUpTimer = 0.0f;
bool dae::ScoreComponent::s_DisplayOneUp = true;
int dae::ScoreComponent::s_NumScoreCounters = 0;

dae::ScoreComponent::ScoreComponent(dae::GameObject* pParent, glm::vec2 displayPosition, dae::Font* pFont, std::string resourceName)
	: ObservingSubjectComponent(pParent), m_DisplayPos{ displayPosition }, m_pFont{ pFont }, m_ResourceFileStr{ resourceName }
{
	if (!s_pOneUpTextTexture) {
		const SDL_Color color = { 255, 0, 0, 255 };
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), s_ONE_UP_TEXT.c_str(), color);
		if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(BL_RENDERER.GetSDLRenderer(), surf);
		if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		s_pOneUpTextTexture = BL_RESOURCE.LoadTexture(texture, "1UP_Text");
	}

	s_NumScoreCounters++;
}

dae::ScoreComponent::~ScoreComponent()
{
	s_NumScoreCounters--;
}

void dae::ScoreComponent::AddPoints(int pointsToAdd)
{
	m_Score += pointsToAdd;

	m_TextDirty = true;

	if (m_Score >= s_NUM_POINTS_FOR_1UP) {
		if (m_Score / s_NUM_POINTS_FOR_1UP > m_NumTimesOneUp) {
			NotifyObservers(Event::ONE_UP);
			m_NumTimesOneUp++;
		}
	}

	NotifyObservers(dae::Event::PLAYER_SHOT_ENEMY);
}

void dae::ScoreComponent::Update()
{
	if (m_TextDirty) {
		auto& resourceManager = BL_RESOURCE;

		if (resourceManager.IsTextureLoaded(m_ResourceFileStr)) resourceManager.UnLoadTexture(m_ResourceFileStr);

		std::string text{ std::to_string(m_Score) };

		const SDL_Color color = { 255,255,255,255 };
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), text.c_str(), color);
		if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());

		auto texture = SDL_CreateTextureFromSurface(BL_RENDERER.GetSDLRenderer(), surf);
		if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		m_pTextTexture = resourceManager.LoadTexture(texture, m_ResourceFileStr);
		m_TextDirty = false;
	}

	if (s_OneUpTimer >= s_MAX_ONE_UP_TIMER) {
		s_DisplayOneUp = !s_DisplayOneUp;
		s_OneUpTimer = 0.0f;
	}
	s_OneUpTimer += BL_TIMER.GetDeltaTime() / s_NumScoreCounters;
}

void dae::ScoreComponent::Render() const
{
	if (m_pTextTexture != nullptr) {
		const auto& parentScale = m_pParent->GetWorldScale();
		const auto& textSize = m_pTextTexture->GetSize();

		glm::vec2 realMainPos = m_DisplayPos;

		realMainPos.x -= ((textSize.x / 2) * parentScale.x) / 2; // always keep centered

		BL_RENDERER.RenderTexture(*m_pTextTexture, realMainPos.x, realMainPos.y);

		if (s_pOneUpTextTexture != nullptr) {

			if (s_DisplayOneUp) {
				glm::vec2 real1upPos = m_DisplayPos;

				real1upPos.x -= ((s_pOneUpTextTexture->GetSize().x / 2) * parentScale.x) / 2; // always keep centered
				real1upPos.y -= textSize.y;

				BL_RENDERER.RenderTexture(*s_pOneUpTextTexture, real1upPos.x, real1upPos.y);
			}
		}
	}
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