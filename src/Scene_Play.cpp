#include "Scene_Play.h"
#include "engine.h"

Scene_Play::Scene_Play(GameEngine* engine)
{
	_engine = engine;
	init();
}


void Scene_Play::init()
{
	registerKeyAction(sf::Keyboard::Escape, "EXIT");


	earth = _entityManager.createEntity("Earth");
	_entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindow().getSize().x/2, _engine->getWindow().getSize().y / 2), Vec2(0,0), Vec2(1,1), false);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
	_entityManager.addComponent<Component::Health>(earth, 100);

	auto test = _entityManager.createEntity("Test");
	_entityManager.addComponent<Component::Transform>(test, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), true);
	_entityManager.addComponent<Component::Orbit>(test, earth, 300, 0.01, true);
}

void Scene_Play::tick()
{
	_entityManager.update();

	for (auto entity : _entityManager.getEntities())
	{
		if (_entityManager.hasComponent<Component::Material>(entity))
		{
			auto& mat = _entityManager.getComponent<Component::Material>(entity);
			mat.sprite.animate();
			if (mat.sprite.ended() && !mat.repeat)
			{
				_entityManager.destroyEntity(entity);
			}
		}

		if (_entityManager.hasComponent<Component::Orbit>(entity)) 
		{
			auto& orbit = _entityManager.getComponent<Component::Orbit>(entity);
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);

			if (_entityManager.isAlive(orbit.target)) 
			{
				auto& target_transform = _entityManager.getComponent<Component::Transform>(orbit.target);

				if (orbit.clockWise) { orbit.currentAngle = orbit.currentAngle + orbit.speed; }
				else { orbit.currentAngle = orbit.currentAngle - orbit.speed; }
				
				if (orbit.currentAngle > 2.0f * PI) { orbit.currentAngle -= 2.0f * PI; }
				else if (orbit.currentAngle < 0) { orbit.currentAngle += 2.0f * PI; }

				Vec2 vec(orbit.distance * sin(orbit.currentAngle), orbit.distance * cos(orbit.currentAngle));
				vec = target_transform.position + vec;

				transform.prevVelocity = transform.velocity;
				transform.velocity = vec - transform.position;
			}
		}

		if (_entityManager.hasComponent<Component::Transform>(entity)) 
		{
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);

			transform.prevPosition = transform.position;
			transform.position = transform.position + transform.velocity;
		}
	}
}

void Scene_Play::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	for (auto entity : _entityManager.getEntities()) 
	{
		renderEntity(entity);

		if (_entityManager.getTag(entity) == "Test") 
		{
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);
			sf::RectangleShape rect;
			rect.setPosition({transform.position.x - 25, transform.position.y - 25});
			rect.setFillColor(sf::Color(255,255,255));
			rect.setSize({ 50,50 });
			_engine->getWindow().draw(rect);
		}
	}
}


void Scene_Play::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "EXIT" && action.type == ActionType::KEY_PRESS) 
	{
		_engine->changeScene(nullptr, "MAIN_MENU", true);
	}
}
