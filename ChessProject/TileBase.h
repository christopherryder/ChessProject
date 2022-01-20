/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves as a general implementation of a game-board tile.
*/

#ifndef __TILEBASE_HEADER
#define __TILEBASE_HEADER

//Template class, takes a tile type and a faction.
template<class TileTypeEnum, class FactionEnum>
class TileBase
{
public:

	TileBase(const TileTypeEnum& tiletype, const FactionEnum& faction) : m_tiletype(tiletype), m_faction(faction) {}
	virtual ~TileBase() = 0;

	inline const bool operator==(const TileTypeEnum type) const { return(m_tiletype == type); }
	inline const bool operator==(const FactionEnum faction) const { return(m_faction == faction); }
	inline const bool operator!=(const TileTypeEnum type) const { return(m_tiletype != type); }
	inline const bool operator!=(const FactionEnum faction) const { return(m_faction != faction); }

	inline const TileTypeEnum getTileType() const { return m_tiletype; }
	inline const FactionEnum getFaction() const { return m_faction; }

protected:
	TileTypeEnum m_tiletype;
	FactionEnum m_faction;
};

template<class TileTypeEnum, class FactionEnum>
inline TileBase<TileTypeEnum, FactionEnum>::~TileBase() = default;
#endif