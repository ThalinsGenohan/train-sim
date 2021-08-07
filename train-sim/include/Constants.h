#pragma once

#include <SFML/System/Vector2.hpp>

#include "Types.h"

namespace ott_const
{
const byte GAME_VERSION_MAJOR = 0;
const byte GAME_VERSION_MINOR = 0;
const byte GAME_VERSION_PATCH = 0;

enum Rotation : byte
{
	Up = 0b000,
	UpRight = 0b001,
	Right = 0b010,
	DownRight = 0b011,
	Down = 0b100,
	DownLeft = 0b101,
	Left = 0b110,
	UpLeft = 0b111,
};

enum class BuildingType
{
	None = 0,
	Track = 1,
	Signal = 2,
};

[[maybe_unused]] const sf::Vector2i DirectionOffsets[8] = {
	{ 0,  1 },
	{ 1,  1 },
	{ 1,  0 },
	{ 1,  -1 },
	{ 0,  -1 },
	{ -1, -1 },
	{ -1, 0 },
	{ -1, 1 },
};
}
