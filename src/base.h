#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

#include "Action.h"
#include "Vec2.h"
#pragma once

//Amount of frames per tick
const size_t SIM_SPEED = 1;

namespace Util
{
	void centerText(sf::Text& text);
}