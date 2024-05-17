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
#include <algorithm>
#include <math.h>
#include "DerivedEvents.h"
#include "Observer.h"
#include "ServiceLocator.h"

#pragma region TextComponent
dae::TextComponent::TextComponent(dae::GameObject* pParent, const std::string& text, Font* font, const std::string& internalIdentifier)
	: Component(pParent), m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTextTexture(nullptr), m_ResourceFile{ internalIdentifier }
{
	m_ComponentName = "TextComponent";
}

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate) {
		auto& resourceManager = ResourceManager::GetInstance();

		if (resourceManager.IsTextureLoaded(m_ResourceFile)) resourceManager.UnLoadTexture(m_ResourceFile);

		const SDL_Color color = { 255,255,255,255 }; // TODO : Implement colors
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());

		SDL_FreeSurface(surf);
		m_pTextTexture = resourceManager.LoadTexture(texture, m_ResourceFile);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	//const auto& transform = m_pParent->GetWorldPosition();

	if (m_pTextTexture != nullptr)
	{
		const auto& pos = m_pParent->GetWorldPosition();
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
dae::TextureComponent::TextureComponent(dae::GameObject* pParent, const std::string& filename)
	: Component(pParent)
{
	SetTexture(filename);
	m_ComponentName = "TextureComponent";
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
	// TODO : add scaling & rotation compatibility
	const auto& transform = m_pParent->GetWorldTransform();
	const auto& pos = transform.GetTranslation();
	const auto& scale = transform.GetScale();
	const auto& widthHeight = m_pTexture->GetSize();

	//Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, static_cast<float>(widthHeight.x) * scale.x, static_cast<float>(widthHeight.y) * scale.y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
#pragma endregion TextureComponent

#pragma region FPSComponent
dae::FPSComponent::FPSComponent(dae::GameObject* pParent, float displayUpdateInterval)
	: Component(pParent), m_CounterInterval{ displayUpdateInterval }
{
	m_pTextLocation = pParent->GetComponent<dae::TextComponent>();
	if (!m_pTextLocation) {
		std::cerr << "Parent for added FPSComponent does not contain TextComponent! (has no place to output fps data to!)\n";
		Remove(); // Mark self as removed as no TextComponent was found
	}

	m_Counter = m_CounterInterval; // Max out immediately to allow immediate update to FPS data
}

void dae::FPSComponent::Update()
{
	m_Counter += Timer::GetInstance().GetDeltaTime();

	if (m_pTextLocation && m_Counter >= m_CounterInterval) {
		//const float fps{ std::clamp(Timer::GetInstance().GetFPS(), 0.0f, g_MaxFPS) }; // Not currently using g_MaxFPS
		const float fps{ Timer::GetInstance().GetFPS() };
		
		std::stringstream fpsString;
		fpsString << std::fixed << std::setprecision(1) << "FPS: " << fps;
	
		m_pTextLocation->SetText(fpsString.str());
		m_Counter = 0.0f;
	}
}
#pragma endregion FPSComponent

#pragma region WaveMovementYComponent
dae::WaveMovementYComponent::WaveMovementYComponent(GameObject* pParent, float waveHeight, float waveSpeed)
	: Component(pParent), m_WaveHeight{ waveHeight }, m_WaveSpeed{ waveSpeed }
{
	m_Origin = pParent->GetLocalPosition();
}

void dae::WaveMovementYComponent::Update()
{
	m_WaveTime += Timer::GetInstance().GetDeltaTime() * m_WaveSpeed;

	m_WaveTime = static_cast<float>(std::fmod(m_WaveTime, M_PI * 2));

	glm::vec2 wavePos{ m_Origin };

	wavePos.y += m_WaveHeight * (std::sinf(m_WaveTime));

	m_pParent->SetLocalTranslation(wavePos);
}
#pragma endregion WaveMovementYComponent

#pragma region LivesComponent
dae::LivesComponent::LivesComponent(GameObject* pParent, int lives)
	: Component(pParent), m_NumLives{ lives }
{}

void dae::LivesComponent::GainLife()
{
	m_NumLives++;
}

void dae::LivesComponent::LoseLife()
{
	m_NumLives--;
}
#pragma endregion LivesComponent

#pragma region DataDisplayComponent
dae::DisplayDataComponent::DisplayDataComponent(GameObject* pParent, std::string dataPrefix, std::string startData)
	: Component(pParent), m_DataPrefix{ dataPrefix }
{
	m_pTextLocation = pParent->GetComponent<dae::TextComponent>();
	if (!m_pTextLocation) {
		std::cerr << "Parent for added LivesCounterComponent does not contain TextComponent! (has no place to output lives counter to!)\n";
		Remove(); // Mark self as removed as no TextComponent was found
	}
	else {
		UpdateDisplay(startData);
	}
}

void dae::DisplayDataComponent::UpdateDisplay(std::string dataStr)
{
	std::stringstream dataString;
	dataString << m_DataPrefix << dataStr;

	m_pTextLocation->SetText(dataString.str());
}
#pragma endregion DataDisplayComponent

dae::LivesObservingComponent::LivesObservingComponent(GameObject* pParent, GameObject* pDisplay)
	: ObservingComponent(pParent), m_pDisplayObject(pDisplay)
{}

void dae::LivesObservingComponent::OnNotify(const ISubject* pSubject, Event event)
{
	// Attempts to get the player GameObject from pSubject that is calling the event
	const GameObject* pPlayerObj = nullptr;
	if (pSubject) {
		try {
			pPlayerObj = dynamic_cast<const Component*>(pSubject)->GetParent();
		}
		catch (...) {
			std::cout << "[ScoreObservingComponent] Getting player failed!\n";
			return;
		}
	}

	dae::LivesComponent* pLivesComp = nullptr;
	if (pPlayerObj) pLivesComp = pPlayerObj->GetComponent<dae::LivesComponent>();

	int prevLives{};
	if (pLivesComp) prevLives = pLivesComp->GetLifeCount();

	dae::DisplayDataComponent* pDataComp = nullptr;
	if (m_pDisplayObject) pDataComp = m_pDisplayObject->GetComponent<dae::DisplayDataComponent>();
	
	bool doUpdate{ false };

	switch (event) {
	case Event::PLAYER_DIED:
		if (pLivesComp) pLivesComp->LoseLife();
		//ServiceLocator::GetSoundSystem().PlaySound(9, 0.4f);
		doUpdate = true;
		break;
	}

	if (!doUpdate) return;

	if (pDataComp && pLivesComp) {
		if (prevLives != pLivesComp->GetLifeCount()) {
			pDataComp->UpdateDisplay(std::to_string(pLivesComp->GetLifeCount()));
		}
	}
#ifndef NDEBUG
	else {
		if (!pDataComp) std::cout <<  "[LivesObservingComponent] Data Displaying Component is nullptr!\n";
		if (!pLivesComp) std::cout << "[LivesObservingComponent] Lives Component is nullptr!\n";
	}
#endif
}

dae::ScoreObservingComponent::ScoreObservingComponent(GameObject* pParent, GameObject* pDisplay)
	: ObservingComponent(pParent), m_pDisplayObject(pDisplay)
{}

void dae::ScoreObservingComponent::OnNotify(const ISubject* pSubject, Event event)
{
	// Attempts to get the player GameObject from pSubject that is calling the event
	const GameObject* pPlayerObj = nullptr;
	if (pSubject) {
		try {
			pPlayerObj = dynamic_cast<const Component*>(pSubject)->GetParent();
		}
		catch (...) {
			std::cout << "[ScoreObservingComponent] Getting player failed!\n";
			return;
		}
	}

	dae::ScoreComponent* pScoreComp = nullptr;
	if (pPlayerObj) pScoreComp = pPlayerObj->GetComponent<dae::ScoreComponent>();

	int prevScore{};
	if (pScoreComp) prevScore = pScoreComp->GetScore();

	dae::DisplayDataComponent* pDataComp = nullptr;
	if (m_pDisplayObject) pDataComp = m_pDisplayObject->GetComponent<dae::DisplayDataComponent>();

	bool doUpdate{ false };

	switch (event) {
	case Event::BIG_ENEMY_DIED:
		if (pScoreComp) pScoreComp->AddPoints(500);
		//ServiceLocator::GetSoundSystem().PlaySound(8, 0.4f);
		doUpdate = true;
		break;
	case Event::SMALL_ENEMY_DIED:
		if (pScoreComp) pScoreComp->AddPoints(100);
		//ServiceLocator::GetSoundSystem().PlaySound(10, 0.4f);
		doUpdate = true;
		break;
	}

	if (!doUpdate) return;

	if (pDataComp && pScoreComp) {
		if (prevScore != pScoreComp->GetScore()) {
			pDataComp->UpdateDisplay(std::to_string(pScoreComp->GetScore()));
		}
	}
#ifndef NDEBUG
	else {
		if (!pDataComp) std::cout <<  "[ScoreObservingComponent] Data Displaying Component is nullptr!\n";
		if (!pScoreComp) std::cout << "[ScoreObservingComponent] Score Component is nullptr!\n";
	}
#endif
}
