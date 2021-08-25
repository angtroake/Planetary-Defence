#include "Physics.h"
#include "EntityManager.h"

//DEPRECATED
Vec2 Physics::getOverlap(EntityManager & entityManager, Entity entity1, Entity entity2)
{
	Vec2 vec();
	if (entityManager.hasComponent<Component::BoundingBox>(entity1) && entityManager.hasComponent<Component::BoundingBox>(entity2))
	{

		float dx = abs(entityManager.getComponent<Component::Transform>(entity1).position.x - entityManager.getComponent<Component::Transform>(entity2).position.x);
		float dy = abs(entityManager.getComponent<Component::Transform>(entity1).position.y - entityManager.getComponent<Component::Transform>(entity2).position.y);

		auto& bb1 = entityManager.getComponent<Component::BoundingBox>(entity1);
		auto& bb2 = entityManager.getComponent<Component::BoundingBox>(entity2);

		return Vec2(bb1.halfSize.x + bb2.halfSize.x - dx, bb1.halfSize.y + bb2.halfSize.y - dy);
	}
	return Vec2(0, 0);
}

bool Physics::isColliding(EntityManager& entityManager, Entity entity1, Entity entity2) 
{
	auto& bb1 = entityManager.getComponent<Component::BoundingBox>(entity1);
	auto& bb2 = entityManager.getComponent<Component::BoundingBox>(entity2);

	auto& t1 = entityManager.getComponent<Component::Transform>(entity1);
	auto& t2 = entityManager.getComponent<Component::Transform>(entity2);

	sf::RectangleShape rect1;
	rect1.setSize({ bb1.size.x, bb1.size.y });
	rect1.setOrigin({ bb1.halfSize.x, bb1.halfSize.y });
	rect1.setPosition({ t1.position.x, t1.position.y });
	if (t1.rotates) {
		rect1.setRotation(Vec2(0, -1).angle(t1.direction) * 180.0f / PI);
	}

	sf::RectangleShape rect2;
	rect2.setSize({ bb2.size.x, bb2.size.y });
	rect2.setOrigin({ bb2.halfSize.x, bb2.halfSize.y });
	rect2.setPosition({ t2.position.x, t2.position.y });
	if (t2.rotates) {
		rect2.setRotation(Vec2(0, -1).angle(t2.direction) * 180.0f / PI);
	}
	return rect1.getGlobalBounds().intersects(rect2.getGlobalBounds());
}

