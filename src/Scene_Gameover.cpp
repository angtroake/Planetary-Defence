#include "Scene_Gameover.h"
#include "engine.h"
#include "Scene_Main_Menu.h"


Scene_Gameover::Scene_Gameover(GameEngine* engine)
{
	_engine = engine;
	init();
}


void Scene_Gameover::init()
{
	registerKeyAction(sf::Keyboard::Escape, "EXIT");
	registerKeyAction(sf::Keyboard::Enter, "EXIT");
	_engine->stopSounds();
	_engine->getAssets().getSound("GameOver").play();

}


void Scene_Gameover::tick()
{
}

void Scene_Gameover::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	Sprite spr = _engine->getAssets().getSprite("Background");
	spr.get().setOrigin({ spr.getSize().x / 2.0f, spr.getSize().y / 2.0f });
	spr.get().setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y / 2.0f });
	_engine->getWindow().draw(spr.get());

	sf::Text text;
	text.setString("Game Over");
	text.setFillColor(sf::Color(255, 255, 255));
	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setCharacterSize(64);
	text.setPosition({_engine->getWindowSize().x / 2.0f, 200});
	Util::centerText(text);
	_engine->getWindow().draw(text);

	text.setString("Main Menu: ESC");
	text.setCharacterSize(42);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, 500 });
	Util::centerText(text);
	_engine->getWindow().draw(text);


	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setOutlineColor(sf::Color(0, 0, 0));
	text.setOutlineThickness(1.0f);
	text.setFillColor(sf::Color(255, 255, 255));

	text.setCharacterSize(18);
	text.setString("Score");
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 200.0f });
	_engine->getWindow().draw(text);

	text.setCharacterSize(36);
	text.setString(std::to_string(_engine->currentScore));
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 170.0f });
	_engine->getWindow().draw(text);

}

void Scene_Gameover::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "EXIT" && action.type == ActionType::KEY_PRESS) 
	{
		_engine->changeScene(std::make_shared<Scene_Main_Menu>(_engine), "MAIN_MENU", true);
	}
}
	