#pragma once

namespace ott
{
	class Track;
	class Train;

	class Block
	{
	public:
		Block();

		bool IsOccupied() const;

		const Train* GetTrain() const;
		void         SetTrain(Train* train);

		const std::vector<Track*>& GetTracks() const;
		const std::vector<Block*>& GetConnectedBlocks() const;

		bool AddTrack(Track& track);
		bool RemoveTrack(Track& track);

		bool AddConnection(Block& block);
		bool RemoveConnection(Block& block);

	private:
		Train* _train;

		std::vector<Track*> _tracks;
		std::vector<Block*> _connectedBlocks;
	};
}
