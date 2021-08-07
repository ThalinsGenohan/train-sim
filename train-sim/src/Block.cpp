#include "pch.h"
#include "Block.h"

namespace ott
{
	Block::Block()
		: _train(nullptr)
	{
		_tracks          = std::vector<Track*>();
		_connectedBlocks = std::vector<Block*>();
	}

	bool Block::IsOccupied() const { return this->_train != nullptr; }

	const Train* Block::GetTrain() const { return this->_train; }

	void Block::SetTrain(Train* train) { this->_train = train; }

	const std::vector<Track*>& Block::GetTracks() const { return this->_tracks; }

	const std::vector<Block*>& Block::GetConnectedBlocks() const { return this->_connectedBlocks; }

	bool Block::AddTrack(Track& track)
	{
		if (std::ranges::find(this->_tracks, &track) != this->_tracks.end())
		{
			return false;
		}
		this->_tracks.push_back(&track);
		return true;
	}

	bool Block::RemoveTrack(Track& track)
	{
		const auto it = std::ranges::find(this->_tracks, &track);
		if (it == this->_tracks.end())
		{
			return false;
		}
		this->_tracks.erase(it);
		return true;
	}

	bool Block::AddConnection(Block& block)
	{
		if (std::ranges::find(this->_connectedBlocks, &block) != this->_connectedBlocks.end())
		{
			return false;
		}
		this->_connectedBlocks.push_back(&block);
		return true;
	}

	bool Block::RemoveConnection(Block& block)
	{
		const auto it = std::ranges::find(this->_connectedBlocks, &block);
		if (it == this->_connectedBlocks.end())
		{
			return false;
		}
		this->_connectedBlocks.erase(it);
		return true;
	}
}
