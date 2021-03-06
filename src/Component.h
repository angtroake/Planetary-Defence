#pragma once
#include "base.h"
#include "Sprite.h"
#include "Animation.h"
#include "AI.h"

class AI;

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
		Transform(Vec2 pos, Vec2 vel, Vec2 scale, bool rot) : position(pos), velocity(vel), scale(scale), rotates(rot) 
		{
			if (rotates && vel.mag() != 0) { direction = vel / vel.mag(); }
		}

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
		Material(Sprite spr, bool repeat) : Material(spr, repeat, sf::IntRect({ 0, 0 }, { (int)spr.getSize().x, (int)spr.getSize().y })) {}
		Material(Sprite spr, bool repeat, sf::IntRect crop) : sprite(spr), repeat(repeat), crop(crop) {}

		Sprite sprite;
		bool repeat;
		sf::IntRect crop;
		std::shared_ptr<sf::Shader> fragShader = nullptr;
		int opacity = 255;
	};

	class Health : public Component
	{
	public:
		Health() {}
		Health(float maxHealth): health(maxHealth), maxHealth(maxHealth) {}
		
		void damage(float amount) { health -= amount; }

		float health;
		float maxHealth;

	};

	class Orbit : public Component
	{
	public:
		Orbit() {}
		Orbit(Vec2* target, float distance, float speed, bool cw, float startAngle, bool transformDirection, bool stickToDistance) 
			: target(target), distance(distance), speed(speed), clockWise(cw), currentAngle(startAngle), transformDirection(transformDirection), stickToDistance(stickToDistance){}
		Orbit(Vec2 target, float distance, float speed, bool cw, float startAngle, bool transformDirection, bool stickToDistance)
			:_target(target), target(&_target), distance(distance), speed(speed), clockWise(cw), currentAngle(startAngle), transformDirection(transformDirection), stickToDistance(stickToDistance) {}

		Vec2 _target;
		Vec2* target;
		float distance;
		float speed;
		bool clockWise;
		bool moving = false;
		float currentAngle = 0;
		bool stickToDistance;
		bool transformDirection;
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
		bool canShoot = false;

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
		Rope(size_t ropeLength, float segmentDistance, Vec2 anchor, Vec2* position, Vec2 positionOffset, bool movementOnAnchor) 
			: ropeLength(ropeLength), segmentDistance(segmentDistance), position(position), positionOffset(positionOffset), movementOnAnchor(movementOnAnchor)
		{
			assert(ropeLength >= 2);
			Vec2 dir = (*position - anchor)/segmentDistance;
			for (size_t i = 0; i < ropeLength; i++) 
			{
				segmentPositions.push_back(anchor + (dir * i));
				prevSegmentPositions.push_back(anchor + (dir * i));
			}
			segmentPositions[ropeLength - 1] = *position;
			prevSegmentPositions[ropeLength - 1] = *position;

			this->anchor = &segmentPositions[0];
		}

		Rope(size_t ropeLength, float segmentDistance, Vec2 anchor, Vec2* position, Vec2* direction, Vec2 positionOffset, bool movementOnAnchor)
			: Rope(ropeLength, segmentDistance, anchor, position, positionOffset, movementOnAnchor)
		{
			this->direction = direction;
		}

		bool movementOnAnchor;
		Vec2* anchor;
		Vec2* position;
		Vec2* direction = nullptr;
		Vec2 positionOffset;
		std::vector<Vec2> segmentPositions;
		std::vector<Vec2> prevSegmentPositions;
		size_t ropeLength;
		float segmentDistance;
	};

	class Lifespan : public Component 
	{
	public:
		Lifespan() {}
		Lifespan(size_t frames): frames(frames) {}

		int frames;
	};

	class PopsicleStick : public Component 
	{
	public:
		PopsicleStick() {};
		PopsicleStick(Vec2 anchor, Vec2* position, bool movementOnAnchor): _anchor(anchor), anchor(&_anchor), attachment(position), movementOnAnchor(movementOnAnchor){};
		PopsicleStick(Vec2* anchor, Vec2* position, bool movementOnAnchor) : anchor(anchor), attachment(position), movementOnAnchor(movementOnAnchor) {};

		Vec2* anchor;
		Vec2* attachment;
		bool movementOnAnchor;
	protected:
		Vec2 _anchor;
	};

	class CAI : public Component 
	{
	public:
		CAI() {}
		CAI(std::shared_ptr<AI> ai): ai(ai) {}
		
		std::shared_ptr<AI> ai;
	};

	class Invincibility : public Component 
	{
	public:
		Invincibility() {}
		Invincibility(size_t frames): frames(frames) {}

		size_t frames;
	};

	class Parent : public Component 
	{
	public:
		Parent() {}
		Parent(Entity entity, Vec2* position, Vec2* directon, Vec2* velocity, bool rotateOnVelocity)
			:parent(entity), position(position), directon(directon), velocity(velocity), rotateOnVelocity(rotateOnVelocity){}

		bool rotateOnVelocity;
		Entity parent;
		Vec2* position;
		Vec2* directon;
		Vec2* velocity;
	};
}


