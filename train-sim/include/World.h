#pragma once
#include <string>
#include <vector>
#include <SFML/System/Vector2.hpp>

#include "Chunk.h"
#include "Types.h"

namespace ott
{
	class World
	{
	public:
		static const byte WORLD_FORMAT         = 0xFF;
		static const int  INITIAL_CHUNK_RADIUS = 5;

		World();

		Chunk&                    GetChunk(sf::Vector2i coords);
		Chunk&                    GetChunk(int x, int y);
		std::vector<Chunk>& GetChunks();

		Chunk& GenerateChunk(sf::Vector2i coords);
		Chunk& GenerateChunk(int x, int y);

		struct SavedWorld
		{
			friend class World;

			byte gameVersionMajor;
			byte gameVersionMinor;
			byte gameVersionPatch;
			byte worldFormat;

			size_t                   stationCount;
			std::unique_ptr<void*[]> stations; // TODO: Stations

			byte                                 chunkSize;
			size_t                               chunkCount;
			std::unique_ptr<Chunk::SavedChunk[]> chunks;

			std::vector<char>& GetData() const;

			friend std::ostream& operator<<(std::ostream& out, const SavedWorld& world);

		private:
			explicit SavedWorld(World& world);
		};

		SavedWorld& GetSavedWorld();

	private:
		std::string        _name;
		std::vector<Chunk> _chunks;
	};
}
