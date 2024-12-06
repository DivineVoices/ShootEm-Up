#include "pch.h"

#include "SampleScene.h"

#include "DummyEntity.h"

#include "Bullet.h"

#include "Debug.h"

void SampleScene::OnInitialize()
{
	pEntity1 = CreateEntity<DummyEntity>(100, sf::Color::Red);
	pEntity1->SetPosition(1000, 100);

	pPlayer = CreateEntity<DummyEntity>(25, sf::Color::White);
	pPlayer->SetPosition(800, 350);

	x = pPlayer->GetPosition().x;
	y = pPlayer->GetPosition().y;

	speed = 200;

	pEntitySelected = nullptr;

	mAreas[0] = { 0, 0, 1100, 720 };

}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			if (pEntitySelected != nullptr) 
			{
				pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
			}
		}
		if (event.mouseButton.button == sf::Mouse::Button::Right)
		{
			TrySetSelectedEntity(pEntity1, event.mouseButton.x, event.mouseButton.y);
		}
	}

	// Mouvement
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
			direction.x = 1;
		}
		else if (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Left) {
			direction.x = -1;
		}
		else if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up) {
			direction.y = -1;
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
			direction.y = 1;
		}
	}

	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right ||
			event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Left) {
			direction.x = 0;
		}
		if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up ||
			event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
			direction.y = 0;
		}
	}
}

void SampleScene::TrySetSelectedEntity(DummyEntity* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::OnUpdate()
{
	if(pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position.x, position.y, 10, sf::Color::Blue);
	}

	//Zone pour le d�bug
	for (int i = 0; i < 4; i++)
	{
		const AABB& aabb = mAreas[i];

		Debug::DrawRectangle(aabb.xMin, aabb.yMin, aabb.xMax - aabb.xMin, aabb.yMax - aabb.yMin, sf::Color::White);
	}

	//Projectile
	//std::cout << "Projectiles remaining: " << pProjectiles.size() << std::endl;

	for (auto it = pProjectiles.begin(); it != pProjectiles.end(); ) {
		BulletEntity* pBullet = *it;
		if (pBullet->GetPosition().x > 1280) {
			//std::cout << "Deleting Bullet: " << pBullet << std::endl;
			pBullet->Destroy();
			it = pProjectiles.erase(it);
		}
		else {
			++it;
		}
	}

	int pPx = pPlayer->GetPosition().x + 35;
	int pPy = pPlayer->GetPosition().y;
	pProjectiles.push_back(CreateEntity<BulletEntity>(5, sf::Color::Yellow));
	pProjectiles.back()->SetPosition(pPx, pPy);

	float dt = GameManager::Get()->GetDeltaTime();
	sf::Vector2f velocity = direction * (speed * dt);

	x += velocity.x;
	y += velocity.y;

	pPlayer->SetPosition(x, y);
}