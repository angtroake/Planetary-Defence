#include "Animation.h"
#include <assert.h>
#include "EntityManager.h"
#include "engine.h"

void Animation::finish()
{
	_isFinished = true;
}

bool Animation::isFinished() const 
{
	return _isFinished;
}

void Animation::animateStep(const Entity& entity, EntityManager& entityManager)
{
	animate(entity, entityManager);
}

void Animation::init(const Entity& entity, EntityManager& entityManager)
{
	setup(entity, entityManager);
}

void AnimationDrop::setup(const Entity& entity, EntityManager& entityManager)
{
	auto& transform = entityManager.getComponent<Component::Transform>(entity);
	transform.position.y = -50;
}

void AnimationDrop::animate(const Entity& entity, EntityManager& entityManager)
{
	auto& transform = entityManager.getComponent<Component::Transform>(entity);

	if (step == 0) 
	{
		//FALLING
		transform.velocity.y += GRAVITY;
		if (transform.velocity.y > MAXFALL) { transform.velocity.y = MAXFALL; }

		if (transform.position.y >= targetY) 
		{
			transform.velocity.y = 0;
			step = 1;
		}
	}
	else if (step == 1) 
	{
		//BOUNCE UP
		transform.velocity.y = (targetY / _engine->getWindow().getSize().y) * -12.0f;
		step = 2;

	}
	else if (step == 2) 
	{
		//2ND FALL
		if (transform.velocity.y > 0 && transform.position.y >= targetY) 
		{
			transform.velocity = resetVelocity;
			finish();
		}
		else 
		{
			transform.velocity.y += GRAVITY;
		}
	}
}

void Cooldown::animate(const Entity& entity, EntityManager& entityManager)
{
	frames--;
	if (frames <= 0)
	{
		finish();
	}
}

void Cooldown::setup(const Entity& entity, EntityManager& entityManager)
{

}

void AnimationBlink::animate(const Entity& entity, EntityManager& entityManager)
{
	frames--;
	if (entityManager.hasComponent<Component::Material>(entity)) 
	{
		auto& mat = entityManager.getComponent<Component::Material>(entity);

		mat.sprite.get().setColor(sf::Color(255, 255, 255, 255 * (frames % interval) / interval));
	}

	if (frames <= 0) 
	{
		finish();
	}
}

void AnimationBlink::setup(const Entity& entity, EntityManager& entityManager)
{
}
