#pragma once
#include "base.h"
#include "Sprite.h"

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
		Transform(Vec2 pos, Vec2 vel, Vec2 scale, bool rot) : position(pos), velocity(vel), scale(scale), rotates(rot) {}

		Vec2 position;
		Vec2 prevPosition;
		Vec2 velocity;
		Vec2 prevVelocity;
		Vec2 direction;
		Vec2 scale;
		bool rotates;
	};

	class Material : public Component 
	{
	public:
		Material() {}
		Material(Sprite spr, bool repeat): sprite(spr), repeat(repeat) {}
		
		Sprite sprite;
		bool repeat;
	};

	class Health : public Component
	{
	public:
		Health() {}
		Health(size_t maxHealth): health(maxHealth), maxHealth(maxHealth) {}
		
		void damage(size_t amount) { health -= amount; }

		size_t health;
		size_t maxHealth;

	};
}


