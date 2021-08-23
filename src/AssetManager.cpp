#include "AssetManager.h"
#include <iostream>
#include <assert.h>

void AssetManager::init() 
{
	addFont("Crater", "assets/fonts/Crater.otf");

	addTexture("Earth", "assets/img/earth16bit.png");
	createSprite("Earth", getTexture("Earth"), 16, 16, 250, true, 1);
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
	createSprite(name, texture, 1, 1, 1, true, 1);
}

void AssetManager::createSprite(const std::string& name, const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, bool repeat, uint32_t speed)
{
	
	Sprite spr(texture, sheetX, sheetY, frameCount, repeat, speed);
	_sprites[name] = spr;
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