#include "PlayerComponents.h"
#include "ServiceLocator.h"

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
