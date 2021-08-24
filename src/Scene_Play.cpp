#include "Scene_Play.h"
#include "engine.h"
#include <iostream>
#include "Physics.h"


Scene_Play::Scene_Play(GameEngine* engine)
{
	_engine = engine;
	init();
}


void Scene_Play::init()
{
	registerKeyAction(sf::Keyboard::Escape, "EXIT");
	registerKeyAction(sf::Keyboard::BackSlash, "DEBUG");

	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid1"));
	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid2"));

	earth = _entityManager.createEntity("Earth");
	_entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindow().getSize().x/2, _engine->getWindow().getSize().y / 2), Vec2(0,0), Vec2(1,1), false);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
	_entityManager.addComponent<Component::Health>(earth, 100);
	_entityManager.addComponent<Component::BoundingBox>(earth, Vec2(256, 256));
	std::shared_ptr<AnimationDrop> ani = std::make_shared<AnimationDrop>(_engine, _engine->getWindow().getSize().y / 2);
	ani->init(earth, _entityManager);
	_entityManager.addComponent<Component::CAnimation>(earth, ani);

	//Shader Test Fail
	/*
	auto test = _entityManager.createEntity("Test");
	_entityManager.addComponent<Component::Transform>(test, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), true);
	_entityManager.addComponent<Component::Orbit>(test, earth, 300, 0.01, true);
	*/
}

void Scene_Play::tick()
{
	_entityManager.update();


	//Game Logic


	//Entity Logic
	for (auto entity : _entityManager.getEntities())
	{

		//Orbit Update
		if (_entityManager.hasComponent<Component::Orbit>(entity)) 
		{
			auto& orbit = _entityManager.getComponent<Component::Orbit>(entity);
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);

			if (_entityManager.isAlive(orbit.target)) 
			{
				auto& target_transform = _entityManager.getComponent<Component::Transform>(orbit.target);

				if (orbit.clockWise) { orbit.currentAngle = orbit.currentAngle + orbit.speed; }
				else { orbit.currentAngle = orbit.currentAngle - orbit.speed; }
				
				if (orbit.currentAngle > 2.0f * PI) { orbit.currentAngle -= 2.0f * PI; }
				else if (orbit.currentAngle < 0) { orbit.currentAngle += 2.0f * PI; }

				Vec2 vec(orbit.distance * sin(orbit.currentAngle), orbit.distance * cos(orbit.currentAngle));
				vec = target_transform.position + vec;

				transform.prevVelocity = transform.velocity;
				transform.velocity = vec - transform.position;
			}
		}

		handleAnimations(entity);
		handleMovement(entity);
	}

	handleCollisions();

	timeUntilAsteroid--;
	if (timeUntilAsteroid == 0) 
	{
		spawnAsteroid();
		timeUntilAsteroid = rand() % 600;
	}


	_currentFrame++;
	timeAlive++;
}

void Scene_Play::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	for (auto entity : _entityManager.getEntities())
	{
		renderEntity(entity, renderDebug);
	}

	renderHealth();
}

void Scene_Play::renderHealth()
{
	//Draw Title
	sf::Text text;
	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setCharacterSize(36);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setString("Health");
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x - 20.0f - (_engine->getWindowSize().x / 8.0f), 20 });
	_engine->getWindow().draw(text);

	//Health Bar
	sf::RectangleShape rect;
	rect.setSize({ _engine->getWindowSize().x / 4.0f, _engine->getWindowSize().y / 25.0f });
	rect.setOutlineColor(sf::Color(255, 255, 255));
	rect.setFillColor(sf::Color(255, 0, 0));
	rect.setOutlineThickness(5);
	rect.setOrigin(_engine->getWindowSize().x / 4.0f, 0);
	rect.setPosition({ _engine->getWindowSize().x - 20.0f, 50.0f });
	_engine->getWindow().draw(rect);
}

void Scene_Play::handleCollisions()
{
	for (Entity entity : _entityManager.getEntities("Asteroid")) 
	{
		//If asteroid is dropping down don't do collisions
		if (_entityManager.hasComponent<Component::CAnimation>(entity)) { continue; }

		Vec2 overlap = Physics::getOverlap(_entityManager, entity, earth);
		if (overlap.x > 0 && overlap.y > 0) 
		{
			_entityManager.destroyEntity(entity);
			_entityManager.getComponent<Component::Health>(earth).damage(1);
		}
	}
}

void Scene_Play::handleMovement(Entity entity)
{
	if (_entityManager.hasComponent<Component::Transform>(entity))
	{
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);

		transform.prevPosition = transform.position;
		transform.position = transform.position + transform.velocity;
	}
}

void Scene_Play::handleAnimations(Entity entity)
{
	//Sprite animations
	if (_entityManager.hasComponent<Component::Material>(entity))
	{
		auto& mat = _entityManager.getComponent<Component::Material>(entity);
		mat.sprite.animate();
		if (mat.sprite.ended() && !mat.repeat)
		{
			_entityManager.destroyEntity(entity);
		}
	}

	//Physical Animations
	if (_entityManager.hasComponent<Component::CAnimation>(entity))
	{
		auto& animation = _entityManager.getComponent<Component::CAnimation>(entity);
		if (!animation.animation->isFinished())
		{
			animation.animation->animateStep(entity, _entityManager);
		}
		else
		{
			_entityManager.removeComponent<Component::CAnimation>(entity);
		}
	}
}


void Scene_Play::onKeyAction(std::string actionName, KeyAction action)
{
	if (actionName == "EXIT" && action.type == ActionType::KEY_PRESS) 
	{
		_engine->changeScene(nullptr, "MAIN_MENU", true);
	}
	if (actionName == "DEBUG" && action.type == ActionType::KEY_PRESS) 
	{
		renderDebug = !renderDebug;
	}
}

void Scene_Play::spawnAsteroid()
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	float angle = (float)(rand() % 360) * PI / 180.0f;
	Vec2 pos(500 * sin(angle), 500 * cos(angle));
	pos = earthTransform.position + pos;
	Vec2 vel = (earthTransform.position - pos) / pos.dist(earthTransform.position) * 0.5;

	size_t sprite = rand() % _asteroidSprites.size();

	auto entity = _entityManager.createEntity("Asteroid");
	_entityManager.addComponent<Component::Transform>(entity, pos, vel, Vec2(1,1), false);
	_entityManager.addComponent<Component::Health>(entity, 1);
	auto & mat = _entityManager.addComponent<Component::Material>(entity, _asteroidSprites[sprite], true);
	_entityManager.addComponent<Component::BoundingBox>(entity, (mat.sprite.getSize()*0.6f));
	std::shared_ptr<AnimationDrop> ani = std::make_shared<AnimationDrop>(_engine, pos.y, vel);
	ani->init(entity, _entityManager);
	_entityManager.addComponent<Component::CAnimation>(entity, ani);

}