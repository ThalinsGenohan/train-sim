#include "pch.h"
#include "World.h"

#include "Constants.h"

namespace ott
{
	World::World()
	{
		this->_chunks = std::vector<Chunk>();
		for (int y = -INITIAL_CHUNK_RADIUS; y <= INITIAL_CHUNK_RADIUS; y++)
		{
			for (int x = -INITIAL_CHUNK_RADIUS; x <= INITIAL_CHUNK_RADIUS; x++)
			{
				this->_chunks.emplace_back(sf::Vector2i(x, y));
			}
		}
	}

	Chunk& World::GetChunk(const sf::Vector2i coords)
	{
		for (Chunk& c : this->_chunks)
		{
			if (c.GetChunkCoords() == coords)
			{
				return c;
			}
		}
		return this->GenerateChunk(coords);
	}

	Chunk& World::GetChunk(const int x, const int y)
	{
		return GetChunk(sf::Vector2i(x, y));
	}

	std::vector<Chunk>& World::GetChunks()
	{
		return this->_chunks;
	}

	Chunk& World::GenerateChunk(const sf::Vector2i coords)
	{
		return this->_chunks.emplace_back(coords);
	}

	Chunk& World::GenerateChunk(const int x, const int y)
	{
		return GenerateChunk(sf::Vector2i(x, y));
	}

	std::vector<char>& World::SavedWorld::GetData() const
	{
		const auto vec = new std::vector<char>();
		vec->push_back(this->gameVersionMajor);
		vec->push_back(this->gameVersionMinor);
		vec->push_back(this->gameVersionPatch);
		vec->push_back(this->worldFormat);

		for (unsigned i = 0; i < sizeof this->stationCount; i++)
		{
			vec->push_back(this->stationCount >> 8 * i & 0xFF);
		}
		// TODO: save stations

		vec->push_back(this->chunkSize);
		for (unsigned i = 0; i < sizeof this->chunkCount; i++)
		{
			vec->push_back(this->chunkCount >> 8 * i & 0xFF);
		}
		for (unsigned i = 0; i < this->chunkCount; i++)
		{
			auto& [coords, tiles] = this->chunks[i];

			for (unsigned x = 0; x < sizeof coords.x; x++)
			{
				vec->push_back(coords.x >> 8 * x & 0xFF);
			}
			for (unsigned y = 0; y < sizeof coords.y; y++)
			{
				vec->push_back(coords.y >> 8 * y & 0xFF);
			}

			for (auto& [tracks, signals] : tiles)
			{
				vec->push_back(tracks);
				vec->push_back(signals);
			}
		}

		std::cout << vec->size() << std::endl;

		return *vec;
	}

	World::SavedWorld::SavedWorld(World& world)
		: stationCount(0),
		  stations(nullptr),
		  chunkCount(world._chunks.size()),
		  chunks(new Chunk::SavedChunk[this->chunkCount])
	{
		this->gameVersionMajor = ott_const::GAME_VERSION_MAJOR;
		this->gameVersionMinor = ott_const::GAME_VERSION_MINOR;
		this->gameVersionPatch = ott_const::GAME_VERSION_PATCH;
		this->worldFormat      = WORLD_FORMAT;

		this->chunkSize = Chunk::CHUNK_SIZE;

		for (int i = 0; Chunk& chunk : world._chunks)
		{
			this->chunks[i] = chunk.GetSavedChunk();
			i++;
		}
	}

	World::SavedWorld& World::GetSavedWorld()
	{
		const auto save = new SavedWorld(*this);
		return *save;
	}

	std::ostream& operator<<(std::ostream& out, const World::SavedWorld& world)
	{
		out << "\tGame Version: " << static_cast<int>(world.gameVersionMajor) << "."
			<< static_cast<int>(world.gameVersionMinor) << "."
			<< static_cast<int>(world.gameVersionPatch) << "\n"
			<< "\tWorld Format: " << static_cast<int>(world.worldFormat) << "\n"
			<< "\tStation Count: " << world.stationCount << "\n"
			<< "\tChunk Size: " << static_cast<int>(world.chunkSize) << "\n"
			<< "\tChunk Count: " << world.chunkCount << std::endl;

		return out;
	}
}
