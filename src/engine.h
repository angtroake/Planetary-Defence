#include "base.h"
#include "Scene.h"
#include "AssetManager.h"

class GameEngine 
{
public:
	GameEngine();
	std::shared_ptr<Scene> getActiveScene();
	void changeScene(std::shared_ptr<Scene> scene, const std::string & name, bool endScene);
	void run();
	void quit();
	AssetManager & getAssets();

	sf::RenderWindow & getWindow();
	const sf::Vector2u & getWindowSize() const;
	
	void fadePlayingSounds();
	void stopSounds();

	size_t bestScore = 0;
	size_t currentScore = 0;

protected:
	
	sf::RenderWindow _window;
	bool _isRunning;
	std::string _activeScene;
	std::string _previousScene;
	std::map<std::string, std::shared_ptr<Scene>> _sceneMap;
	AssetManager _assets;

	float soundFadeFactor = 0.0f;
	float soundFadeDecrease = 0.01f;
	bool fadeSounds = false;


	void init();
	void update();
	void handleInput();
};