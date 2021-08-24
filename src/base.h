#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

#include "Action.h"
#include "Vec2.h"
#pragma once

//Amount of frames per tick
const size_t SIM_SPEED = 1;

const float PI = 3.14159265;

const float GRAVITY = 0.5;
const float MAXFALL = 20;

const bool DEBUG = true;

typedef uint32_t Entity;

namespace Util
{
	void centerText(sf::Text& text);
}