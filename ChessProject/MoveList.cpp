#include "MoveList.h"

#include <ostream>
#include <cassert>

namespace Chess {

	MoveList& MoveList::operator=(MoveList&& movelist) noexcept
	{
		assert((this != &movelist) && "Self assignment not allowed!");
		m_movelist = std::move(movelist.m_movelist);
		m_origin = movelist.m_origin;

		return *this;
	}

	std::ostream& operator<<(std::ostream& os, const MoveList& movelist)
	{
		os << "[" << movelist.get_origin() << "] --> ";
		for (const auto& it : movelist.m_movelist)
			os << "[" << it->get_to() << "]";
		return os;
	}
}