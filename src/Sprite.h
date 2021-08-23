#pragma once
#include "base.h"
class Sprite
{
protected:
	sf::Sprite _sprite;
	uint32_t _frame;
	uint32_t _frameCount;
	uint16_t _speed;
	bool _repeat;
	sf::Vector2u _textureSize;
	size_t _sheetX;
	size_t _sheetY;
public:
	Sprite() {};
	Sprite(const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, bool repeat, uint16_t speed);
	void animate();
	const bool & ended() const;
	const Vec2 & getSize() const;
	const sf::Sprite& get() const;
};

