#include "Scene_Main_Menu.h"
#include "engine.h"
#include <iostream>

Scene_Main_Menu::Scene_Main_Menu(GameEngine* engine) {
	_engine = engine;
	init();
}

void Scene_Main_Menu::init() {
	registerKeyAction(sf::Keyboard::Escape, "QUIT");

}

void Scene_Main_Menu::render() {

}

void Scene_Main_Menu::tick() {
	_entityManager.update();
}

void Scene_Main_Menu::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "QUIT") { _engine->quit(); }
}
