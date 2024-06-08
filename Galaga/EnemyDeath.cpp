#include "EnemyDeath.h"
#include "GameObject.h"
#include "Scene.h"
#include "DerivedComponents.h"
#include "TextureComponents.h"

dae::GameObject* dae::Death::AddToScene(Scene* pSceneToAddTo)
{
	auto pDeathObject = pSceneToAddTo->AddGameObject();

	auto pTextureComp = pDeathObject->AddComponent<dae::AnimatedTextureComponent>(std::string{ "EnemyDeath.png" }, 5, 90.0f);
	float timeForEnd = pTextureComp->GetTimeForLoop();
	pDeathObject->AddComponent<dae::TimedDestroyComponent>(timeForEnd);

	return pDeathObject;
}
