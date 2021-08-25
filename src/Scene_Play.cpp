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
	registerKeyAction(sf::Keyboard::Down, "DOWN");
	registerKeyAction(sf::Keyboard::Up, "UP");
	registerKeyAction(sf::Keyboard::Left, "LEFT");
	registerKeyAction(sf::Keyboard::Right, "RIGHT");
	registerKeyAction(sf::Keyboard::S, "DOWN");
	registerKeyAction(sf::Keyboard::W, "UP");
	registerKeyAction(sf::Keyboard::A, "LEFT");
	registerKeyAction(sf::Keyboard::D, "RIGHT");
	registerKeyAction(sf::Keyboard::Space, "SHOOT");

	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid1"));
	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid2"));

	earth = _entityManager.createEntity("Earth");
	auto & transfrom = _entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindow().getSize().x/2 + 300, -100), Vec2(0,0), Vec2(1,1), true);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
	_entityManager.addComponent<Component::Health>(earth, 100);
	_entityManager.addComponent<Component::BoundingBox>(earth, Vec2(256, 256));
	//std::shared_ptr<AnimationDrop> ani = std::make_shared<AnimationDrop>(_engine, _engine->getWindow().getSize().y / 2);
	//ani->init(earth, _entityManager);
	//_entityManager.addComponent<Component::CAnimation>(earth, ani);
	_entityManager.addComponent<Component::Rope>(earth, 20, 20.0f, Vec2(_engine->getWindow().getSize().x / 2, 0), &transfrom.position, &transfrom.direction, Vec2(0, 185), true);

	shield = _entityManager.createEntity("Shield");
	_entityManager.addComponent<Component::Transform>(shield, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), true);
	_entityManager.addComponent<Component::Material>(shield, _engine->getAssets().getSprite("Shield"), true);
	_entityManager.addComponent<Component::Orbit>(shield, earth, 300, 0.01, true);
	_entityManager.addComponent<Component::Input>(shield);
	_entityManager.addComponent<Component::BoundingBox>(shield, Vec2(250, 50));
}

void Scene_Play::tick()
{
	_entityManager.update();


	//Game Logic


	//Entity Logic
	for (auto entity : _entityManager.getEntities())
	{
		//Rope Update
		if (_entityManager.hasComponent<Component::Rope>(entity)) 
		{
			auto& rope = _entityManager.getComponent<Component::Rope>(entity);
			
			for (int i = 1; i < rope.ropeLength; i++) 
			{
				Vec2 vel = rope.segmentPositions[i] - rope.prevSegmentPositions[i];
				rope.prevSegmentPositions[i] = rope.segmentPositions[i];

				vel.y += GRAVITY;
				vel = vel * 0.98f;
				rope.segmentPositions[i] = rope.segmentPositions[i] + vel;

			}

			for (int j = 0; j < 30; j++) {
				for (int i = 1; i < rope.ropeLength; i++)
				{
					//rope constraints
					Vec2 dir = rope.segmentPositions[i - 1] - rope.segmentPositions[i];
					dir = dir / dir.mag();

					float d = rope.segmentPositions[i].dist(rope.segmentPositions[i - 1]);
					float err = d - rope.segmentDistance;

					if (i == 1) 
					{
						rope.segmentPositions[i] = rope.segmentPositions[i] + (dir * err);
					}
					else 
					{
						rope.segmentPositions[i - 1] = rope.segmentPositions[i - 1] - (dir * err * 0.8f);
						rope.segmentPositions[i] = rope.segmentPositions[i] + (dir * err);
					}
				}
			}

			if (rope.direction != nullptr) 
			{
				*rope.direction = rope.segmentPositions[rope.ropeLength - 2] - rope.segmentPositions[rope.ropeLength - 1];
				float angle = rope.positionOffset.angle(*rope.direction);
				Vec2 newOffset(rope.positionOffset.x*cos(angle) - rope.positionOffset.y*sin(angle), rope.positionOffset.x*sin(angle) + rope.positionOffset.y * cos(angle));
				*rope.position = rope.segmentPositions[rope.ropeLength - 1] - newOffset;
			}
			else
			{
				*rope.position = rope.segmentPositions[rope.ropeLength - 1] + rope.positionOffset;
			}
		}

		handleLifespan(entity);
		handleOrbit(entity);
		handleAnimations(entity);
		handleMovement(entity);
	}

	handleCollisions();

	timeUntilAsteroid--;
	if (timeUntilAsteroid <= 0) 
	{
		spawnAsteroid();
		timeUntilAsteroid = rand() % 600;
	}

	timeUntilGamma--;
	if (timeUntilGamma <= 0) 
	{
		spawnGammaWarning();
		timeUntilGamma = 20 + rand() % (60 * 30);
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

		if (_entityManager.getTag(entity) == "Shield")
		{
			/*auto& transform = _entityManager.getComponent<Component::Transform>(entity);
			sf::RectangleShape rect;
			rect.setPosition({ transform.position.x - 25, transform.position.y - 25 });
			rect.setFillColor(sf::Color(255, 255, 255));
			rect.setSize({ 50,50 });
			_engine->getWindow().draw(rect);
			*/
		}
	}

	renderHealth();

	if (!alive)
	{
		_engine->changeScene(NULL, "MAIN_MENU", true);
	}
}

void Scene_Play::renderHealth()
{
	float healthBarLength = _engine->getWindowSize().x / 4.0f;

	//Draw Title
	sf::Text text;
	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setCharacterSize(36);
	text.setFillColor(sf::Color(255, 255, 255));
	text.setString("Health");
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x - 20.0f - healthBarLength / 2, 20 });
	_engine->getWindow().draw(text);

	//Health Bar
	sf::RectangleShape healthRect;
	healthRect.setSize({ healthBarLength, _engine->getWindowSize().y / 25.0f });
	healthRect.setOutlineColor(sf::Color(255, 255, 255));
	healthRect.setFillColor(sf::Color(255, 0, 0));
	healthRect.setOutlineThickness(5);
	healthRect.setOrigin(healthBarLength, 0);
	healthRect.setPosition({ _engine->getWindowSize().x - 20.0f, 50.0f });
	_engine->getWindow().draw(healthRect);

	//Lost Health
	float lostHealth = _entityManager.getComponent<Component::Health>(earth).maxHealth - _entityManager.getComponent<Component::Health>(earth).health;
	float maxHealth = _entityManager.getComponent<Component::Health>(earth).maxHealth;

	sf::RectangleShape lostHealthRect;
	lostHealthRect.setSize({ healthBarLength * (lostHealth / maxHealth), _engine->getWindowSize().y / 25.0f });
	lostHealthRect.setOutlineColor(sf::Color(0, 0, 0));
	lostHealthRect.setFillColor(sf::Color(0, 0, 0));
	lostHealthRect.setOutlineThickness(0);
	lostHealthRect.setOrigin(healthBarLength, 0);
	lostHealthRect.setPosition({ _engine->getWindowSize().x - 20.0f, 50.0f });
	_engine->getWindow().draw(lostHealthRect);
}

void Scene_Play::handleCollisions()
{
	for (Entity entity : _entityManager.getEntities("Asteroid")) 
	{
		//If asteroid is dropping down don't do collisions
		if (_entityManager.hasComponent<Component::CAnimation>(entity)) { continue; }

		if (Physics::isColliding(_entityManager, entity, earth))
		{
			_entityManager.destroyEntity(entity);
			_entityManager.getComponent<Component::Health>(earth).damage(1);

			if (_entityManager.getComponent<Component::Health>(earth).health <= 0)
			{
				alive = false;
			}
		}

		if (Physics::isColliding(_entityManager, entity, shield))
		{
			_entityManager.destroyEntity(entity);
		}
	}

	for (Entity entity : _entityManager.getEntities("GammaRay")) 
	{
		if (Physics::isColliding(_entityManager, entity, earth))
		{
			auto& health = _entityManager.getComponent<Component::Health>(earth);
			health.health -= health.maxHealth;
		}
	}
}

void Scene_Play::handleMovement(Entity entity)
{
	if (_entityManager.hasComponent<Component::Transform>(entity))
	{
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);
		if (_entityManager.hasComponent<Component::Rope>(entity) && _entityManager.getComponent<Component::Rope>(entity).movementOnAnchor) 
		{
			auto& rope = _entityManager.getComponent<Component::Rope>(entity);
			*rope.anchor = *rope.anchor + transform.velocity;
		}
		else 
		{
			transform.prevPosition = transform.position;
			transform.position = transform.position + transform.velocity;

			if (!_entityManager.hasComponent<Component::Orbit>(entity)) {
				transform.direction = transform.velocity / transform.velocity.mag();
			}

		}

	}
}

void Scene_Play::handleOrbit(Entity entity)
{
	if (_entityManager.hasComponent<Component::Orbit>(entity))
	{
		auto& orbit = _entityManager.getComponent<Component::Orbit>(entity);
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);
		auto& input = _entityManager.getComponent<Component::Input>(entity);

		if (input.ccw != input.cw)
		{
			orbit.clockWise = input.cw;
			orbit.moving = true;
		}
		else
		{
			orbit.moving = false;
		}

		if (_entityManager.isAlive(orbit.target))
		{
			auto& target_transform = _entityManager.getComponent<Component::Transform>(orbit.target);

			if (orbit.clockWise) { orbit.currentAngle = orbit.currentAngle + (orbit.moving ? orbit.speed : 0); }
			else { orbit.currentAngle = orbit.currentAngle - (orbit.moving ? orbit.speed : 0); }

			if (orbit.currentAngle > 2.0f * PI) { orbit.currentAngle -= 2.0f * PI; }
			else if (orbit.currentAngle < 0) { orbit.currentAngle += 2.0f * PI; }

			Vec2 vec(orbit.distance * sin(orbit.currentAngle), orbit.distance * cos(orbit.currentAngle));
			vec = target_transform.position + vec;

			transform.prevVelocity = transform.velocity;
			transform.velocity = vec - transform.position;
			transform.direction = transform.position - target_transform.position;
			transform.direction = transform.direction / transform.direction.mag();
		}
	}
}

void Scene_Play::handleLifespan(Entity entity) 
{
	//Lifespan Update
	if (_entityManager.hasComponent<Component::Lifespan>(entity))
	{
		auto& lifespan = _entityManager.getComponent<Component::Lifespan>(entity);

		lifespan.frames--;
		if (lifespan.frames <= 0)
		{

			if (_entityManager.getTag(entity) == "GammaWarning")
			{

				auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);
				auto& warningTransform = _entityManager.getComponent<Component::Transform>(entity);
				Vec2 dir = earthTransform.position - warningTransform.position;
				spawnGamma(dir / dir.mag());
			}

			_entityManager.destroyEntity(entity);
		}
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
	if (actionName == "LEFT")
	{
		for (auto entity : _entityManager.getEntities("Shield"))
		{
			if (action.type == ActionType::KEY_PRESS)
			{
				_entityManager.getComponent<Component::Input>(entity).cw = true;
			}
			else if (action.type == ActionType::KEY_RELEASE)
			{
				_entityManager.getComponent<Component::Input>(entity).cw = false;
			}
		}
	}
	if (actionName == "RIGHT")
	{
		for (auto entity : _entityManager.getEntities("Shield"))
		{
			if (action.type == ActionType::KEY_PRESS)
			{
				_entityManager.getComponent<Component::Input>(entity).ccw = true;
			}
			else if (action.type == ActionType::KEY_RELEASE)
			{
				_entityManager.getComponent<Component::Input>(entity).ccw = false;
			}
		}
	}
}

void Scene_Play::spawnAsteroid()
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	float angle = 60.0f + (float)(rand() % 360) * PI / 180.0f;
	Vec2 pos(700 * sin(angle), 700 * cos(angle));
	pos = earthTransform.position + pos;
	Vec2 vel = (earthTransform.position - pos) / pos.dist(earthTransform.position) * 0.5;

	size_t sprite = rand() % _asteroidSprites.size();

	auto entity = _entityManager.createEntity("Asteroid");
	auto& transform = _entityManager.addComponent<Component::Transform>(entity, Vec2(pos.x + 10, -100), vel, Vec2(1,1), true);
	_entityManager.addComponent<Component::Health>(entity, 1);
	auto & mat = _entityManager.addComponent<Component::Material>(entity, _asteroidSprites[sprite], true);
	_entityManager.addComponent<Component::BoundingBox>(entity, (mat.sprite.getSize()*0.4f));
	
	std::shared_ptr<Cooldown> ani = std::make_shared<Cooldown>(_engine, 120);
	ani->init(entity, _entityManager);
	_entityManager.addComponent<Component::CAnimation>(entity, ani);

	size_t ropeSegs = 20;
	float segDist = (pos.y + _engine->getWindow().getSize().y / 2.0f) / ropeSegs;
	_entityManager.addComponent<Component::Rope>(entity, ropeSegs, segDist,  Vec2(pos.x, -(float)_engine->getWindow().getSize().y/2.0f), &transform.position, &transform.direction, Vec2(0,32), true);

}

void Scene_Play::spawnGammaWarning()
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	float angle = (float)(rand() % 360) * PI / 180.0f;
	Vec2 warningPos(500 * sin(angle), 500 * cos(angle));
	warningPos = warningPos + earthTransform.position;

	auto warning = _entityManager.createEntity("GammaWarning");
	_entityManager.addComponent<Component::Transform>(warning, warningPos, Vec2(0,0), Vec2(0.5,0.5), false);
	_entityManager.addComponent<Component::Material>(warning, _engine->getAssets().getSprite("GammaWarning"), true);
	_entityManager.addComponent<Component::Lifespan>(warning, 5 * 60);
	std::shared_ptr<AnimationBlink> ani = std::make_shared<AnimationBlink>(_engine, 60, 5 * 60);
	_entityManager.addComponent<Component::CAnimation>(warning, ani);
}

void Scene_Play::spawnGamma(Vec2 dir) 
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	Vec2 gammaPos = earthTransform.position - dir*4000;
	gammaPos = gammaPos + earthTransform.position;
	Vec2 vel = earthTransform.position - gammaPos;
	vel = vel / vel.mag() * 50.0f;


	auto gamma = _entityManager.createEntity("GammaRay");
	_entityManager.addComponent<Component::Transform>(gamma, gammaPos, vel, Vec2(4, 6), true);
	auto& mat = _entityManager.addComponent<Component::Material>(gamma, _engine->getAssets().getSprite("GammaRay"), true);
	_entityManager.addComponent<Component::Lifespan>(gamma, 3 * 60);
	
	Vec2 bbSize = mat.sprite.getSize();
	bbSize.x *= 4;
	bbSize.y *= 6;
	_entityManager.addComponent<Component::BoundingBox>(gamma, bbSize );
	std::shared_ptr<Cooldown> ani = std::make_shared<Cooldown>(_engine, 60 * 5);
}