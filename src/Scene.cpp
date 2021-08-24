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

void Scene::renderEntity(const int & entity, bool debug) 
{
	//Render Sprite of Entity
	if (_entityManager.hasComponent<Component::Material>(entity))
	{
		auto& mat = _entityManager.getComponent<Component::Material>(entity);
		auto& transfrom = _entityManager.getComponent<Component::Transform>(entity);

		mat.sprite.get().setPosition(transfrom.position.x - (mat.sprite.getSize().x * transfrom.scale.x) / 2, transfrom.position.y - (mat.sprite.getSize().y * transfrom.scale.y) / 2);
		mat.sprite.get().setScale({ transfrom.scale.x, transfrom.scale.y });
		if (mat.fragShader != nullptr) 
		{
			_engine->getWindow().draw(mat.sprite.get(), &*mat.fragShader);
		}
		else
		{
			_engine->getWindow().draw(mat.sprite.get());
		}
	}

	if (debug && _entityManager.hasComponent<Component::BoundingBox>(entity)) 
	{
		auto& boundingbox = _entityManager.getComponent<Component::BoundingBox>(entity);
		auto& transfrom = _entityManager.getComponent<Component::Transform>(entity);

		sf::RectangleShape rect;
		rect.setSize({ boundingbox.size.x, boundingbox.size.y });
		rect.setOutlineColor(sf::Color(255, 0, 0));
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineThickness(2);
		rect.setOrigin(boundingbox.halfSize.x, boundingbox.halfSize.y);
		rect.setPosition({ transfrom.position.x, transfrom.position.y });
		_engine->getWindow().draw(rect);
	}
}