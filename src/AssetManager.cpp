#include "AssetManager.h"
#include <iostream>
#include <assert.h>

void AssetManager::init() 
{
	addFont("Crater", "assets/fonts/Crater.otf");

	addTexture("Earth", "assets/img/earth2_512.png");
	createSprite("Earth", getTexture("Earth"), 20, 20, 400, 3);

	//Asteroids
	addTexture("Asteroid1", "assets/img/asteroid1.png");
	createSprite("Asteroid1", getTexture("Asteroid1"), 16, 16, 250, 1);
	addTexture("Asteroid2", "assets/img/asteroid2.png");
	createSprite("Asteroid2", getTexture("Asteroid2"), 16, 16, 250, 1);

	//addShader("Atmosphere", "assets/shaders/atmosphere.frag", ShaderType::FRAGMENT);
	//getShader("Atmosphere")->setUniform("iResolution", sf::Vector2f{ 256, 256}*8.0f);
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