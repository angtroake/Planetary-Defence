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
	}
}

void Scene_Play::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	for (auto entity : _entityManager.getEntities()) 
	{
		renderEntity(entity);
	}
}


void Scene_Play::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "EXIT" && action.type == ActionType::KEY_PRESS) 
	{
		_engine->changeScene(nullptr, "MAIN_MENU", true);
	}
}
