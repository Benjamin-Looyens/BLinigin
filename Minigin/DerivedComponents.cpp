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
#include "DerivedObservers.h"
#include "Events.h"

#pragma region TextComponent
dae::TextComponent::TextComponent(dae::GameObject * pParent, const std::string& text, std::shared_ptr<Font> font)
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

void dae::TextureComponent::Render() const
{
	//const auto& transform = m_pParent->GetLocalTransform();
	const auto& pos = m_pParent->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
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

	glm::vec3 wavePos{ m_Origin };

	wavePos.y += m_WaveHeight * (std::sinf(m_WaveTime));

	m_pParent->SetLocalTranslation(wavePos);
}
#pragma endregion WaveMovementYComponent

#pragma region ObserverSubjectComponent
dae::ObserverSubjectComponent::ObserverSubjectComponent(GameObject* pParent)
	: Component(pParent)
{}

void dae::ObserverSubjectComponent::AddObserver(Observer* pObserverToAdd)
{
	m_pObservers.push_back(pObserverToAdd);
}

void dae::ObserverSubjectComponent::RemoveObserver(Observer* pObserverToRemove) 
{
	m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), pObserverToRemove), m_pObservers.end());
}

void dae::ObserverSubjectComponent::NotifyObservers(Event& eventToNotify)
{
	for (auto& pObserver : m_pObservers) 
		pObserver->OnNotify(m_pParent, eventToNotify);
}
#pragma endregion ObserverSubjectComponent

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