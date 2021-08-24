#include "Physics.h"
#include "EntityManager.h"

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