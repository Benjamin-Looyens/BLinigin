#include "StageManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "DerivedComponents.h"
#include "StageComponents.h"
#include <SDL.h>
#include "Renderer.h"

dae::StageManager::StageManager(std::vector<IObserver*>& enemyObservers)
{
    m_EnemyObservers = enemyObservers;
}

dae::GameObject* dae::StageManager::AddToScene(dae::Scene* pSceneToAddTo)
{
    auto pStageManagerObject = pSceneToAddTo->AddGameObject();
    pStageManagerObject->AddComponent<dae::StageManagerLogic>(  dae::Enemy{ glm::vec2{}, "Boss.png",        300.0f, 50.0f, m_EnemyObservers, 3.0f, 2, 2, 14, 15, dae::Event::BOSS_DIED_FORMATION, dae::Event::BOSS_DIED_KAMIKAZE},
                                                                dae::Enemy{ glm::vec2{}, "Butterfly.png",   300.0f, 50.0f, m_EnemyObservers, 3.0f, 2, 1, 17, 17, dae::Event::BUTTERFLY_DIED_FORMATION, dae::Event::BUTTERFLY_DIED_KAMIKAZE },
                                                                dae::Enemy{ glm::vec2{}, "Bee.png",         300.0f, 50.0f, m_EnemyObservers, 3.0f, 2, 1, 16, 16, dae::Event::BEE_DIED_FORMATION, dae::Event::BEE_DIED_KAMIKAZE });
    
    glm::vec2 windowSize = BL_RENDERER.GetWindowSize();

    pStageManagerObject->SetLocalTranslation({ windowSize.x / 2, 100 });

    return pStageManagerObject;
}
