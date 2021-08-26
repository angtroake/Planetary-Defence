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
		Vec2 vel = *target - *position;
		vel = vel / vel.mag() * 2;

		auto bullet = entityManager->createEntity("Bullet");
		auto& t = entityManager->addComponent<Component::Transform>(bullet, *position, vel, Vec2(0.5,0.5), true);
		auto& m = entityManager->addComponent<Component::Material>(bullet, engine->getAssets().getSprite("GammaRay"), true);
		entityManager->addComponent<Component::Lifespan>(bullet, 5 * 60);
		Vec2 bb = m.sprite.getSize();
		bb.x *= t.scale.x;
		bb.y *= t.scale.y;
		entityManager->addComponent<Component::BoundingBox>(bullet, bb);

		timeToShoot = 3 * 60 + (rand() % (3 * 60));
	}
}