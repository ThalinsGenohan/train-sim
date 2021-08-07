#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Tile.h"
#include "Types.h"
#include "Constants.h"

namespace ott
{
	class Chunk final : public sf::Drawable
	{
	public:
		static const byte CHUNK_SIZE = 64;

		explicit Chunk(int x, int y);
		explicit Chunk(sf::Vector2i chunkCoords);

		sf::Vector2i GetChunkCoords() const;
		sf::Vector2i GetTileCoords() const;
		Tile&        GetChunkTile(sf::Vector2i coords);
		Tile&        GetGlobalTile(sf::Vector2i coords);

		void UpdateTrackVertices(Tile& tile);
		void UpdateTileVertices(Tile& tile);

		void SetGridVisible(bool visible);

		sf::FloatRect GetBounds();
		void HandleClick(bool rmb, sf::Vector2f pos, ott_const::BuildingType build, ott_const::Rotation rot);

		const Vector2D<Tile>& GetTiles() const;

		struct SavedChunk
		{
			sf::Vector2i                                         coords;
			std::array<Tile::SavedTile, CHUNK_SIZE * CHUNK_SIZE> tiles{};
		};

		const SavedChunk& GetSavedChunk() const;

	private:
		const sf::Vector2i _chunkCoords;

		Vector2D<Tile> _tiles;

		sf::VertexArray _tileVertices;
		sf::VertexArray _trackVertices;
		sf::VertexArray _chunkVertices;

		bool _tileGridEnabled;
		bool _tileGridVisible;

		sf::Vector2f _drawPosition;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
