#pragma once
#include "base.h"

const int MAX_COMPONENTS = 32;

 
namespace Component {
	class Component{
	public:
		bool has = false;
	};

	class Transform : public Component 
	{
	public:
		Transform() {}
		Transform(Vec2 pos, Vec2 vel, bool rot) : position(pos), velocity(vel), rotates(rot) {}

		Vec2 position;
		Vec2 prevPosition;
		Vec2 velocity;
		Vec2 prevVelocity;
		Vec2 direction;
		bool rotates;
	};
}


