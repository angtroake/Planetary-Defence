#include "AI.h"
#include "engine.h"

void AI::simulate(GameEngine* engine) 
{
	tick(engine);
}

AIUFO::AIUFO(Vec2* position, Vec2* target, EntityManager* entityManager)
	:position(position), target(target), entityManager(entityManager)
{
	timeToShoot = 3 * 60 + (rand() % (3 * 60));
}

void AIUFO::tick(GameEngine* engine)
{
	timeToShoot--;
	if (timeToShoot <= 0) 
	{
		Vec2 dir = *target - *position;
		dir = dir / dir.mag();
		Vec2 vel = dir * 20.0f;

		auto bullet = entityManager->createEntity("Bullet");
		auto& m = entityManager->addComponent<Component::Material>(bullet, engine->getAssets().getSprite("Laser"), true);
		auto& t = entityManager->addComponent<Component::Transform>(bullet, *position + dir * m.sprite.getSize().y, vel, Vec2(1,1), true);
		entityManager->addComponent<Component::Lifespan>(bullet, 3 * 60);
		entityManager->addComponent<Component::BoundingBox>(bullet, m.sprite.getSize());
	
		int num = rand() % 4 + 1;
		engine->getAssets().getSound("Laser" + std::to_string(num)).play();

		timeToShoot = 3 * 60 + (rand() % (3 * 60));
	}
}