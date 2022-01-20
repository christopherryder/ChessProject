/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves as a basic abstraction of what a game 'piece' is and what it may do.
*/

#ifndef __PIECEBASE_HEADER
#define __PIECEBASE_HEADER

template<class FactionEnum>

class PieceBase
{
public:
	PieceBase(const FactionEnum faction, const int position) : m_faction(faction), m_position(position) {}
	virtual ~PieceBase() = 0;

	//a piece may move from one position to another
	virtual void move(const int position) = 0;
	virtual void undo_move(const int position) = 0;

	//a piece has a faction
	inline const FactionEnum get_faction() const { return m_faction; }

	//need to uncover this for piece-list sort predicate.
	inline const int get_position() const { return m_position; }

protected:
	int m_position;
	FactionEnum m_faction;
};

template<class FactionEnum>
inline PieceBase<FactionEnum>::~PieceBase() = default;
#endif