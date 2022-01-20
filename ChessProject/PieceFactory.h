/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class creates a piece belonging to a given faction given a single char. Refactoring of this method to its own class helped to 'organise' the board class.
*/

#ifndef __PIECEFACTORY_HEADER
#define __PIECEFACTORY_HEADER

/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to produce a certain piece/tile given a char.
*/

#include <memory>

namespace Chess
{
	class Piece;

	class PieceFactory
	{
	public:
		PieceFactory() {}
		~PieceFactory() {}

		PieceFactory(PieceFactory&& piece_factory) = default;
		PieceFactory& operator=(PieceFactory&& piece_factory) = default;

		PieceFactory(const PieceFactory& piece_factory) = delete;
		PieceFactory& operator=(const PieceFactory& piece_factory) = delete;

		std::unique_ptr<Piece> create_piece(const char type, const int position) const;
		std::pair<Tile, std::unique_ptr<Piece>> create_tile_piece_pair(const char type, const int position) const;
	};
}

#endif