/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves as a conceptual base of a 'game board'
*/

#ifndef __BOARDBASE_HEADER
#define __BOARDBASE_HEADER

#include <vector>

//board consists of a collection of 'tiletypes'
template<int X, int Y, class TileType>
class BoardBase
{
public:
	BoardBase() {}
	virtual ~BoardBase() = 0;


	//need to be able to traverse the board, it is unavoidable. 
	virtual const TileType& operator[](const int position) const = 0;

	//required for pieces to navigate around the board.
	//templating allows to keep these as constexpr. (compile time duck-typing).
	struct TileVector
	{
		static constexpr int north = -X;
		static constexpr int east = +1;
		static constexpr int south = +X;
		static constexpr int west = -1;

		static constexpr int north_east = north + east;
		static constexpr int south_east = south + east;
		static constexpr int south_west = south + west;
		static constexpr int north_west = north + west;

		static constexpr int north_north_east = north + north_east;
		static constexpr int north_east_east = north_east + east;
		static constexpr int south_east_east = south_east + east;
		static constexpr int south_south_east = south + south_east;
		static constexpr int south_south_west = south + south_west;
		static constexpr int south_west_west = south_west + west;
		static constexpr int north_west_west = north_west + west;
		static constexpr int north_north_west = north + north_west;
	};

protected:

	//imagine the case someone sets up a chessboard without any kings! or multiple...
	virtual const bool is_board_valid() const = 0;
	
	//the board has some dimension
	int m_board_x{ X };
	int m_board_y{ Y };
};

template<int X, int Y, class TileType>
inline BoardBase<X, Y, TileType>::~BoardBase() = default;
#endif