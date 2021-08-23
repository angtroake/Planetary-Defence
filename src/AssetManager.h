#pragma once
#include "base.h"
#include "Sprite.h"
class AssetManager
{
private:
	std::map<std::string, sf::Font> _fonts;
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, Sprite> _sprites;
	void addFont(const std::string name, const std::string path);
	void addTexture(const std::string& name, const std::string& path);
	void createSprite(const std::string& name, const sf::Texture& texture);
	void createSprite(const std::string& name, const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, bool repeat, uint32_t speed);

public:
	AssetManager() {}
	void init();
	const sf::Texture& getTexture(const std::string& name) const;
	const sf::Font& getFont(const std::string& name) const;
	const Sprite& getSprite(const std::string& name) const;
};