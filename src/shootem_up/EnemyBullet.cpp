#include "pch.h"

#include "EnemyBullet.h"

#include "SampleScene.h"

#include "PlayerEntity.h"

#include "Windows.h"

#include "EnemyEntity.h"

EnemyBulletEntity::EnemyBulletEntity()
    : m_target(nullptr), m_direction(0.f, 0.f)
{
    SetTag(SampleScene::Tag::ENEMYBULLET);
}

void EnemyBulletEntity::SetTarget(Entity* target)
{
    m_target = target;

    if (m_target) {
        sf::Vector2f targetPosition = m_target->GetPosition();
        sf::Vector2f currentPosition = GetPosition();
        sf::Vector2f direction = targetPosition - currentPosition;

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0.f) {
            m_direction = direction / length;
        }
    }
}

void EnemyBulletEntity::SetSpeed(float speed)
{
    m_speed = speed;
}

float EnemyBulletEntity::GetSpeed() const
{
    return 0.0f;
}

void EnemyBulletEntity::OnUpdate()
{
    sf::Vector2f currentPosition = GetPosition();
    GoToDirection(currentPosition.x + m_direction.x * 5, currentPosition.y + m_direction.y * 5, m_speed);

    if (ToDestroy()) return;

    Scene* scene = GetScene();
    if (scene == nullptr) return;

    sf::Vector2f position = GetPosition();
    if (position.x > 1280 || position.x < 0 || position.y > 720 || position.y < 0) {
        Destroy();
    }
}


void EnemyBulletEntity::OnCollision(Entity* pCollidedWith)
{
    if (pCollidedWith == nullptr) return;

    sf::Vector2f tempPos = GetPosition();
    sf::Vector2f collTempPos = pCollidedWith->GetPosition();

    if (pCollidedWith->IsTag(SampleScene::Tag::PLAYER))
    {
        std::cout << "Collision avec le joueur d�tect�e !" << std::endl;

        PlayerEntity* player = dynamic_cast<PlayerEntity*>(pCollidedWith);
        if (player != nullptr)
        {
            player->TakeDamage(1);
            Destroy();
        }
    }
}