#pragma once
#include <SFML/System/Vector2.hpp>

#include "Types.h"
#include "Constants.h"

namespace ott
{
	class Block;
	class Chunk;

class Tile
	{
	public:
		static const int TILE_SIZE = 32;
		static const size_t GRID_VERTEX_COUNT = 8;
		static const size_t TRACK_VERTEX_COUNT = 16;

		explicit Tile(Chunk& chunk, byte x, byte y);
		explicit Tile(Chunk& chunk, sf::Vector2<byte> coords);

		sf::Vector2<byte> GetLocalCoords() const;
		sf::Vector2i      GetGlobalCoords() const;
		byte              GetTracks() const;
		bool              GetTrack(ott_const::Rotation rotation) const;

		void AddBuilding(ott_const::BuildingType build, ott_const::Rotation rot);
		void RemoveBuilding(ott_const::BuildingType build, ott_const::Rotation rot);
		void AddTrack(ott_const::Rotation rotation);
		void RemoveTrack(ott_const::Rotation rotation);

		const sf::VertexArray& GetGridVertices() const;
		const sf::VertexArray& GetTrackVertices() const;
		sf::FloatRect GetBounds();

		struct SavedTile
		{
			byte tracks;
			byte signals;
		};

		const SavedTile& GetSavedTile() const;

	private:
		Chunk&            _chunk;
		const sf::Vector2<byte> _localCoords;
		byte                    _tracks;
		Block*                  _block;
		sf::VertexArray _gridVertices;
		sf::VertexArray _trackVertices;
	};
}
