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

void Scene::renderEntity(const int & entity, bool debug, std::string renderOnly) 
{
	//Render Sprite of Entity
	if (_entityManager.hasComponent<Component::Material>(entity) && (renderOnly == "ALL" || renderOnly == "Material"))
	{
		auto& mat = _entityManager.getComponent<Component::Material>(entity);
		auto& transfrom = _entityManager.getComponent<Component::Transform>(entity);

		mat.sprite.get().setOrigin((mat.sprite.getSize().x) / 2, (mat.sprite.getSize().y) / 2);
		mat.sprite.get().setPosition(transfrom.position.x, transfrom.position.y);
		mat.sprite.get().setScale({ transfrom.scale.x, transfrom.scale.y });

		if (transfrom.rotates) 
		{
			mat.sprite.get().setRotation(Vec2(0, -1).angle(transfrom.direction) * 180.0f / PI);
		}

		if (mat.fragShader != nullptr) 
		{
			_engine->getWindow().draw(mat.sprite.get(), &*mat.fragShader);
		}
		else
		{
			_engine->getWindow().draw(mat.sprite.get());
		}
	}

	if (debug && _entityManager.hasComponent<Component::BoundingBox>(entity) && (renderOnly == "ALL" || renderOnly == "BoundingBox"))
	{
		auto& boundingbox = _entityManager.getComponent<Component::BoundingBox>(entity);
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);

		sf::RectangleShape rect;
		rect.setSize({ boundingbox.size.x, boundingbox.size.y });
		rect.setOutlineColor(sf::Color(255, 0, 0));
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineThickness(2);
		rect.setOrigin(boundingbox.halfSize.x, boundingbox.halfSize.y);
		rect.setPosition({ transform.position.x, transform.position.y });
		if (transform.rotates) 
		{
			rect.setRotation(Vec2(0, -1).angle(transform.direction) * 180.f / PI);
		}
		_engine->getWindow().draw(rect);
	}

	if (_entityManager.hasComponent<Component::Rope>(entity) && (renderOnly == "ALL" || renderOnly == "Rope"))
	{
		
		auto& rope = _entityManager.getComponent<Component::Rope>(entity);

		sf::RectangleShape line;
		line.setFillColor(sf::Color(196, 169, 137));
		for (int i = 1; i < rope.ropeLength; i++) 
		{
			Vec2 diff = rope.segmentPositions[i] - rope.segmentPositions[i - 1];
			Vec2 pos = rope.segmentPositions[i - 1] + (diff/2.0f);
			line.setSize({5, diff.mag()});
			line.setOrigin({ 2.5, diff.mag() / 2 });
			line.setRotation(Vec2(0,-1).angle(diff) * 180 / PI);
			line.setPosition({pos.x, pos.y});
			_engine->getWindow().draw(line);
		}

		if (debug) {
			sf::CircleShape circle;
			circle.setOutlineColor(sf::Color(0, 255, 0));
			circle.setRadius(5);
			circle.setOutlineThickness(2);
			circle.setOrigin({ 5, 5 });
			for (Vec2 pos : rope.segmentPositions)
			{
				circle.setPosition({ pos.x, pos.y });
				_engine->getWindow().draw(circle);
			}
		}
	}

	if (debug && _entityManager.hasComponent<Component::CAnimation>(entity) && (renderOnly == "ALL" || renderOnly == "CAnimation"))
	{
		auto& transform = _entityManager.getComponent<Component::Transform>(entity);

		sf::RectangleShape rect;
		rect.setSize({ 10, 10 });
		rect.setFillColor(sf::Color(0, 255, 255));
		rect.setOrigin(2.5, 2.5);
		rect.setPosition({ transform.position.x, transform.position.y });
		_engine->getWindow().draw(rect);
	}

	if (_entityManager.hasComponent<Component::PopsicleStick>(entity) && (renderOnly == "ALL" || renderOnly == "PopsicleStick"))
	{
		auto& stick = _entityManager.getComponent<Component::PopsicleStick>(entity);

		Vec2 diff = *stick.attachment - *stick.anchor;
		Vec2 dir = diff / diff.mag();

		Sprite mid = _engine->getAssets().getSprite("PopStickMid");
		mid.get().setOrigin(mid.getSize().x / 2, mid.getSize().y / 2);
		mid.get().setRotation(Vec2(0, -1).angle(diff) * 180.0f / PI);

		Sprite end = _engine->getAssets().getSprite("PopStickEnd");
		end.get().setOrigin(end.getSize().x / 2.0f, end.getSize().y / 2.0f);
		end.get().setRotation(Vec2(0, -1).angle(diff) * 180.0f / PI);

		float midCount = (diff.mag() - end.getSize().y) / mid.getSize().x;
		int midCountInt = midCount;
		if (midCount - (float)midCountInt > 0.5f) { midCount++; }
		float midScale = 1.0f + (midCount - (float)midCountInt) / ((float)midCountInt - 1);

		Vec2 startPos = *stick.anchor + (dir * end.getSize().y / 2.0f) + (dir * (mid.getSize().y * midScale / 2.0f));
		for (int i = 0; i < midCountInt; i++) 
		{
			if (i % 2 == 0) { mid.get().setScale(1, midScale); }
			else { mid.get().setScale(1, -midScale); }

			Vec2 pos = startPos + dir * (mid.getSize().y*midScale) * i;
			mid.get().setPosition({pos.x, pos.y});
			_engine->getWindow().draw(mid.get());

			if (debug) {
				sf::RectangleShape rect;
				rect.setSize({ mid.getSize().x, mid.getSize().y * midScale });
				rect.setOrigin(mid.getSize().x / 2, mid.getSize().y / 2);
				rect.setRotation(Vec2(0, -1).angle(diff) * 180.0f / PI);
				rect.setOutlineColor(sf::Color(255, 0, 0));
				rect.setOutlineThickness(2);
				rect.setFillColor(sf::Color(0, 0, 0, 0));
				rect.setPosition({ pos.x, pos.y });
				_engine->getWindow().draw(rect);
			}
		}

		end.get().setPosition({(*stick.attachment + dir).x, (*stick.attachment + dir).y });
		_engine->getWindow().draw(end.get());
		end.get().setScale(1, -1);
		end.get().setPosition({ (*stick.anchor + dir).x, (*stick.anchor + dir).y });
		_engine->getWindow().draw(end.get());

		/*
		sf::RectangleShape rect;
		rect.setSize({ 10, diff.mag() });
		rect.setOrigin(5, diff.mag() / 2.0f);
		rect.setRotation(Vec2(0,-1).angle(diff) * 180.0f / PI);
		rect.setFillColor(sf::Color(255, 0, 0));
		rect.setPosition({ pos.x, pos.y });
		_engine->getWindow().draw(rect);
		*/
	}
}