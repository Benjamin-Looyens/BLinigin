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
#include "InputManager.h"

#pragma region TextComponent
dae::TextComponent::TextComponent(dae::GameObject* pParent, const std::string& text, Font* font, const std::string& internalIdentifier)
	: Component(pParent), m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTextTexture(nullptr), m_ResourceFile{ internalIdentifier }
{}

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

	//Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, static_cast<float>(scaledWidthHeight.x), static_cast<float>(scaledWidthHeight.y));
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
#pragma endregion TextureComponent

dae::AnimatedTextureComponent::AnimatedTextureComponent(dae::GameObject* pParent, const std::string& fileName, int numFrames, float fps)
	: dae::TextureComponent(pParent, fileName), m_NumFrames{ numFrames }, m_FPS{ fps }
{
	m_FrameWidthHeight = { GetTextureSize().x / numFrames, GetTextureSize().y };
}

void dae::AnimatedTextureComponent::Update()
{
	float deltaTime = Timer::GetInstance().GetDeltaTime();

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

	SDL_Rect srcRect = { m_CurFrame * m_FrameWidthHeight.x, 0, m_FrameWidthHeight.x, m_FrameWidthHeight.y };
	SDL_Rect dstRect = { iPos.x, iPos.y,
						 static_cast<int>(m_FrameWidthHeight.x * scale.x), static_cast<int>(m_FrameWidthHeight.y * scale.y) };

	Renderer::GetInstance().RenderTexture(*m_pTexture, srcRect, dstRect);
}

glm::vec2 dae::AnimatedTextureComponent::GetAppliedTextureSize() const
{
	glm::vec2 fullSize = GetTextureSize();
	glm::vec2 appliedSize = { fullSize.x / m_NumFrames, fullSize.y };

	return appliedSize;
}

float dae::AnimatedTextureComponent::GetTimeForLoop() const
{
	return (static_cast<float>(m_NumFrames) / m_FPS) * static_cast<float>(m_NumFrames);
}


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

dae::FollowMouseComponent::FollowMouseComponent(GameObject* pParent)
	: Component(pParent)
{}

void dae::FollowMouseComponent::Update()
{
	m_pParent->SetLocalTranslation(InputManager::GetInstance().GetMousePos());
}

dae::ConstantMovementComponent::ConstantMovementComponent(GameObject* pParent, float speed, glm::vec2 movDir)
	: Component(pParent), m_Speed{ speed }, m_MovDir{ movDir }
{}

void dae::ConstantMovementComponent::Update()
{
	const float& deltaTime = Timer::GetInstance().GetDeltaTime();

	m_pParent->AddLocalTranslation(deltaTime * m_Speed * m_MovDir);
}

dae::TimedDestroyComponent::TimedDestroyComponent(GameObject* pParent, float timeTillDestroy)
	: Component(pParent), m_TimeTillDestroy{ timeTillDestroy }
{}

void dae::TimedDestroyComponent::Update()
{
	m_ElapsedTime += Timer::GetInstance().GetDeltaTime();

	if (m_ElapsedTime >= m_TimeTillDestroy) m_pParent->Destroy();
}
