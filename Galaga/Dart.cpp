#include "Dart.h"
#include "Scene.h"
#include "DerivedComponents.h"
#include "CollisionComponents.h"
#include "DartLogic.h"

dae::Dart::Dart(float speed, glm::vec2 startPos, glm::vec2 movDir, std::string spriteLocation)
    : m_Speed{ speed }, m_Startpos{ startPos }, m_MovDirection{ movDir }, m_SpriteLocation{ spriteLocation }
{}

dae::GameObject* dae::Dart::AddToScene(Scene* pSceneToAddTo)
{
    auto pDart = pSceneToAddTo->AddGameObject();

    auto pTextureComp = pDart->AddComponent<dae::TextureComponent>(m_SpriteLocation);
    pDart->AddComponent<dae::CollisionComponent>(pTextureComp->GetTextureSize());
    pDart->AddComponent<dae::ConstantMovementComponent>(m_Speed, m_MovDirection);
    pDart->AddComponent<dae::DartLogic>();
    pDart->SetLocalTranslation(m_Startpos);
    pDart->SetLocalScale(2.0f, 2.0f);

    return pDart;
}