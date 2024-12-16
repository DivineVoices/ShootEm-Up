#include "pch.h"

#include "Explosion.h"

#include "SampleScene.h"

#include "Windows.h"

#include "EnemyEntity.h"

ExplosiontEntity::ExplosiontEntity()
{
    SetTag(SampleScene::Tag::BULLET);
}

void ExplosiontEntity::OnUpdate()
{
    GoToDirection(10000, GetPosition().y, 850);

    if (ToDestroy()) return;

    Scene* scene = GetScene();
    if (scene == nullptr) return;

    sf::Vector2f position = GetPosition();
    if (position.x > 1280 || position.x < 0 || position.y > 720 || position.y < 0) {
        Destroy();
    }
}


void ExplosiontEntity::OnCollision(Entity* pCollidedWith)
{
    if (pCollidedWith == nullptr) return;

    if (pCollidedWith->IsTag(SampleScene::Tag::ENNEMIES))
    {
        EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(pCollidedWith);
        if (enemy != nullptr)
        {
            enemy->TakeDamage(80);
        }
        Destroy();
    }
}