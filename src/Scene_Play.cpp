#include "Scene_Play.h"
#include "engine.h"
#include <iostream>
#include "Physics.h"
#include "Scene_Gameover.h";


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
	registerKeyAction(sf::Keyboard::A, "SATELLITE_LEFT");
	registerKeyAction(sf::Keyboard::D, "SATELLITE_RIGHT");
	registerKeyAction(sf::Keyboard::Space, "SATELLITE_SHOOT");
	registerKeyAction(sf::Keyboard::S, "DOWN");
	registerKeyAction(sf::Keyboard::W, "UP");
	registerKeyAction(sf::Keyboard::Left, "SHIELD_LEFT");
	registerKeyAction(sf::Keyboard::Right, "SHIELD_RIGHT");

	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid1"));
	_asteroidSprites.push_back(_engine->getAssets().getSprite("Asteroid2"));

	_engine->currentScore = 0;

	Vec2 ropePos(_engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y / 2.0f - 148);
	size_t ropeSegs = 20;
	float segDist = ropePos.y / ropeSegs;

	earth = _entityManager.createEntity("Earth");
	auto & earthTransfrom = _entityManager.addComponent<Component::Transform>(earth, Vec2(_engine->getWindow().getSize().x/2 + 250, -200), Vec2(0,0), Vec2(0.8,0.8), true);
	_entityManager.addComponent<Component::Material>(earth, _engine->getAssets().getSprite("Earth"), true);
	_entityManager.addComponent<Component::Health>(earth, 100);
	_entityManager.addComponent<Component::BoundingBox>(earth, Vec2(256, 256));
	_entityManager.addComponent<Component::Rope>(earth, 20, segDist, Vec2(_engine->getWindow().getSize().x / 2, 0), &earthTransfrom.position, &earthTransfrom.direction, Vec2(0, 148), true);
	//std::shared_ptr<AnimationDrop> ani = std::make_shared<AnimationDrop>(_engine, _engine->getWindow().getSize().y / 2);
	//ani->init(earth, _entityManager);
	//_entityManager.addComponent<Component::CAnimation>(earth, ani);

	shield = _entityManager.createEntity("Shield");
	auto & shieldTransform = _entityManager.addComponent<Component::Transform>(shield, Vec2(0, 0), Vec2(0, 0), Vec2(1, 0.7), true);
	_entityManager.addComponent<Component::Material>(shield, _engine->getAssets().getSprite("Shield"), true);
	_entityManager.addComponent<Component::Orbit>(shield, &earthTransfrom.position, 300, 0.03, true, 0, true, true);
	_entityManager.addComponent<Component::Input>(shield);
	_entityManager.addComponent<Component::BoundingBox>(shield, Vec2(250, 50));
	_entityManager.addComponent<Component::PopsicleStick>(shield, &earthTransfrom.position, &shieldTransform.position, false);

	satellite = _entityManager.createEntity("Satellite");
	auto& satelliteTransform = _entityManager.addComponent<Component::Transform>(satellite, Vec2(0, 0), Vec2(0, 0), Vec2(0.4, 0.4), true);
	_entityManager.addComponent<Component::Material>(satellite, _engine->getAssets().getSprite("SatelliteCharge"), true);
	_entityManager.addComponent<Component::Orbit>(satellite, &earthTransfrom.position, 225, 0.03, true, 0, true, true);
	_entityManager.addComponent<Component::Input>(satellite);
	_entityManager.addComponent<Component::PopsicleStick>(satellite, &earthTransfrom.position, &satelliteTransform.position, false);

	currentMusicTrack = rand() % musicTrackCount;
	currentMusic = &_engine->getAssets().getSound("MusicPlay" + std::to_string(currentMusicTrack));
	currentMusic->play();

	timeUntilBoss = 30 * 60;
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

		//AI TICK
		if (_entityManager.hasComponent<Component::CAI>(entity)) 
		{
			_entityManager.getComponent<Component::CAI>(entity).ai->simulate(_engine);
		}


		handleControls(entity);
		handleLifespan(entity);
		handleOrbit(entity);
		handleAnimations(entity);
		handleMovement(entity);
	}

	handleCollisions();


	if (satelliteCooldown > 0) 
	{
		satelliteCooldown--;
	}
	else if (!_entityManager.getComponent<Component::Input>(satellite).canShoot) 
	{
		_engine->getAssets().getSound("SatelliteReady").play();
		_entityManager.getComponent<Component::Input>(satellite).canShoot = true;
		_entityManager.getComponent<Component::Material>(satellite).sprite = _engine->getAssets().getSprite("SatelliteReady");
	}
	if (isBoss) 
	{
		handleBoss();
	}
	else if (!isBoss && !isBossWarning)
	{
		timeUntilAsteroid--;
		if (timeUntilAsteroid <= 0)
		{
			spawnAsteroid();
			int time = (2 * 60 - 10 * difficulty) + rand() % (10 * 60 - 10 * difficulty);
			if (time < 30) { time = 30; }
			timeUntilAsteroid = time;
			
		}

		timeUntilGamma--;
		if (timeUntilGamma <= 0)
		{
			spawnGammaWarning();
			int time = (10 - 0.5 * difficulty) * 60 + rand() % (60 * (20 - difficulty));
			if (time < 5 * 60) { time = 5 * 60; }
			timeUntilGamma = time;
		}

		timeUntilUFO--;
		if (timeUntilUFO <= 0)
		{
			spawnUFO();
			int time = (2 * 60 - 10 * difficulty) + rand() % (10 * 60 - 10 * difficulty);
			if (time < 30) { time = 30; }
			timeUntilUFO = time;
		}

		timeUntilBoss--;
		if (timeUntilBoss <= 0) 
		{
			spawnBossWarning();
			timeUntilBoss = 60 * 60;
		}

		if (currentMusic != nullptr && !isBossWarning)
		{
			if (currentMusic->getStatus() == sf::SoundSource::Status::Stopped)
			{
				currentMusicTrack = (currentMusicTrack + 1) % musicTrackCount;
				currentMusic->stop();
				currentMusic = &_engine->getAssets().getSound("MusicPlay" + std::to_string(currentMusicTrack));
				currentMusic->play();
			}
		}
	}

	_currentFrame++;
	timeAlive++;


	if (_entityManager.getComponent<Component::Health>(earth).health <= 0)
	{
		alive = false;
		_engine->changeScene(std::make_shared<Scene_Gameover>(_engine), "GAME_OVER", true);
	}

}

void Scene_Play::render()
{
	_engine->getWindow().setView(_engine->getWindow().getDefaultView());
	_engine->getWindow().clear(sf::Color(0, 0, 0));

	/*
	Sprite spr = _engine->getAssets().getSprite("Background");
	spr.get().setOrigin({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y / 2.0f });
	spr.get().setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y / 2.0f });
	_engine->getWindow().draw(spr.get());
	*/

	sf::Text text;
	text.setFont(_engine->getAssets().getFont("Crater"));
	text.setOutlineColor(sf::Color(0, 0, 0));
	text.setOutlineThickness(1.0f);
	text.setFillColor(sf::Color(255, 255, 255));

	text.setCharacterSize(18);
	text.setString("Current Score");
	Util::centerText(text);
	text.setPosition({ _engine->getWindowSize().x / 2.0f, _engine->getWindowSize().y - 200.0f });
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

	//Render Before Earth
	for (auto entity : _entityManager.getEntities())
	{
		if (_entityManager.getTag(entity) != "Earth") 
		{
			renderEntity(entity, renderDebug, "PopsicleStick");
		}
	}
	
	//Render Earth
	renderEntity(earth, renderDebug, "Rope");
	renderEntity(earth, renderDebug, "Material");
	renderEntity(earth, renderDebug, "BoundingBox");

	//Render After Earth
	for (auto entity : _entityManager.getEntities())
	{
		if (_entityManager.getTag(entity) != "Earth") 
		{
			renderEntity(entity, renderDebug, "Rope");
			renderEntity(entity, renderDebug, "Material");
			renderEntity(entity, renderDebug, "BoundingBox");
			renderEntity(entity, renderDebug, "CAnimation");
		}
	}

	if (isBoss) 
	{
		renderEntity(boss, renderDebug, "Rope");
		renderEntity(boss, renderDebug, "Material");
		renderEntity(boss, renderDebug, "BoundingBox");
		renderEntity(boss, renderDebug, "CAnimation");
	}

	renderHealth();
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

		//If already destroyed skip
		if (!_entityManager.hasComponent<Component::Material>(entity)) { continue; }

		if (Physics::isColliding(_entityManager, entity, earth))
		{
			//_entityManager.destroyEntity(entity);

			_entityManager.removeComponent<Component::Material>(entity);
			_entityManager.addComponent<Component::Lifespan>(entity, 5 * 60);
			_entityManager.removeComponent<Component::BoundingBox>(entity);
			auto& t = _entityManager.getComponent<Component::Transform>(entity);
			t.velocity.y = -5;

			//Asteroid Explosion
			auto p = _entityManager.createEntity("Particle");
			_entityManager.addComponent<Component::Transform>(p, t.position, Vec2(0, 0), Vec2(1, 1), false);
			_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);
			size_t soundIndex = rand() % 6;
			_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();

			_entityManager.getComponent<Component::Health>(earth).damage(1);
		}

		if (Physics::isColliding(_entityManager, entity, shield))
		{	
			//_entityManager.destroyEntity(entity);

			//Asteroid Remove Animation
			_entityManager.removeComponent<Component::Material>(entity);
			_entityManager.addComponent<Component::Lifespan>(entity, 5 * 60);
			_entityManager.removeComponent<Component::BoundingBox>(entity);
			auto& t = _entityManager.getComponent<Component::Transform>(entity);
			t.velocity.y = -5;

			//Asteroid Explosion
			Entity p = _entityManager.createEntity("Particle");
			_entityManager.addComponent<Component::Transform>(p, t.position, Vec2(0, 0), Vec2(1, 1), false);
			_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);
			size_t soundIndex = rand() % 6;
			_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();


			auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);

			//Particle
			Vec2 dir = earthTransform.position - transform.position;
			dir = dir / dir.mag();

			auto part = _entityManager.createEntity("Particle");
			_entityManager.addComponent<Component::Transform>(part, transform.position + dir * 32, Vec2(0,0), Vec2(0.5, 0.5), false);
			_entityManager.addComponent<Component::Material>(part, _engine->getAssets().getSprite("BlueExplosion"), false);


			_engine->currentScore++;
		}
	}

	for (Entity entity : _entityManager.getEntities("GammaRay")) 
	{
		auto& mat = _entityManager.getComponent<Component::Material>(entity);
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);
		auto& shieldTransform = _entityManager.getComponent<Component::Transform>(shield);
		auto& shieldMat = _entityManager.getComponent<Component::Material>(shield);

		if (Physics::isColliding(_entityManager, entity, shield) && abs(((int)(abs(transform.direction.angle(shieldTransform.direction)) * 180.0f / PI) % 360) - 180) < 90)
		{

			mat.crop.top += transform.velocity.mag() / 6.0f;
			mat.crop.height -= transform.velocity.mag() / 6.0f;
			transform.position = transform.position - transform.velocity;
			if (mat.crop.height <= 0)
			{
				mat.crop.height = 0;
				_entityManager.destroyEntity(entity);
			}

			//Shield Particle
			if (_entityManager.getEntities("ParticleShieldHit").size() <= 10){
				float y = shieldMat.sprite.getSize().y / 2.0f * shieldTransform.scale.y;
				float angle = Vec2(0, -1).angle(shieldTransform.direction);
				Vec2 pos = shieldTransform.position - Vec2(-y * sin(angle), y * cos(angle));

				auto p = _entityManager.createEntity("ParticleShieldHit");
				auto& t = _entityManager.addComponent<Component::Transform>(p, pos, Vec2(0, 0), Vec2(1, 0.7), true);
				t.direction = shieldTransform.direction;
				_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("ShieldHit"), false);
			}
		} 
		else if (Physics::isColliding(_entityManager, entity, earth))
		{
			auto& health = _entityManager.getComponent<Component::Health>(earth);
			health.health -= 3;
		}
	}

	for (Entity entity : _entityManager.getEntities("Bullet")) 
	{
		if (Physics::isColliding(_entityManager, entity, shield))
		{
			_entityManager.destroyEntity(entity);

			auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);
			auto& mat = _entityManager.getComponent<Component::Material>(entity);

			//Particle
			Vec2 dir = earthTransform.position - transform.position;
			dir = dir / dir.mag();

			auto p = _entityManager.createEntity("Particle");
			_entityManager.addComponent<Component::Transform>(p, transform.position + dir * mat.sprite.getSize().y * transform.scale.y, Vec2(0, 0), Vec2(0.5, 0.5), false);
			_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("BlueExplosion"), false);
		}
		else if (Physics::isColliding(_entityManager, entity, earth)) 
		{
			_entityManager.destroyEntity(entity);
			auto& health = _entityManager.getComponent<Component::Health>(earth);
			health.health -= 1;
		}
	}

	for (Entity entity : _entityManager.getEntities("SatelliteBullet"))
	{
		for (Entity enemy : _entityManager.getEntities("Asteroid")) 
		{
			if (Physics::isColliding(_entityManager, entity, enemy))
			{
				_entityManager.destroyEntity(entity);
				//_entityManager.destroyEntity(enemy);

				_entityManager.removeComponent<Component::Material>(enemy);
				_entityManager.addComponent<Component::Lifespan>(enemy, 5 * 60);
				_entityManager.removeComponent<Component::BoundingBox>(enemy);
				auto& t = _entityManager.getComponent<Component::Transform>(enemy);
				t.velocity.y = -5;

				Entity p = _entityManager.createEntity("Particle");
				_entityManager.addComponent<Component::Transform>(p, t.position, Vec2(0, 0), Vec2(1, 1), false);
				_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);
				size_t soundIndex = rand() % 6;
				_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();

				_engine->currentScore++;
			}
		}
		
		for (Entity enemy : _entityManager.getEntities("UFO"))
		{
			if (Physics::isColliding(_entityManager, entity, enemy))
			{
				_entityManager.destroyEntity(entity);
				//_entityManager.destroyEntity(enemy);

				_entityManager.removeComponent<Component::Material>(enemy);
				_entityManager.addComponent<Component::Lifespan>(enemy, 5 * 60);
				_entityManager.removeComponent<Component::BoundingBox>(enemy);
				_entityManager.removeComponent<Component::CAI>(enemy);
				_entityManager.removeComponent<Component::Orbit>(enemy);
				auto& t = _entityManager.getComponent<Component::Transform>(enemy);
				t.velocity.y = -5;

				auto p = _entityManager.createEntity("Particle");
				_entityManager.addComponent<Component::Transform>(p, t.position, Vec2(0, 0), Vec2(1, 1), false);
				_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);
				size_t soundIndex = rand() % 6;
				_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();

				_engine->currentScore++;
			}
		}
	}

	for (Entity entity : _entityManager.getEntities("Health")) 
	{
		if (Physics::isColliding(_entityManager, entity, earth))
		{
			auto& h = _entityManager.getComponent<Component::Health>(earth);
			h.health = h.maxHealth;
			_engine->getAssets().getSound("GetHealth").play();
			_entityManager.destroyEntity(entity);
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

			if (!_entityManager.hasComponent<Component::Orbit>(entity) && transform.velocity.mag() != 0) {
				transform.direction = transform.velocity / transform.velocity.mag();
			}
		}

	}
}

void Scene_Play::handleControls(Entity entity)
{
	if (_entityManager.hasComponent<Component::Input>(entity))
	{
		auto& input = _entityManager.getComponent<Component::Input>(entity);

		if (_entityManager.hasComponent<Component::Orbit>(entity))
		{
			auto& orbit = _entityManager.getComponent<Component::Orbit>(entity);
			if (input.ccw != input.cw)
			{
				orbit.clockWise = input.cw;
				orbit.moving = true;
			}
			else
			{
				orbit.moving = false;
			}
		}

		if (entity == satellite)
		{
			if (input.canShoot && input.shoot) 
			{
				auto& transform = _entityManager.getComponent<Component::Transform>(entity);

				auto bullet = _entityManager.createEntity("SatelliteBullet");
				auto &mat =_entityManager.addComponent<Component::Material>(bullet, _engine->getAssets().getSprite("Laser"), true);
				_entityManager.addComponent<Component::Transform>(bullet, transform.position + transform.direction * mat.sprite.getSize().y, transform.direction * 40.0f, Vec2(1,1), true);
				_entityManager.addComponent<Component::BoundingBox>(bullet, mat.sprite.getSize());
				_entityManager.addComponent<Component::Lifespan>(bullet, 3 * 60);
				
				int num = rand() % 4 + 1;
				_engine->getAssets().getSound("Laser" + std::to_string(num)).play();

				satelliteCooldown = satelliteCooldownMax;
				input.canShoot = false;
				_entityManager.getComponent<Component::Material>(entity).sprite = _engine->getAssets().getSprite("SatelliteCharge");

			}
		}
	}
}

void Scene_Play::handleOrbit(Entity entity)
{

	if (_entityManager.hasComponent<Component::CAnimation>(entity)) 
	{
		return;
	}

	if (_entityManager.hasComponent<Component::Orbit>(entity))
	{
		auto& orbit = _entityManager.getComponent<Component::Orbit>(entity);
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);

		float nextAngle = orbit.currentAngle;

		if (orbit.moving) {
			if (orbit.clockWise) { nextAngle = orbit.currentAngle + orbit.speed; }
			else { nextAngle = orbit.currentAngle - orbit.speed; }
		}

		if (nextAngle > 2.0f * PI) { nextAngle -= 2.0f * PI; }
		else if (nextAngle < 0) { nextAngle += 2.0f * PI; }

		Vec2 curr(orbit.distance * sin(orbit.currentAngle), orbit.distance * cos(orbit.currentAngle));
		curr = *orbit.target + curr;

		Vec2 next(orbit.distance * sin(nextAngle), orbit.distance * cos(nextAngle));
		next = *orbit.target + next;

		transform.prevVelocity = transform.velocity;
		if (orbit.stickToDistance) {
			transform.velocity = next - transform.position;
		}
		else 
		{
			transform.velocity = next - curr;
		}
		
		if (orbit.transformDirection) {
			transform.direction = transform.position - *orbit.target;
			transform.direction = transform.direction / transform.direction.mag();
		}

		orbit.currentAngle = nextAngle;
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
			else if (_entityManager.getTag(entity) == "BossWarning") 
			{
				spawnBoss();
			}

			_entityManager.destroyEntity(entity);
		}
	}

	//Invincibility
	if (_entityManager.hasComponent<Component::Invincibility>(entity))
	{
		auto& inv = _entityManager.getComponent<Component::Invincibility>(entity);
		inv.frames--;
		if (inv.frames == 0) 
		{
			_entityManager.removeComponent<Component::Invincibility>(entity);
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
	else if (actionName == "DEBUG" && action.type == ActionType::KEY_PRESS) 
	{
		renderDebug = !renderDebug;
	}
	else if (actionName == "SHIELD_LEFT")
	{
		if (action.type == ActionType::KEY_PRESS)
		{
			_entityManager.getComponent<Component::Input>(shield).cw = true;
		}
		else if (action.type == ActionType::KEY_RELEASE)
		{
			_entityManager.getComponent<Component::Input>(shield).cw = false;
		}
	}
	else if (actionName == "SHIELD_RIGHT")
	{
		if (action.type == ActionType::KEY_PRESS)
		{
			_entityManager.getComponent<Component::Input>(shield).ccw = true;
		}
		else if (action.type == ActionType::KEY_RELEASE)
		{
			_entityManager.getComponent<Component::Input>(shield).ccw = false;
		}
	}
	else if (actionName == "SATELLITE_LEFT")
	{
		if (action.type == ActionType::KEY_PRESS)
		{
			_entityManager.getComponent<Component::Input>(satellite).cw = true;
		}
		else if (action.type == ActionType::KEY_RELEASE)
		{
			_entityManager.getComponent<Component::Input>(satellite).cw = false;
		}
	}
	else if (actionName == "SATELLITE_RIGHT") 
	{
		if (action.type == ActionType::KEY_PRESS)
		{
			_entityManager.getComponent<Component::Input>(satellite).ccw = true;
		}
		else if (action.type == ActionType::KEY_RELEASE)
		{
			_entityManager.getComponent<Component::Input>(satellite).ccw = false;
		}
	}
	else if (actionName == "SATELLITE_SHOOT") 
	{
		if (action.type == ActionType::KEY_PRESS)
		{
			_entityManager.getComponent<Component::Input>(satellite).shoot = true;
		}
		else if (action.type == ActionType::KEY_RELEASE)
		{
			_entityManager.getComponent<Component::Input>(satellite).shoot = false;
		}
	}
}


void Scene_Play::handleBoss()
{
	if (bossType == BossType::MOTHERSHIP) 
	{
		auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);
		auto& mothershipTransform = _entityManager.getComponent<Component::Transform>(boss);
		auto& shieldTransform = _entityManager.getComponent<Component::Transform>(shield);

		for (Entity entity : _entityManager.getEntities("MothershipLaser")) 
		{
			auto& transform = _entityManager.getComponent<Component::Transform>(entity);
			auto& mat = _entityManager.getComponent<Component::Material>(entity);

			transform.position = mothershipTransform.position + (earthTransform.position - mothershipTransform.position) / 2.0f;
			transform.direction = (earthTransform.position - mothershipTransform.position) / (earthTransform.position - mothershipTransform.position).mag();

			if (Physics::isColliding(_entityManager, entity, shield) && abs(((int)(abs(transform.direction.angle(shieldTransform.direction)) * 180.0f / PI) % 360) - 180) < 90)
			{	
				Vec2 pos = mothershipTransform.position + (earthTransform.position - mothershipTransform.position) / 2.0f;

				float spriteSize = mat.sprite.getSize().y;

				//float crop = spriteSize / 2.0f - pos.dist(shieldTransform.position);
				float crop = shieldTransform.position.dist(earthTransform.position);

				Vec2 a = mothershipTransform.position - earthTransform.position;
				a = a / a.mag();

				mat.crop.top = crop;
				transform.position = pos + a * crop;

				if (_entityManager.getEntities("ParticleMothershipBlock").size() < 10) {
					//Particle
					Vec2 dir = mothershipTransform.position - earthTransform.position;
					dir = dir / dir.mag();
					Vec2 pos = earthTransform.position + dir * (shieldTransform.position - earthTransform.position).mag();

					auto p = _entityManager.createEntity("ParticleMothershipBlock");
					_entityManager.addComponent<Component::Transform>(p, pos, Vec2(0, 0), Vec2(0.8, 0.8), false);
					_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("BlueExplosion"), false);
				}
			}
			else 
			{
				_entityManager.getComponent<Component::Health>(earth).damage(0.05f);
				mat.crop.top = 0;
				transform.position = mothershipTransform.position + (earthTransform.position - mothershipTransform.position) / 2.0f;
			}
		}

		for (Entity entity : _entityManager.getEntities("SatelliteBullet"))
		{
			bool hitShield = false;

			for (Entity shield : _entityManager.getEntities("MothershipShield"))
			{
				if (Physics::isColliding(_entityManager, entity, shield)) 
				{
					hitShield = true;
					_entityManager.addComponent<Component::Invincibility>(shield, 5);
					_entityManager.destroyEntity(entity);
				}
			}
			if (!hitShield && Physics::isColliding(_entityManager, entity, boss))
			{
				_entityManager.destroyEntity(entity);
				_entityManager.getComponent<Component::Health>(boss).damage(1);
				//_entityManager.addComponent<Component::Invincibility>(boss, 10);

				auto& t = _entityManager.getComponent<Component::Transform>(entity);
				auto& m = _entityManager.getComponent<Component::Material>(entity);
				auto p = _entityManager.createEntity("Particle");
				_entityManager.addComponent<Component::Transform>(p, t.position + t.direction * m.sprite.getSize().y / 2, Vec2(0, 0), Vec2(0.5, 0.5), false);
				_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);

				size_t soundIndex = rand() % 6;
				_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();
			}
		}
	}

	if (_entityManager.getComponent<Component::Health>(boss).health <= 0) 
	{
		//_entityManager.destroyEntity(boss);
		isBoss = false;
		difficulty++;
		_engine->stopSounds();

		_engine->currentScore+=50;

		currentMusic->stop();
		currentMusic = &_engine->getAssets().getSound("BossWin");
		currentMusic->play();

		spawnHealth();


		if (bossType == BossType::MOTHERSHIP) 
		{
			//remove any lazers shooting
			for (Entity entity : _entityManager.getEntities("MothershipLaser")) 
			{
				_entityManager.destroyEntity(entity);
			}

			//Remove Shield
			for (Entity shield : _entityManager.getEntities("MothershipShield")) 
			{
				_entityManager.destroyEntity(shield);
			}

			//Remove Animation
			_entityManager.removeComponent<Component::Material>(boss);
			_entityManager.addComponent<Component::Lifespan>(boss, 4 * 60);
			_entityManager.removeComponent<Component::Orbit>(boss);
			_entityManager.removeComponent<Component::BoundingBox>(boss);
			_entityManager.removeComponent<Component::CAI>(boss);
			auto& t = _entityManager.getComponent<Component::Transform>(boss);
			t.velocity.y = -5;

			//Explosion
			Entity p = _entityManager.createEntity("Particle");
			_entityManager.addComponent<Component::Transform>(p, t.position, Vec2(0, 0), Vec2(2, 2), false);
			_entityManager.addComponent<Component::Material>(p, _engine->getAssets().getSprite("Explosion"), false);
			size_t soundIndex = rand() % 6;
			_engine->getAssets().getSound("Explode" + std::to_string(soundIndex)).play();
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
	Vec2 warningPos(400 * sin(angle), 400 * cos(angle));
	warningPos = warningPos + earthTransform.position;

	auto warning = _entityManager.createEntity("GammaWarning");
	_entityManager.addComponent<Component::Transform>(warning, warningPos, Vec2(0,0), Vec2(0.5,0.5), false);
	_entityManager.addComponent<Component::Material>(warning, _engine->getAssets().getSprite("GammaWarning"), true);
	_entityManager.addComponent<Component::Lifespan>(warning, 5 * 60);
	std::shared_ptr<AnimationBlink> ani = std::make_shared<AnimationBlink>(_engine, 60, 5 * 60);
	_entityManager.addComponent<Component::CAnimation>(warning, ani);

	_engine->getAssets().getSound("GammaWarning").play();
}

void Scene_Play::spawnGamma(Vec2 dir) 
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	Vec2 gammaPos = earthTransform.position - dir*2000;
	Vec2 vel = dir * 40.0f;


	auto gamma = _entityManager.createEntity("GammaRay");
	_entityManager.addComponent<Component::Transform>(gamma, gammaPos, vel, Vec2(4, 6), true);
	auto& mat = _entityManager.addComponent<Component::Material>(gamma, _engine->getAssets().getSprite("GammaRay"), true);
	_entityManager.addComponent<Component::Lifespan>(gamma, 3 * 60);
	
	Vec2 bbSize = mat.sprite.getSize();
	bbSize.x *= 4;
	bbSize.y *= 6;
	_entityManager.addComponent<Component::BoundingBox>(gamma, bbSize );
	std::shared_ptr<Cooldown> ani = std::make_shared<Cooldown>(_engine, 60 * 5);

	_engine->getAssets().getSound("GammaRay").play();
}

void Scene_Play::spawnUFO() 
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	float angle = (float)(rand() % 360) * PI / 180.0f;
	float dist = 300 + rand() % 200;

	Vec2 pos(dist * sin(angle), dist * cos(angle));
	pos = earthTransform.position + pos;
	
	auto entity = _entityManager.createEntity("UFO");
	auto& transform = _entityManager.addComponent<Component::Transform>(entity, Vec2(pos.x + 10, -100), Vec2(0,0), Vec2(1, 1), true);
	_entityManager.addComponent<Component::Health>(entity, 1);
	auto& mat = _entityManager.addComponent<Component::Material>(entity, _engine->getAssets().getSprite("UFO"), true);
	_entityManager.addComponent<Component::BoundingBox>(entity, (mat.sprite.getSize() * 0.6f));

	std::shared_ptr<Cooldown> ani = std::make_shared<Cooldown>(_engine, 120);
	ani->init(entity, _entityManager);
	_entityManager.addComponent<Component::CAnimation>(entity, ani);

	std::shared_ptr<AIUFO> ai = std::make_shared<AIUFO>(&transform.position, &earthTransform.position, &_entityManager);
	_entityManager.addComponent<Component::CAI>(entity, ai);

	size_t ropeSegs = 20;
	float segDist = (_engine->getWindow().getSize().y) / ropeSegs;
	_entityManager.addComponent<Component::Rope>(entity, ropeSegs, segDist, Vec2(pos.x, pos.y - segDist*ropeSegs), &transform.position, &transform.direction, Vec2(0, 28), true);

	Vec2 orbitTarget = earthTransform.position;
	orbitTarget.y = orbitTarget.y - _engine->getWindow().getSize().y;
	auto& orbit = _entityManager.addComponent<Component::Orbit>(entity, &earthTransform.position, dist, 0.01f, (bool)(rand() % 2), angle, false, false);
	orbit.moving = true;
}

void Scene_Play::spawnBoss() 
{
	if (bossType == BossType::MOTHERSHIP) 
	{
		auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);
		
		float dist = 500;
		float angle = (rand() % 2) == 0 ? PI / 2.0f : 3.0f * PI / 2.0f;
		Vec2 pos(dist * sin(angle), dist * cos(angle));
		pos = earthTransform.position + pos;

		size_t ropeSegs = 20;
		float segDist = (_engine->getWindow().getSize().y) / ropeSegs;

		float startAngle = (angle == PI/2.0f ? -PI/2.0f:PI/2.0f);
		Vec2 ropeVec = (pos - Vec2(pos.x, pos.y - segDist * ropeSegs));
		Vec2 startPos = Vec2(pos.x, pos.y - segDist * ropeSegs) + Vec2(ropeVec.x * cos(startAngle) - ropeVec.y * sin(startAngle), ropeVec.x * sin(startAngle) + ropeVec.y * cos(startAngle)) * 0.9f;


		//CREATE MOTHERSHIP SHIELD
		auto mShield = _entityManager.createEntity("MothershipShield");
		_entityManager.addComponent<Component::Transform>(mShield, Vec2(startPos), Vec2(0, 0), Vec2(1.1, 1.1), false);
		_entityManager.addComponent<Component::Material>(mShield, _engine->getAssets().getSprite("MothershipShield"), true);
		_entityManager.addComponent<Component::BoundingBox>(mShield, _engine->getAssets().getSprite("MothershipShield").getSize());

		//CREATE MOTHERSHIP
		boss = _entityManager.createEntity("Boss");
		auto& transform = _entityManager.addComponent<Component::Transform>(boss, startPos, Vec2(0, 0), Vec2(1, 1), true);
		auto& mat = _entityManager.addComponent<Component::Material>(boss, _engine->getAssets().getSprite("Mothership"), true);
		_entityManager.addComponent<Component::Health>(boss, 50);
		_entityManager.addComponent<Component::Rope>(boss, ropeSegs, segDist, Vec2(pos.x, pos.y - segDist * ropeSegs), &transform.position, &transform.direction, Vec2(0, 0), true);
		_entityManager.addComponent<Component::BoundingBox>(boss, mat.sprite.getSize() * 0.8);
		auto& orbit = _entityManager.addComponent<Component::Orbit>(boss, &earthTransform.position, dist, 0.001f, (bool)(rand() % 2), angle, false, false);
		orbit.moving = true;

		std::shared_ptr<Cooldown> ani = std::make_shared<Cooldown>(_engine, 120);
		ani->init(boss, _entityManager);
		_entityManager.addComponent<Component::CAnimation>(boss, ani);

		std::shared_ptr<AIMothership> ai = std::make_shared<AIMothership>(boss, mShield, &earthTransform.position, &_entityManager);
		_entityManager.addComponent<Component::CAI>(boss, ai);
	}

	currentMusic->stop();
	size_t bossMusicTrack = rand() % 3;
	currentMusic = &_engine->getAssets().getSound("BossMusic" + std::to_string(bossMusicTrack));
	currentMusic->play();

	musicFade = 1.0f;
	isBoss = true;
	isBossWarning = false;
}

void Scene_Play::spawnBossWarning()
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	size_t bossSelection = rand() % 1;

	auto warning = _entityManager.createEntity("BossWarning");
	_entityManager.addComponent<Component::Transform>(warning, Vec2(_engine->getWindowSize().x / 2, _engine->getWindowSize().y / 2), Vec2(0, 0), Vec2(1, 1), false);
	_entityManager.addComponent<Component::Material>(warning, _engine->getAssets().getSprite("MothershipWarning"), true);
	_entityManager.addComponent<Component::Lifespan>(warning, 60 * 3);
	_entityManager.addComponent<Component::Lifespan>(warning, 5 * 60);
	std::shared_ptr<AnimationBlink> ani = std::make_shared<AnimationBlink>(_engine, 60, 5 * 60);
	_entityManager.addComponent<Component::CAnimation>(warning, ani);

	if (bossSelection == 0) 
	{
		bossType = BossType::MOTHERSHIP;
	}

	isBossWarning = true;
	currentMusic->stop();
	_engine->getAssets().getSound("BossWarning").play();
}

void Scene_Play::spawnHealth()
{
	auto& earthTransform = _entityManager.getComponent<Component::Transform>(earth);

	float dist = 500;
	float angle = (rand() % 2) == 0 ? PI / 2.0f : 3.0f * PI / 2.0f;
	Vec2 pos(dist * sin(angle), dist * cos(angle));
	pos = earthTransform.position + pos;

	size_t ropeSegs = 20;
	float segDist = (_engine->getWindow().getSize().y) / ropeSegs;

	float startAngle = (angle == PI / 2.0f ? -PI / 2.0f : PI / 2.0f);
	Vec2 ropeVec = (pos - Vec2(pos.x, pos.y - segDist * ropeSegs));
	Vec2 startPos = Vec2(pos.x, pos.y - segDist * ropeSegs) + Vec2(ropeVec.x * cos(startAngle) - ropeVec.y * sin(startAngle), ropeVec.x * sin(startAngle) + ropeVec.y * cos(startAngle)) * 0.9f;

	Vec2 vel = earthTransform.position - pos;
	vel = vel / vel.mag() * 3.0f;

	auto health = _entityManager.createEntity("Health");
	auto& transform = _entityManager.addComponent<Component::Transform>(health, startPos, vel, Vec2(1, 1), true);
	auto& mat = _entityManager.addComponent<Component::Material>(health, _engine->getAssets().getSprite("Health"), true);
	_entityManager.addComponent<Component::Rope>(health, ropeSegs, segDist, Vec2(pos.x, pos.y - segDist * ropeSegs), &transform.position, &transform.direction, Vec2(0, 0), true);
	_entityManager.addComponent<Component::BoundingBox>(health, mat.sprite.getSize() * 0.2);
}
