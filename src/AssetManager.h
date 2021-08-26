#pragma once
#include "base.h"
#include "Sprite.h"

enum ShaderType {FRAGMENT, VERTEX};

class AssetManager
{
private:
	std::map<std::string, sf::Font> _fonts;
	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, Sprite> _sprites;
	std::map<std::string, std::shared_ptr<sf::Shader>> _shaders;
	std::map<std::string, sf::Sound> _sounds;
	std::map<std::string, sf::SoundBuffer> _soundBuffers;

	void addFont(const std::string name, const std::string path);
	void addTexture(const std::string& name, const std::string& path);
	void createSprite(const std::string& name, const sf::Texture& texture);
	void createSprite(const std::string& name, const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, uint32_t speed);
	void addShader(const std::string& name, const std::string path, const ShaderType type);
	void addSound(const std::string& name, const std::string path);

public:
	AssetManager() {}
	void init();
	const sf::Texture& getTexture(const std::string& name) const;
	const sf::Font& getFont(const std::string& name) const;
	const Sprite& getSprite(const std::string& name) const;
	const std::shared_ptr<sf::Shader> & getShader(const std::string& name) const;
	sf::Sound& getSound(const std::string& name);
	
	void setGlobalSoundVolume(float volume);
	void stopAllSounds();
};