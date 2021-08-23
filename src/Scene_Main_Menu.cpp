#include "Scene_Main_Menu.h"
#include "engine.h"
#include <iostream>

Scene_Main_Menu::Scene_Main_Menu(GameEngine* engine) 
{
	_engine = engine;
	init();
}

void Scene_Main_Menu::init() 
{
	registerKeyAction(sf::Keyboard::Escape, "QUIT");
	registerKeyAction(sf::Keyboard::Down, "DOWN");
	registerKeyAction(sf::Keyboard::Down, "UP");

	auto earth = _entityManager.createEntity("Earth");
	_entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindowSize().x/2, 600), Vec2(0,0), false);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
} 


void Scene_Main_Menu::render() 
{

	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	//Draw Title
	sf::Text text;
	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setCharacterSize(48);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setString("Planetary Defence!");
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition({_engine->getWindowSize().x/2.0f, 60});
	_engine->getWindow().draw(text);


	for (auto entity : _entityManager.getEntities()) 
	{
		renderEntity(entity);
	}


}

void Scene_Main_Menu::tick() 
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

void Scene_Main_Menu::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "QUIT") { _engine->quit(); }
}
