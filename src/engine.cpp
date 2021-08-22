#include "engine.h"
#include "Scene_Main_Menu.h"

GameEngine::GameEngine()
{
	init();
	_isRunning = true;
}

void GameEngine::init() 
{
	std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();
	_window.create(videoModes[0], "GameJam");
	_window.setFramerateLimit(60);

	changeScene(std::make_shared<Scene_Main_Menu>(this), "MAIN_MENU", true);
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
		else if (event.type == sf::Event::KeyPressed) 
		{
		
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