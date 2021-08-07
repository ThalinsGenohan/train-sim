#include "pch.h"
#include "Tile.h"

#include "Chunk.h"
#include "Constants.h"

using ott_const::Rotation;

namespace ott
{

Tile::Tile(Chunk& chunk, const byte x, const byte y) :Tile(chunk, sf::Vector2<byte>(x, y))
{
}

Tile::Tile(Chunk& chunk, const sf::Vector2<byte> coords)
		:_chunk(chunk),
		 _localCoords(coords),
		 _tracks(0x00),
		 _block(nullptr),
		 _gridVertices(sf::Lines, GRID_VERTEX_COUNT),
		 _trackVertices(sf::Lines, TRACK_VERTEX_COUNT)
{
	sf::Vector2f chunkPos =
			sf::Vector2f(this->_chunk.GetChunkCoords()) * static_cast<float>(TILE_SIZE * Chunk::CHUNK_SIZE);

	const sf::Vector2f topLeft = chunkPos + sf::Vector2f(
			static_cast<float>(this->_localCoords.x) + 0.f,
			static_cast<float>(this->_localCoords.y) + 0.f
	) * static_cast<float>(TILE_SIZE);
	const sf::Vector2f topRight = sf::Vector2f(topLeft.x + TILE_SIZE, topLeft.y);
	const sf::Vector2f bottomLeft = sf::Vector2f(topLeft.x, topLeft.y + TILE_SIZE);
	const sf::Vector2f bottomRight = sf::Vector2f(topLeft.x + TILE_SIZE, topLeft.y + TILE_SIZE);
	const sf::Vector2f center = sf::Vector2f(topLeft.x + (TILE_SIZE / 2.f), topLeft.y + (TILE_SIZE / 2.f));
	const sf::Color color = sf::Color(0xFFFFFF0F);

	// Rectangle
	// Top Left -> Top Right
	this->_gridVertices[0] = sf::Vertex(topLeft, color);
	this->_gridVertices[1] = sf::Vertex(topRight, color);

	// Top Right -> Bottom Right
	this->_gridVertices[2] = sf::Vertex(topRight, color);
	this->_gridVertices[3] = sf::Vertex(bottomRight, color);

	// Bottom Right -> Bottom Left
	this->_gridVertices[4] = sf::Vertex(bottomRight, color);
	this->_gridVertices[5] = sf::Vertex(bottomLeft, color);

	// Bottom Left -> Top Left
	this->_gridVertices[6] = sf::Vertex(bottomLeft, color);
	this->_gridVertices[7] = sf::Vertex(topLeft, color);

	// Tracks
	// Up
	this->_trackVertices[0] = sf::Vertex(sf::Vector2f(center.x, topLeft.y), sf::Color::Transparent);
	this->_trackVertices[1] = sf::Vertex(center, sf::Color::Transparent);

	// UpRight
	this->_trackVertices[2] = sf::Vertex(topRight, sf::Color::Transparent);
	this->_trackVertices[3] = sf::Vertex(center, sf::Color::Transparent);

	// Right
	this->_trackVertices[4] = sf::Vertex(sf::Vector2f(topRight.x, center.y), sf::Color::Transparent);
	this->_trackVertices[5] = sf::Vertex(center, sf::Color::Transparent);

	// DownRight
	this->_trackVertices[6] = sf::Vertex(bottomRight, sf::Color::Transparent);
	this->_trackVertices[7] = sf::Vertex(center, sf::Color::Transparent);

	// Down
	this->_trackVertices[8] = sf::Vertex(sf::Vector2f(center.x, bottomLeft.y), sf::Color::Transparent);
	this->_trackVertices[9] = sf::Vertex(center, sf::Color::Transparent);

	// DownLeft
	this->_trackVertices[10] = sf::Vertex(bottomLeft, sf::Color::Transparent);
	this->_trackVertices[11] = sf::Vertex(center, sf::Color::Transparent);

	// Left
	this->_trackVertices[12] = sf::Vertex(sf::Vector2f(topLeft.x, center.y), sf::Color::Transparent);
	this->_trackVertices[13] = sf::Vertex(center, sf::Color::Transparent);

	// Left
	this->_trackVertices[14] = sf::Vertex(topLeft, sf::Color::Transparent);
	this->_trackVertices[15] = sf::Vertex(center, sf::Color::Transparent);
}

sf::Vector2<byte> Tile::GetLocalCoords() const { return this->_localCoords; }

sf::Vector2i Tile::GetGlobalCoords() const
{
	return sf::Vector2i(this->_localCoords) + this->_chunk.GetTileCoords();
}

byte Tile::GetTracks() const { return this->_tracks; }

bool Tile::GetTrack(Rotation rotation) const
{
	return this->_tracks & static_cast<byte>(rotation);
}

void Tile::AddTrack(Rotation rotation)
{
	this->_tracks |= static_cast<byte>(rotation);

	this->_trackVertices[(int)rotation * 2].color = sf::Color::White;
	this->_trackVertices[(int)rotation * 2 + 1].color = sf::Color::White;
}

void Tile::RemoveTrack(Rotation rotation)
{
	this->_tracks &= ~static_cast<byte>(rotation);

	this->_trackVertices[(int)rotation * 2].color = sf::Color::Transparent;
	this->_trackVertices[(int)rotation * 2 + 1].color = sf::Color::Transparent;
}

const sf::VertexArray& Tile::GetGridVertices() const
{
	return this->_gridVertices;
}
const sf::VertexArray& Tile::GetTrackVertices() const
{
	return this->_trackVertices;
}

sf::FloatRect Tile::GetBounds()
{
	return this->_gridVertices.getBounds();
}

const Tile::SavedTile& Tile::GetSavedTile() const
{
	const auto savedTile = new SavedTile();

	savedTile->tracks = this->_tracks;

	// TODO: Signals
	// Signal type: 0 = signal/none, 1= chain
	// Positive: 1 = train can go in positive direction
	// Negative: 1 = train can go in negative direction
	savedTile->signals = 0b000;

	return *savedTile;
}
void Tile::AddBuilding(ott_const::BuildingType build, ott_const::Rotation rot)
{
	switch (build)
	{
	case ott_const::BuildingType::Track:
		this->AddTrack(rot);
		break;

	default:;
	}
}
void Tile::RemoveBuilding(ott_const::BuildingType build, ott_const::Rotation rot)
{
	switch (build)
	{
	case ott_const::BuildingType::Track:
		this->RemoveTrack(rot);
		break;

	default:;
	}
}

}
