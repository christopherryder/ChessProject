/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class implements the pawn piece. The pawn piece is a surprisingly unique piece and struggles to fit into the current class hierachy. Especially in terms
of how many 'moves' it can make and the validation required for all of them.

This class uses the C++ conditional operator to initialise certain parameters in the initialiser list according to the given faction. This negated the need for having
a separate 'white' and 'black' pawn class, which would have invalidated the concept of using a 'Faction' flag entirely!
*/

#ifndef __PAWN_HEADER
#define __PAWN_HEADER

#include "Piece.h"

namespace Chess {
	class Pawn : public Piece
	{
	public:
		Pawn(const Faction faction, const int position) : Piece(faction, position, (faction == Faction::White) ? std::vector<int>{TileVector::north} : std::vector<int>{ TileVector::south }),
			m_attack_vectors( (faction == Faction::White) ? std::vector<int>{TileVector::north_west, TileVector::north_east} : std::vector<int>{ TileVector::south_west, TileVector::south_east }) {}

		~Pawn() override {}

		virtual MoveList generate_capture_list(Board& board) const override;

	private:
		
		//no need to expose this.
		inline const int get_move_direction() const { return m_move_vectors.front(); }

		MoveList generate_move_list(Board& board, const std::vector<int> move_vectors) override;

		std::vector<int> m_attack_vectors;
		std::vector<int> m_enpassant_vectors{ TileVector::west, TileVector::east };
	};
}

#endif