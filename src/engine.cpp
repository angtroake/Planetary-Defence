#include "engine.h"
#include "Scene_Main_Menu.h"
#include <iostream>
#include <fstream>

GameEngine::GameEngine()
{
	init();
	_isRunning = true;
}

void GameEngine::init() 
{

	std::ifstream f("best_score.txt");
	while (f.good()) 
	{
		f >> bestScore;
	}
	f.close();


	//std::vector<sf::VideoMode> videoModes = sf::VideoMode::getFullscreenModes();
	_window.create(sf::VideoMode(1920, 1080), "GameJam", sf::Style::Fullscreen);
	//_window.create(sf::VideoMode(1280, 720), "GameJam", sf::Style::Fullscreen);
	_window.setFramerateLimit(60);

	_assets.init();

	changeScene(std::make_shared<Scene_Main_Menu>(this), "MAIN_MENU", false);
}

void GameEngine::run() 
{
	while (_isRunning && _window.isOpen()) 
	{
		update();

		if (currentScore > bestScore) 
		{
			bestScore = currentScore;
		}
	}

	std::ofstream f("best_score.txt");
	f << bestScore;
	f.close();

}

void GameEngine::update() 
{
	if (_sceneMap.empty()) { return; }

	if (soundFadeFactor > 0.0 && fadeSounds) 
	{
		soundFadeFactor -= soundFadeDecrease;
		getAssets().setGlobalSoundVolume(100 * soundFadeFactor);
	}
	else if(fadeSounds)
	{
		fadeSounds = false;
		soundFadeFactor = 0;
		stopSounds();
		getAssets().setGlobalSoundVolume(20);
	}

	handleInput();


	if (!_isRunning) { return; }

	_sceneMap[_activeScene]->simulate(SIM_SPEED);
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
			std::cerr << "Scene " << name << " does not exist!" << std::endl;
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

void GameEngine::fadePlayingSounds()
{
	fadeSounds = true;
	soundFadeFactor = 1.0f;
}

void GameEngine::stopSounds()
{
	getAssets().stopAllSounds();
}
