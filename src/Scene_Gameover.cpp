#include "Scene_Gameover.h"
#include "engine.h"


Scene_Gameover::Scene_Gameover(GameEngine* engine)
{
	_engine = engine;
	init();
}


void Scene_Gameover::init()
{
	registerKeyAction(sf::Keyboard::Escape, "EXIT");
}


void Scene_Gameover::tick()
{
}

void Scene_Gameover::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

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

}

void Scene_Gameover::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "EXIT" && action.type == ActionType::KEY_PRESS) 
	{
		_engine->changeScene(nullptr, "MAIN_MENU", true);
	}
}
	