/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class implements the king piece
*/

#ifndef __KING_HEADER
#define __KING_HEADER

#include "Piece.h"

namespace Chess
{
	class King : public Piece
	{
	public:
		King(const Faction faction, const int position) : Piece(faction, position,
			std::vector<int>{TileVector::north_west, TileVector::north, TileVector::north_east, TileVector::west, TileVector::east, TileVector::south_west, TileVector::south, TileVector::south_east}) {}
		~King() override {}


	private:
		
		MoveList generate_move_list(Board& board, const std::vector<int> move_vectors) override;

		int m_castle_range{ 2 };
		std::vector<int> m_castle_vectors{ TileVector::west, TileVector::east };
	};
}

#endif