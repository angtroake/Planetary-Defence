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

void Scene::simulate(const int frames)
{
	for (size_t i = 0; i < frames; i++) 
	{
		tick();
	}
}

void Scene::renderEntity(const int & entity) 
{
	//Render Sprite of Entity
	if (_entityManager.hasComponent<Component::Material>(entity))
	{
		auto& mat = _entityManager.getComponent<Component::Material>(entity);
		auto& transfrom = _entityManager.getComponent<Component::Transform>(entity);

		mat.sprite.get().setPosition(transfrom.position.x - mat.sprite.getSize().x / 2, transfrom.position.y - mat.sprite.getSize().y / 2);
		_engine->getWindow().draw(mat.sprite.get());
	}
}