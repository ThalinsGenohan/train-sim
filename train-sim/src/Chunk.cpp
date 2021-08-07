#include "pch.h"
#include "Chunk.h"

namespace ott
{
void Chunk::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
	if (this->_tileGridEnabled && this->_tileGridVisible)
		target.draw(this->_tileVertices, states);

	target.draw(this->_trackVertices, states);
	target.draw(this->_chunkVertices, states);
}

Chunk::Chunk(const int x, const int y) :Chunk(sf::Vector2i(x, y))
{
}

Chunk::Chunk(const sf::Vector2i chunkCoords)
		:_chunkCoords(chunkCoords),
		 _drawPosition(sf::Vector2f(chunkCoords) * static_cast<float>(Tile::TILE_SIZE * CHUNK_SIZE)),
		 _tileVertices(sf::Lines, Tile::GRID_VERTEX_COUNT * CHUNK_SIZE * CHUNK_SIZE),
		 _trackVertices(sf::Lines, Tile::TRACK_VERTEX_COUNT * CHUNK_SIZE * CHUNK_SIZE),
		 _tileGridEnabled(true),
		 _tileGridVisible(true)
{
	this->_tiles.reserve(CHUNK_SIZE);
	for (unsigned y = 0; y < CHUNK_SIZE; y++)
	{
		auto& row = this->_tiles.emplace_back();
		row.reserve(CHUNK_SIZE);
		for (unsigned x = 0; x < CHUNK_SIZE; x++)
		{
			auto& tile = row.emplace_back(*this, static_cast<byte>(x), static_cast<byte>(y));

			this->UpdateTileVertices(tile);
			this->UpdateTrackVertices(tile);
		}
	}

	this->_chunkVertices.setPrimitiveType(sf::LineStrip);
	this->_chunkVertices.append(sf::Vertex(this->_drawPosition, sf::Color::Green));
	this->_chunkVertices.append(sf::Vertex(
			sf::Vector2f(
					this->_drawPosition.x + static_cast<float>(Tile::TILE_SIZE * CHUNK_SIZE),
					this->_drawPosition.y
			),
			sf::Color::Green
	));
	this->_chunkVertices.append(sf::Vertex(
			sf::Vector2f(
					this->_drawPosition.x + static_cast<float>(Tile::TILE_SIZE * CHUNK_SIZE),
					this->_drawPosition.y + static_cast<float>(Tile::TILE_SIZE * CHUNK_SIZE)
			),
			sf::Color::Green
	));
	this->_chunkVertices.append(sf::Vertex(
			sf::Vector2f(
					this->_drawPosition.x,
					this->_drawPosition.y + static_cast<float>(Tile::TILE_SIZE * CHUNK_SIZE)
			),
			sf::Color::Green
	));
	this->_chunkVertices.append(sf::Vertex(this->_drawPosition, sf::Color::Green));
}

sf::Vector2i Chunk::GetChunkCoords() const
{
	return this->_chunkCoords;
}

sf::Vector2i Chunk::GetTileCoords() const
{
	return this->_chunkCoords * static_cast<int>(CHUNK_SIZE);
}

Tile& Chunk::GetChunkTile(const sf::Vector2i coords)
{
	return this->_tiles[coords.y][coords.x];
}

Tile& Chunk::GetGlobalTile(const sf::Vector2i coords)
{
	return this->_tiles[coords.y][coords.x];
}

const Vector2D<Tile>& Chunk::GetTiles() const
{
	return this->_tiles;
}

void Chunk::UpdateTrackVertices(Tile& tile)
{
	auto tileCoords = tile.GetLocalCoords();
	auto& trackVerts = tile.GetTrackVertices();
	size_t startIndex = tileCoords.y * Tile::TRACK_VERTEX_COUNT * CHUNK_SIZE + tileCoords.x * Tile::TRACK_VERTEX_COUNT;

	for (size_t i = 0; i < Tile::TRACK_VERTEX_COUNT; i++)
	{
		this->_trackVertices[startIndex + i] = trackVerts[i];
	}
}
void Chunk::UpdateTileVertices(Tile& tile)
{
	auto tileCoords = tile.GetLocalCoords();
	auto& tileVerts = tile.GetGridVertices();
	size_t startIndex = tileCoords.y * Tile::GRID_VERTEX_COUNT * CHUNK_SIZE + tileCoords.x * Tile::GRID_VERTEX_COUNT;

	for (size_t i = 0; i < Tile::GRID_VERTEX_COUNT; i++)
	{
		this->_tileVertices[startIndex + i] = tileVerts[i];
	}
}

const Chunk::SavedChunk& Chunk::GetSavedChunk() const
{
	const auto savedChunk = new SavedChunk();

	savedChunk->coords = this->_chunkCoords;

	int i = 0;
	for (int y = 0; y < CHUNK_SIZE; y++)
	{
		auto& row = this->_tiles[y];
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			savedChunk->tiles[i] = row[x].GetSavedTile();
			i++;
		}
	}

	return *savedChunk;
}
sf::FloatRect Chunk::GetBounds()
{
	return this->_chunkVertices.getBounds();
}
void Chunk::HandleClick(bool rmb, sf::Vector2f pos, ott_const::BuildingType build, ott_const::Rotation rot)
{
	for (auto& row : this->_tiles)
	{
		for (auto& tile : row)
		{
			if (tile.GetBounds().contains(pos))
			{
				if (rmb)
				{
					tile.RemoveBuilding(build, rot);
					if (build == ott_const::BuildingType::Track)
						tile.RemoveBuilding(build, static_cast<ott_const::Rotation>((rot + 4) % 8));
				}
				else
				{
					tile.AddBuilding(build, rot);
					if (build == ott_const::BuildingType::Track)
						tile.AddBuilding(build, static_cast<ott_const::Rotation>((rot + 4) % 8));
				}

				this->UpdateTrackVertices(tile);
				return;
			}
		}
	}
}
void Chunk::SetGridVisible(bool visible)
{
	this->_tileGridVisible = visible;
}
}
