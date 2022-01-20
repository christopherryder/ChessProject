/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to implement all scanning pieces, those which can move an indefinite amount of tiles. The scanning piece subclass was particularly useful as essentially
all 'scanners' have the same logic.
*/

#ifndef __SCANNINGPIECE_HEADER
#define __SCANNINGPIECE_HEADER

#include "Piece.h"

namespace Chess
{
	class ScanningPiece : public Piece
	{
	public:
		ScanningPiece(const Faction faction, const int position, const std::vector<int>& move_vectors) : Piece(faction, position, move_vectors) {}
		virtual ~ScanningPiece() = 0;

		//scanning pieces generate moves differently, as they can generate moves until being blocked or encountering an enemy piece.
		MoveList generate_capture_list(Board& board) const override;

		virtual const int get_alignment_vector(const Board& board, const int position) const override;
		virtual const Pin generate_pins(const int enemy_king_position, const Board& board) const override;

	protected:
		virtual MoveList generate_move_list(Board& board, const std::vector<int> move_vectors) override;
	};

	inline ScanningPiece::~ScanningPiece() = default;

	class Bishop : public ScanningPiece
	{
	public:
		Bishop(const Faction faction, const int position) :
			ScanningPiece(faction, position, std::vector<int>{TileVector::north_west, TileVector::north_east, TileVector::south_west, TileVector::south_east}) {}
		~Bishop() override {}
	};

	class Rook : public ScanningPiece
	{
	public:
		Rook(const Faction faction, const int position) :
			ScanningPiece(faction, position, std::vector<int>{TileVector::north, TileVector::west, TileVector::east, TileVector::south}) {}
		~Rook() override {}
	};

	class Queen : public ScanningPiece
	{
	public:
		Queen(const Faction faction, const int position) :
			ScanningPiece(faction, position, std::vector<int>{TileVector::north_west, TileVector::north, TileVector::north_east, TileVector::west, TileVector::east, TileVector::south_west, TileVector::south, TileVector::south_east}) {}
		~Queen() override {}
	};
}

#endif