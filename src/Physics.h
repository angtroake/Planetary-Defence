#pragma once
#include "base.h"

class EntityManager;

namespace Physics 
{
	Vec2 getOverlap(EntityManager & entityManager, Entity entity1, Entity entity2);
}