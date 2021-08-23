#include "engine.h"
#include "Scene_Main_Menu.h"

GameEngine::GameEngine()
{
	init();
	_isRunning = true;
}

void GameEngine::init() 
{
	//std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();
	//_window.create(videoModes[0], "GameJam");
	_window.create(sf::VideoMode(1280, 720), "GameJam");
	_window.setFramerateLimit(60);

	_assets.init();

	changeScene(std::make_shared<Scene_Main_Menu>(this), "MAIN_MENU", false);
}

void GameEngine::run() 
{
	while (_isRunning) 
	{
		update();
	}
}

void GameEngine::update() 
{
	if (_sceneMap.empty()) { return; }

	handleInput();

	if (!_isRunning) { return; }

	_sceneMap[_activeScene]->tick();
	_sceneMap[_activeScene]->render();

	_window.display();

}


void GameEngine::handleInput() 
{
	sf::Event event;
	while (_window.pollEvent(event)) 
	{
		if (event.type == sf::Event::Closed) 
		{
			quit();
		}
		else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) 
		{

			KeyAction action;
			action.keyCode = event.key.code;
			action.type = event.type == sf::Event::KeyPressed ? ActionType::KEY_PRESS : ActionType::KEY_RELEASE;

			if (getActiveScene()->getKeyActionMap().find(action.keyCode) != getActiveScene()->getKeyActionMap().end())
			{
				getActiveScene()->onKeyAction(getActiveScene()->getKeyActionMap().at(event.key.code), action);
			}
		}
	}
}

void GameEngine::quit() 
{
	_isRunning = false;
}

void GameEngine::changeScene(std::shared_ptr<Scene> scene, const std::string& name, bool endScene) 
{
	if (scene != nullptr) 
	{
		_previousScene = _activeScene;
		_sceneMap[name] = scene;
	}
	else
	{
		if (_sceneMap.find(name) == _sceneMap.end()) 
		{
			return;
		}
	}

	if (endScene) 
	{
		_sceneMap.erase(_sceneMap.find(_activeScene));
	}

	_activeScene = name;
}

std::shared_ptr<Scene> GameEngine::getActiveScene() 
{
	return _sceneMap[_activeScene];
}

sf::RenderWindow & GameEngine::getWindow()
{
	return _window;
}

AssetManager & GameEngine::getAssets() 
{
	return _assets;
}

const sf::Vector2u & GameEngine::getWindowSize() const 
{
	return _window.getSize();
}