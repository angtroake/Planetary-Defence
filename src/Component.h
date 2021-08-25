#pragma once
#include "base.h"
#include "Sprite.h"
#include "Animation.h"


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
		std::shared_ptr<sf::Shader> fragShader = nullptr;
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

	class Orbit : public Component
	{
	public:
		Orbit() {}
		Orbit(Entity target, float distance, float speed, bool cw): target(target), distance(distance), speed(speed), clockWise(cw) {}

		Entity target;
		float distance;
		float speed;
		bool clockWise;
		bool moving = false;
		float currentAngle = 0;
	};

	class Input : public Component
	{
	public:
		bool cw = false;
		bool ccw = false;
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		bool shoot = false;
		bool canShoot = true;

		Input() {}
	};

	class CAnimation : public Component
	{
	public:
		CAnimation(){}
		CAnimation(std::shared_ptr<Animation> ani): animation(ani) {}

		std::shared_ptr<Animation> animation;
	};

	class BoundingBox : public Component 
	{
	public:
		BoundingBox() {}
		BoundingBox(Vec2 size): size(size), halfSize(size/2.0f) {}

		Vec2 size;
		Vec2 halfSize;
	};

	class Rope : public Component
	{
	public:
		Rope() {}
		Rope(size_t ropeLength, float segmentDistance, Vec2 anchor, Vec2* position, Vec2 positionOffset) : ropeLength(ropeLength), segmentDistance(segmentDistance), anchor(anchor), position(position), positionOffset(positionOffset)
		{
			Vec2 dir = (*position - anchor)/segmentDistance;
			for (size_t i = 0; i < ropeLength; i++) 
			{
				segmentPositions.push_back(anchor + (dir * i));
				prevSegmentPositions.push_back(anchor + (dir * i));
			}
			segmentPositions[ropeLength - 1] = *position;
			prevSegmentPositions[ropeLength - 1] = *position;
		}

		Vec2 anchor;
		Vec2* position;
		Vec2 positionOffset;
		std::vector<Vec2> segmentPositions;
		std::vector<Vec2> prevSegmentPositions;
		size_t ropeLength;
		float segmentDistance;
	};
}


