#include "Piece.h"

#include "PieceFactory.h"
#include "Tile.h"

#include "ScanningPiece.h"
#include "King.h"
#include "Pawn.h"

std::unique_ptr<Chess::Piece> Chess::PieceFactory::create_piece(const char type, const int position) const
{
	std::unique_ptr<Piece> piece;

	switch (type)
	{
	case 'P':
		piece = std::make_unique<Pawn>(Faction::White, position);
		break;

	case 'N':
		piece = std::make_unique<Knight>(Faction::White, position);
		break;

	case 'B':
		piece = std::make_unique<Bishop>(Faction::White, position);
		break;

	case 'R':
		piece = std::make_unique<Rook>(Faction::White, position);
		break;

	case 'Q':
		piece = std::make_unique<Queen>(Faction::White, position);
		break;

	case 'K':
		piece = std::make_unique<King>(Faction::White, position);
		break;

	case 'p':
		piece = std::make_unique<Pawn>(Faction::Black, position);
		break;

	case 'n':
		piece = std::make_unique<Knight>(Faction::Black, position);
		break;

	case 'b':
		piece = std::make_unique<Bishop>(Faction::Black, position);
		break;

	case 'r':
		piece = std::make_unique<Rook>(Faction::Black, position);
		break;

	case 'q':
		piece = std::make_unique<Queen>(Faction::Black, position);
		break;

	case 'k':
		piece = std::make_unique<King>(Faction::Black, position);
		break;
	default:
		break;
	}

	return piece;
}

std::pair<Chess::Tile, std::unique_ptr<Chess::Piece>> Chess::PieceFactory::create_tile_piece_pair(const char type, const int position) const
{
	std::pair<Chess::Tile, std::unique_ptr<Chess::Piece>> pair;

	switch (type)
	{
	case 'P':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Pawn, Faction::White), std::make_unique<Pawn>(Faction::White, position));
		break;

	case 'N':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Knight, Faction::White), std::make_unique<Knight>(Faction::White, position));
		break;

	case 'B':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Bishop, Faction::White), std::make_unique<Bishop>(Faction::White, position));
		break;

	case 'R':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Rook, Faction::White), std::make_unique<Rook>(Faction::White, position));
		break;

	case 'Q':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Queen, Faction::White), std::make_unique<Queen>(Faction::White, position));
		break;

	case 'K':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::King, Faction::White), std::make_unique<King>(Faction::White, position));
		break;

	case 'p':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Pawn, Faction::Black), std::make_unique<Pawn>(Faction::Black, position));
		break;

	case 'n':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Knight, Faction::Black), std::make_unique<Knight>(Faction::Black, position));
		break;

	case 'b':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Bishop, Faction::Black), std::make_unique<Bishop>(Faction::Black, position));
		break;

	case 'r':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Rook, Faction::Black), std::make_unique<Rook>(Faction::Black, position));
		break;

	case 'q':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::Queen, Faction::Black), std::make_unique<Queen>(Faction::Black, position));
		break;

	case 'k':
		pair = std::pair<Tile, std::unique_ptr<Piece>>(Tile(TileType::King, Faction::Black), std::make_unique<King>(Faction::Black, position));
		break;
	default:
		break;
	}

	return pair;
}
