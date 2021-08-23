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
	registerKeyAction(sf::Keyboard::Up, "UP");
	registerKeyAction(sf::Keyboard::S, "DOWN");
	registerKeyAction(sf::Keyboard::W, "UP");
	registerKeyAction(sf::Keyboard::Enter, "ENTER");
	registerKeyAction(sf::Keyboard::Space, "ENTER");

	auto earth = _entityManager.createEntity("Earth");
	_entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindowSize().x/2, _engine->getWindowSize().y), Vec2(0,0), Vec2(5,5), false);
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
	Util::centerText(text);
	text.setPosition({_engine->getWindowSize().x/2.0f, 60});
	_engine->getWindow().draw(text);


	for (auto entity : _entityManager.getEntities()) 
	{
		renderEntity(entity);
	}

	for (size_t i = 0; i < menuButtonCount; i++)
	{
		text.setOutlineColor(sf::Color(0, 0, 0));
		text.setOutlineThickness(1.0f);
		if (selectedMenuButton == i) 
		{ 
			text.setFillColor(sf::Color(255, 255, 255)); 
		}
		else 
		{
			text.setFillColor(sf::Color(200, 200, 200));
		}
		text.setCharacterSize(36);
		text.setString(menuButtons[i]);
		Util::centerText(text);
		text.setPosition({ _engine->getWindowSize().x / 2.0f, 300 + 50 * (float)i });
		_engine->getWindow().draw(text);
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
	else if (actionName == "DOWN" && action.type == ActionType::KEY_PRESS) 
	{
		selectedMenuButton = (selectedMenuButton + menuButtonCount + 1) % menuButtonCount;
	}
	else if (actionName == "UP" && action.type == ActionType::KEY_PRESS)
	{
		selectedMenuButton = (selectedMenuButton + menuButtonCount - 1) % menuButtonCount;
	}
	else if (actionName == "ENTER" && action.type == ActionType::KEY_PRESS) 
	{
		//Play Button
		if (selectedMenuButton == 0) 
		{
			
		}
		//Options Screen
		else if (selectedMenuButton == 1) 
		{
		
		}
		//Exit Button
		else if(selectedMenuButton == 2)
		{
			_engine->quit();
		}
	}
}
