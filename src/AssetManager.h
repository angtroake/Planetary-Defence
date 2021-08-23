#pragma once
#include "base.h"
class AssetManager
{
private:
	std::map<std::string, sf::Font> _fonts;
	std::map<std::string, sf::Texture> _textures;
	void addFont(const std::string name, const std::string path);
	void addTexture(const std::string& name, const std::string& path);

public:
	AssetManager() {}
	void init();
	sf::Texture& getTexture(const std::string& name);
	sf::Font& getFont(const std::string& name);
};