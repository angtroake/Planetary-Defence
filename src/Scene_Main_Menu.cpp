#include "Scene_Main_Menu.h"
#include "engine.h"
#include "Scene_Play.h"
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

	_engine->getAssets().getSound("MusicMenu").play();
	/*
	auto earth = _entityManager.createEntity("Earth");
	_entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindowSize().x/2, _engine->getWindowSize().y), Vec2(0,0), Vec2(3,3), false);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
	*/
} 


void Scene_Main_Menu::render() 
{

	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	Sprite spr = _engine->getAssets().getSprite("Background");
	spr.get().setOrigin({spr.getSize().x / 2.0f, spr.getSize().y / 2.0f});
	spr.get().setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y / 2.0f });
	_engine->getWindow().draw(spr.get());

	//Draw Title
	sf::Text text;
	text.setFont(_engine->getAssets().getFont("Crater"));
	/*
	text.setCharacterSize(48);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setString("Planetary Defence!");
	Util::centerText(text);
	text.setPosition({_engine->getWindowSize().x/2.0f, 60});
	_engine->getWindow().draw(text);
	*/
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
			text.setFillColor(sf::Color(150, 150, 150));
		}
		text.setCharacterSize(36);
		text.setString(menuButtons[i]);
		Util::centerText(text);
		text.setPosition({ _engine->getWindowSize().x / 2.0f, 500 + 50 * (float)i });
		_engine->getWindow().draw(text);
	}

	text.setOutlineColor(sf::Color(0, 0, 0));
	text.setOutlineThickness(1.0f);
	text.setFillColor(sf::Color(255, 255, 255));

	text.setCharacterSize(18);
	text.setString("Current Score");
	Util::centerText(text);
	text.setPosition({_engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 200.0f});
	_engine->getWindow().draw(text);

	text.setCharacterSize(36);
	text.setString(std::to_string(_engine->currentScore));
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 170.0f });
	_engine->getWindow().draw(text);

	text.setCharacterSize(18);
	text.setString("Best Score");
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 120.0f });
	_engine->getWindow().draw(text);

	text.setCharacterSize(36);
	text.setString(std::to_string(_engine->bestScore));
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 90.0f });
	_engine->getWindow().draw(text);
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
	if (actionName == "QUIT" && action.type == ActionType::KEY_PRESS) { _engine->quit(); }
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
			_engine->stopSounds();
			_engine->changeScene(std::make_shared<Scene_Play>(_engine), "PLAY", false);
		}
		//Exit Button
		else if(selectedMenuButton == 1)
		{
			_engine->quit();
		}
	}
}
