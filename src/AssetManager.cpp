#include "AssetManager.h"
#include <iostream>
#include <assert.h>

void AssetManager::init() 
{

	addTexture("Background", "assets/img/background2.png");
	createSprite("Background", getTexture("Background"));

	addFont("Crater", "assets/fonts/Crater.otf");

	addTexture("Earth", "assets/img/earth3_512.png");
	createSprite("Earth", getTexture("Earth"), 20, 20, 400, 3);

	addTexture("Health", "assets/img/health.png");
	createSprite("Health", getTexture("Health"));
	addSound("GetHealth", "assets/sounds/get_health.wav");

	//Asteroids
	addTexture("Asteroid1", "assets/img/asteroid1.png");
	createSprite("Asteroid1", getTexture("Asteroid1"), 16, 16, 250, 1);
	addTexture("Asteroid2", "assets/img/asteroid2.png");
	createSprite("Asteroid2", getTexture("Asteroid2"), 16, 16, 250, 1);

	//Gamma Stuff
	addTexture("GammaWarning", "assets/img/gamma_warning.png");
	createSprite("GammaWarning", getTexture("GammaWarning"));
	addTexture("GammaRay", "assets/img/gamma.png");
	createSprite("GammaRay", getTexture("GammaRay"), 5, 1, 5, 6);

	//Sheild
	addTexture("Shield", "assets/img/shield.png");
	createSprite("Shield", getTexture("Shield"), 1, 8, 8, 3);

	//Popsicle Stick
	addTexture("PopStickMid", "assets/img/popstick_mid.png");
	createSprite("PopStickMid", getTexture("PopStickMid"));
	addTexture("PopStickEnd", "assets/img/popstick_end.png");
	createSprite("PopStickEnd", getTexture("PopStickEnd"));

	//UFO
	addTexture("UFO", "assets/img/ufo.png");
	createSprite("UFO", getTexture("UFO"), 15, 14, 200, 1);

	//Mothership
	addTexture("MothershipWarning", "assets/img/mothership_warning.png");
	createSprite("MothershipWarning", getTexture("MothershipWarning"));
	addTexture("Mothership", "assets/img/mothership.png");
	createSprite("Mothership", getTexture("Mothership"), 15, 14, 200, 2);
	addSound("MothershipCharge", "assets/sounds/mothership_charge.ogg");
	getSound("MothershipCharge").setVolume(50);
	addSound("MothershipShoot", "assets/sounds/mothership_shoot.wav");
	addTexture("MothershipShield", "assets/img/mothership_shield.png");
	createSprite("MothershipShield", getTexture("MothershipShield"));


	//Laser
	addTexture("Laser", "assets/img/laser_short.png");
	createSprite("Laser", getTexture("Laser"));

	//Satellite Turret
	addTexture("SatelliteReady", "assets/img/satellite_ready.png");
	createSprite("SatelliteReady", getTexture("SatelliteReady"));
	addTexture("SatelliteCharge", "assets/img/satellite_charge.png");
	createSprite("SatelliteCharge", getTexture("SatelliteCharge"));

	//Music
	addSound("MusicMenu", "assets/music/Cyberspace.ogg");
	addSound("MusicPlay0", "assets/music/Dungeon1.ogg");
	addSound("MusicPlay1", "assets/music/Dungeon2.ogg");
	addSound("BossMusic0", "assets/music/Battle1.ogg");
	getSound("BossMusic0").setLoop(true);
	addSound("BossMusic1", "assets/music/Battle2.ogg");
	getSound("BossMusic1").setLoop(true);
	addSound("BossMusic2", "assets/music/Battle3.ogg");
	getSound("BossMusic2").setLoop(true);
	addSound("BossWin", "assets/music/Victory.ogg");
	addSound("GameOver", "assets/music/GameOver.ogg");

	//Sounds
	addSound("Laser1", "assets/sounds/laser1.wav");
	addSound("Laser2", "assets/sounds/laser2.wav");
	addSound("Laser3", "assets/sounds/laser3.wav");
	addSound("Laser4", "assets/sounds/laser4.wav");
	addSound("SatelliteReady", "assets/sounds/satellite_ready.wav");
	addSound("BossWarning", "assets/sounds/boss_warning.wav");
	addSound("GammaWarning", "assets/sounds/gamma_warning.ogg");
	addSound("Explode0", "assets/sounds/explode0.wav");
	addSound("Explode1", "assets/sounds/explode1.wav");
	addSound("Explode2", "assets/sounds/explode2.wav");
	addSound("Explode3", "assets/sounds/explode3.wav");
	addSound("Explode4", "assets/sounds/explode4.wav");
	addSound("Explode5", "assets/sounds/explode5.wav");
	addSound("GammaRay", "assets/sounds/gammaray.ogg");

	//Particles
	addTexture("BlueExplosion", "assets/img/blue_explosion.png");
	createSprite("BlueExplosion", getTexture("BlueExplosion"), 5, 5, 24, 2);
	addTexture("ShieldHit", "assets/img/shield_hit.png");
	createSprite("ShieldHit", getTexture("ShieldHit"), 1, 8, 8, 2);
	addTexture("Explosion", "assets/img/Explosion.png");
	createSprite("Explosion", getTexture("Explosion"), 12, 1, 12, 1);

	//addShader("Atmosphere", "assets/shaders/atmosphere.frag", ShaderType::FRAGMENT);
	//getShader("Atmosphere")->setUniform("iResolution", sf::Vector2f{ 256, 256}*8.0f);
	//addShader("String", "assets/shaders/string.frag", ShaderType::FRAGMENT);
}

void AssetManager::addFont(const std::string name, const std::string path) 
{
	sf::Font font;
	if (!font.loadFromFile(path)) 
	{
		std::cerr << "Failed to load: " << path << std::endl;
	}
	else
	{
		_fonts[name] = font;
	}
}

void AssetManager::addTexture(const std::string& name, const std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Failed to load: " << path << std::endl;
	}
	else
	{
		_textures[name] = texture;

	}
}

void AssetManager::createSprite(const std::string& name, const sf::Texture& texture)
{
	createSprite(name, texture, 1, 1, 1, 1);
}

void AssetManager::createSprite(const std::string& name, const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, uint32_t speed)
{
	
	Sprite spr(texture, sheetX, sheetY, frameCount, speed);
	_sprites[name] = spr;
}

void AssetManager::addShader(const std::string& name, const std::string path, const ShaderType type)
{
	if (type == ShaderType::FRAGMENT) 
	{
		std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
		if (!shader->loadFromFile(path, sf::Shader::Fragment)) 
		{
			std::cerr << "Failed to load shader: " << path << std::endl;
			shader.reset();
		}
		else 
		{
			_shaders[name] = shader;
		}
	}
	else if (type == ShaderType::VERTEX) 
	{
		std::shared_ptr<sf::Shader> shader = std::make_shared<sf::Shader>();
		if (!shader->loadFromFile(path, sf::Shader::Vertex))
		{
			std::cerr << "Failed to load shader: " << path << std::endl;
			shader.reset();
		}
		else
		{
			_shaders[name] = shader;
		}
	}
}

void AssetManager::addSound(const std::string& name, const std::string path) 
{
	sf::SoundBuffer sound;

	if (!sound.loadFromFile(path)) 
	{
		std::cerr << "Could not load sound file: " << path << std::endl;
	}
	else 
	{
		_soundBuffers[name] = sound;
		_sounds[name] = sf::Sound(_soundBuffers[name]);
		_sounds[name].setVolume(DEFAULT_AUDIO_VOLUME);
	}

}

const sf::Texture& AssetManager::getTexture(const std::string& name) const
{
	assert(_textures.find(name) != _textures.end());
	return _textures.at(name);
}

const sf::Font& AssetManager::getFont(const std::string& name) const
{
	assert(_fonts.find(name) != _fonts.end());
	return _fonts.at(name);
}

const Sprite& AssetManager::getSprite(const std::string& name) const
{
	assert(_sprites.find(name) != _sprites.end());
	return _sprites.at(name);
}

const std::shared_ptr<sf::Shader> & AssetManager::getShader(const std::string& name) const
{
	assert(_shaders.find(name) != _shaders.end());
	return _shaders.at(name);
}

sf::Sound& AssetManager::getSound(const std::string& name)
{
	assert(_sounds.find(name) != _sounds.end());
	return _sounds.at(name);
}

void AssetManager::stopAllSounds()
{
	for (auto iter = _sounds.begin(); iter != _sounds.end(); iter++) 
	{
		(*iter).second.stop();
	}

}

void AssetManager::setGlobalSoundVolume(float volume) 
{
	for (auto iter = _sounds.begin(); iter != _sounds.end(); iter++)
	{
		(*iter).second.setVolume(volume);
	}
}

void AssetManager::destroy()
{
	_textures.clear();
	_sprites.clear();
	_sounds.clear();
	_soundBuffers.clear();
	_fonts.clear();
} 