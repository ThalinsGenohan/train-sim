#pragma once

using byte = uint8_t;

namespace ott
{
	template <typename T, size_t Sx, size_t Sy>
	using Array2D = std::array<std::array<T, Sx>, Sy>;
	
	template <typename T>
	using Vector2D = std::vector<std::vector<T>>;
}
