/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to implement the abstract PieceBase class and describe in some detail what all the methods will do. 
This class also implements the Knight piece, as it follows the logic of 'Piece' completely.
*/

#ifndef __PIECE_HEADER
#define __PIECE_HEADER

#include "PieceBase.h"

#include <vector>

#include "Board.h"
#include "MoveList.h"


namespace Chess {

	struct Pin {
		//vector of the pin direction
		int m_vector{ 0 };
		//position of the pinned piece.
		int m_position{ 0 }; 
	};

	class Piece : public PieceBase<Faction>
	{
	public:
		Piece(const Faction faction, const int position, const std::vector<int>& move_vectors) : PieceBase(faction, position), m_move_vectors(move_vectors) {}
		virtual ~Piece() = 0;

		//move commands simply assign the piece a new internal position and increment their move counter.
		virtual void move(const int position) override { ++m_move_count; m_position = position; }
		virtual void undo_move(const int position) override { --m_move_count; m_position = position; }

		//pinning a piece represents a restriction to it's move vectors so that it can only move in a certain direction.
		void pin(const int restriction_vector);

		//this function is overloaded, privately, so that we can generate a different set of moves if the piece is pinned.
		MoveList generate_move_list(Board& board);

		//generate a list of all the positions that can currently be 'captured' NOT 'moved' to.
		virtual MoveList generate_capture_list(Board& board) const;

		//the pieces alignment with a given position.
		virtual const int get_alignment_vector(const Board& board, const int position) const;

		//twin-function to pin; determines wether this piece pins another.
		virtual const Pin generate_pins(const int enemy_king_position, const Board& board) const;

	protected:

		virtual MoveList generate_move_list(Board& board, const std::vector<int> move_vectors);

		int m_move_count{ 0 };
		bool m_pinned{ false };

		std::vector<int> m_move_vectors;
		std::vector<int> m_pinned_move_vectors;
		typedef typename Board::TileVector TileVector;
	};
	inline Piece::~Piece() = default;

	class Knight : public Piece
	{
	public:
		Knight(const Faction faction, const int position) : Piece(faction, position, std::vector<int>{TileVector::north_north_west, TileVector::north_north_east, TileVector::north_west_west, TileVector::north_east_east, TileVector::south_west_west,
			TileVector::south_east_east, TileVector::south_south_west, TileVector::south_south_east }) {}

		~Knight() override {}
	};

	//functor to compare a piece objects position w.r.t to an int or another piece object. used in standard library algorithms.
	struct PieceSortPredicate
	{
		inline bool operator()(const std::unique_ptr<Piece>& piece1, const std::unique_ptr<Piece>& piece2) const { return(piece1->get_position() < piece2->get_position()); }

		inline bool operator()(const int& position, const std::unique_ptr<Piece>& piece) const { return(position < piece->get_position()); }
		inline bool operator()(const std::unique_ptr<Piece>& piece, const int& position) const { return(piece->get_position() < position); }
	};
}

#endif