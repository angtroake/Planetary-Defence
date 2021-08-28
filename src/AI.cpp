#include "AI.h"
#include "engine.h"

void AI::simulate(GameEngine* engine) 
{
	tick(engine);
}

AIUFO::AIUFO(Vec2* position, Vec2* target, EntityManager* entityManager,size_t maxTimeToShoot)
	:position(position), target(target), entityManager(entityManager), maxTimeToShoot(maxTimeToShoot)
{
	timeToShoot = maxTimeToShoot;
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

		timeToShoot = maxTimeToShoot;
	}
}

AIMothership::AIMothership(Entity mothership, Entity shield, Vec2* target, EntityManager* entityManager)
	:mothership(mothership), target(target), entityManager(entityManager), shield(shield)
{
	timeToShoot = randTimeToShoot();
	timeToSpawn = randTimeToSpawn();
}

void AIMothership::tick(GameEngine* engine)
{
	if (isShooting) 
	{
		shootCooldown--;
		if (shootCooldown <= 0) 
		{
			auto& transform = entityManager->getComponent<Component::Transform>(mothership);

			//Shoot
			engine->getAssets().getSound("MothershipShoot").play();
			
			Vec2 pos = transform.position + (*target - transform.position) / 2.0f;
			
			auto entity = entityManager->createEntity("MothershipLaser");
			auto& t = entityManager->addComponent<Component::Transform>(entity, pos, Vec2(0,0), Vec2(1, 1), true);
			auto& mat = entityManager->addComponent<Component::Material>(entity, engine->getAssets().getSprite("GammaRay"), true);
			entityManager->addComponent<Component::Lifespan>(entity, 2 * 60);
			
			Vec2 bbSize = mat.sprite.getSize();
			bbSize.x *= t.scale.x;
			bbSize.y * -t.scale.y;
			entityManager->addComponent<Component::BoundingBox>(entity, bbSize);

			isShooting = false;
		}

		//Decrease size of shield
		if (entityManager->getComponent<Component::Transform>(shield).scale.x > 0) 
		{
			auto& t = entityManager->getComponent<Component::Transform>(shield);
			t.scale = t.scale - Vec2(0.01, 0.01);
			Vec2 bb = entityManager->getComponent<Component::Material>(shield).sprite.getSize();
			bb.x *= t.scale.x;
			bb.y *= t.scale.y;
			auto& b = entityManager->getComponent<Component::BoundingBox>(shield);
			b.size = bb;
			b.halfSize = bb / 2.0f;
		}
	}
	else
	{
		timeToShoot--;
		if (timeToShoot <= 0)
		{
			shootCooldown = 5 * 60;
			isShooting = true;
			engine->getAssets().getSound("MothershipCharge").play();
			timeToShoot = randTimeToShoot();
		}

		timeToSpawn--;
		if (timeToSpawn <= 0)
		{
			auto& transform = entityManager->getComponent<Component::Transform>(mothership);
			auto& rope = entityManager->getComponent<Component::Rope>(mothership);
			auto& orbit = entityManager->getComponent<Component::Orbit>(mothership);
			//Spawn 2 UFOs
			for (int i = 0; i < 2; i++)
			{
				auto ufo = entityManager->createEntity("UFO");
				auto& t = entityManager->addComponent<Component::Transform>(ufo, transform.position, Vec2(0, 0), Vec2(1, 1), true);
				entityManager->addComponent<Component::Health>(ufo, 1.0f);
				auto& mat = entityManager->addComponent<Component::Material>(ufo, engine->getAssets().getSprite("UFO"), true);
				entityManager->addComponent<Component::BoundingBox>(ufo, (mat.sprite.getSize() * 0.6f));

				size_t timeToShoot = 3 * 60 + (rand() % (3 * 60));
				std::shared_ptr<AIUFO> ai = std::make_shared<AIUFO>(&t.position, target, entityManager, timeToShoot);
				entityManager->addComponent<Component::CAI>(ufo, ai);

				//copy rope of mothership to ufo
				auto& r = entityManager->addComponent<Component::Rope>(ufo, rope.ropeLength, rope.segmentDistance, *rope.anchor, &t.position, Vec2(0, 28), true);
				for (size_t j = 0; j < r.ropeLength; j++)
				{
					r.segmentPositions[j] = rope.segmentPositions[j];
					r.prevSegmentPositions[j] = rope.prevSegmentPositions[j];
				}

				float dist = 300 + rand() % 200;
				auto& o = entityManager->addComponent<Component::Orbit>(ufo, target, dist, 0.01f, (bool)(i), orbit.currentAngle, false, false);
				o.moving = true;
			}

			timeToSpawn = randTimeToSpawn();
		}

		//Increase size of shield
		if (entityManager->getComponent<Component::Transform>(shield).scale.x < 1.1)
		{
			auto& t = entityManager->getComponent<Component::Transform>(shield);
			t.scale = t.scale + Vec2(0.01, 0.01);
			Vec2 bb = entityManager->getComponent<Component::Material>(shield).sprite.getSize();
			bb.x *= t.scale.x;
			bb.y *= t.scale.y;
			auto& b = entityManager->getComponent<Component::BoundingBox>(shield);
			b.size = bb;
			b.halfSize = bb/2.0f;
		}
	}

	entityManager->getComponent<Component::Transform>(shield).position = entityManager->getComponent<Component::Transform>(mothership).position;
}

int AIMothership::randTimeToShoot()
{
	return (15 * 60) + (rand() % 6) * 60;
}

int AIMothership::randTimeToSpawn()
{
	return (3 * 60) + (rand() % 2) * 60;
}