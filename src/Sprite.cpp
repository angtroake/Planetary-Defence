#include "Sprite.h"

Sprite::Sprite(const sf::Texture& texture, size_t sheetX, size_t sheetY, uint32_t frameCount, uint16_t speed)
	:_frameCount(frameCount), _speed(speed), _sheetX(sheetX), _sheetY(sheetY)
{
	_sprite = sf::Sprite(texture);
	_textureSize = texture.getSize();
	_sprite.setTextureRect(sf::IntRect(0 * getSize().x, 0 * getSize().y, getSize().x, getSize().y));
}

void Sprite::animate()
{
	//Only 1 frame so no need to animate
	if (_frameCount <= 1) { return; }

	_frame += 1;

	size_t frame = (_frame / _speed) % _frameCount;
	size_t frameX = frame % _sheetX;
	size_t frameY = int(frame / _sheetX) % _sheetY;
	_sprite.setTextureRect(sf::IntRect(frameX * getSize().x, frameY * getSize().y, getSize().x, getSize().y));
}

const bool Sprite::ended() const
{
	return _frame >= _frameCount * _speed;
}

const Vec2 Sprite::getSize() const
{
	return Vec2(_textureSize.x / _sheetX, _textureSize.y / _sheetY);
}

sf::Sprite& Sprite::get()
{
	return _sprite;
}