#include "Tile.h"
#include <ostream>

namespace Chess {
	std::ostream& operator<<(std::ostream& os, const Tile& tile)
	{
		switch (tile.getTileType())
		{
		case TileType::Empty:
			os << " ";
			break;
		case TileType::Border:
			os << "!!!";
			break;
		case TileType::Pawn:
			(tile == Faction::White) ? os << "P" : os << "p";
			break;
		case TileType::Knight:
			(tile == Faction::White) ? os << "N" : os << "n";
			break;
		case TileType::Bishop:
			(tile == Faction::White) ? os << "B" : os << "b";
			break;
		case TileType::Rook:
			(tile == Faction::White) ? os << "R" : os << "r";
			break;
		case TileType::Queen:
			(tile == Faction::White) ? os << "Q" : os << "q";
			break;
		case TileType::King:
			(tile == Faction::White) ? os << "K" : os << "k";
			break;
		default:
			//should not be printed, so would be surprising to see this!
			os << "???";
			break;
		}
		return os;
	}
}
