#include "Scene.h"
#include "engine.h"

Scene::Scene(GameEngine* engine) {
	_engine = engine;
}

const std::map<int, std::string> & Scene::getKeyActionMap() const
{
	return _keyActionMap;
}


void Scene::registerKeyAction(int key, std::string actionName) 
{
	_keyActionMap[key]= actionName;
}