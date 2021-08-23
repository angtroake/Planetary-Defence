#include "AssetManager.h"
#include <iostream>
#include <assert.h>

void AssetManager::init() 
{
	addFont("Crater", "assets/fonts/Crater.otf");
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