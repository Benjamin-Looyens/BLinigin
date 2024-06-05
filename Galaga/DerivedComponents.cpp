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
#include "TextureComponents.h"

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
	m_Counter += BL_TIMER.GetDeltaTime();

	if (m_pTextLocation && m_Counter >= m_CounterInterval) {
		//const float fps{ std::clamp(BL_TIMER.GetFPS(), 0.0f, g_MaxFPS) }; // Not currently using g_MaxFPS
		const float fps{ BL_TIMER.GetFPS() };
		
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
	m_WaveTime += BL_TIMER.GetDeltaTime() * m_WaveSpeed;

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
	m_pParent->SetLocalTranslation(BL_INPUT.GetMousePos());
}

dae::ConstantMovementComponent::ConstantMovementComponent(GameObject* pParent, float speed, glm::vec2 movDir)
	: Component(pParent), m_Speed{ speed }, m_MovDir{ movDir }
{}

void dae::ConstantMovementComponent::Update()
{
	const float& deltaTime = BL_TIMER.GetDeltaTime();

	m_pParent->AddLocalTranslation(deltaTime * m_Speed * m_MovDir);
}

dae::TimedDestroyComponent::TimedDestroyComponent(GameObject* pParent, float timeTillDestroy)
	: Component(pParent), m_TimeTillDestroy{ timeTillDestroy }
{}

void dae::TimedDestroyComponent::Update()
{
	m_ElapsedTime += BL_TIMER.GetDeltaTime();

	if (m_ElapsedTime >= m_TimeTillDestroy) m_pParent->Destroy();
}


dae::IncrementingScaleComponent::IncrementingScaleComponent(GameObject* pParent, float incrementPerSecond)
	: Component(pParent), m_IncrPerSecond{ incrementPerSecond }
{}

void dae::IncrementingScaleComponent::Update()
{
	glm::vec2 curScale = m_pParent->GetLocalScale();
	
	curScale.y += m_IncrPerSecond * BL_TIMER.GetDeltaTime(); 
	curScale.x += m_IncrPerSecond * BL_TIMER.GetDeltaTime(); 

	m_pParent->SetLocalScale(curScale);
}
