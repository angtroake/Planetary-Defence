#pragma once
#include "base.h"
class Sprite
{
protected:
	sf::Sprite _sprite;
	size_t _frame = 0;
	size_t _frameCount;
	size_t _speed;
	sf::Vector2u _textureSize;
	size_t _sheetX;
	size_t _sheetY;
public:
	Sprite() {};
	Sprite(const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, uint16_t speed);
	void animate();
	const bool ended() const;
	const Vec2 getSize() const;
	sf::Sprite& get();
	sf::IntRect getCurrentFrame();
};

