#include "base.h"
#include "Scene.h"

class GameEngine 
{
public:
	GameEngine();
	std::shared_ptr<Scene> getActiveScene();
	void changeScene(std::shared_ptr<Scene> scene, const std::string & name, bool endScene);
	void run();

protected:
	
	sf::Window _window;
	bool _isRunning;
	std::string _activeScene;
	std::string _previousScene;
	std::map<std::string, std::shared_ptr<Scene>> _sceneMap;

	void init();
	void update();
	void handleInput();
	void quit();

};