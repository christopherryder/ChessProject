/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to represent a 'tile' on the internal gameboard, which is different to that of what a player might see. The internal gameboard is surrounded by a 'border' of tiles
to represent to pieces that they cannot traverse any further in this direction. This class also contains the 'chess' definitions for a faction and what tiles may be present on
a chess board.
*/

#ifndef __TILE_HEADER
#define __TILE_HEADER

#include "TileBase.h"

#include <ostream>

namespace Chess
{
	//the internal game-board is composed of the following types.
	enum class TileType
	{
		Empty,
		Pawn,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
		Border
	};

	//represents the different teams in standard chess as well as non-aligned pieces, e.g empty tiles, border tiles.
	enum class Faction
	{
		NonAligned,
		White,
		Black
	};

	class Tile : public TileBase<TileType, Faction>
	{
	public:

		Tile(const TileType tiletype, const Faction faction) : TileBase<TileType, Faction>(tiletype, faction) {}
		Tile() : Tile(TileType::Empty, Faction::NonAligned) {}
		~Tile() {}

		void clear() { m_faction = Faction::NonAligned; m_tiletype = TileType::Empty; }

		friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
	};
}
#endif