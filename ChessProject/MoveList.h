/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves as a wrapped for a vector of unique_ptrs of chess commands. The class appears to be a 'simple' wrapper of vector, which it mainly is, but this has been done for two
reasons. One to reduced the typing required of std::vector<std::unique_ptr<ChessCommandBase>>::iterator and secondly each 'movelist' is assigned an 'origin' tile to show where the 
moves emenate from. It could argued that I could store this class in the piece class directly to bypass the need for this one. But this seemed more convenient for assessing the endgame
i.e if the vector of movelists was empty, the game was over.
*/

#ifndef __MOVELIST_HEADER
#define __MOVELIST_HEADER

#include <vector>
#include <memory>
#include <ostream>
#include <algorithm>

#include "MoveCommand.h"

namespace Chess {

	class MoveList
	{
	public:

		MoveList(int origin) : m_origin(origin) {}

		MoveList(MoveList&& movelist) noexcept : m_movelist(std::move(movelist.m_movelist)), m_origin(movelist.m_origin) {}
		MoveList& operator=(MoveList&& movelist) noexcept;

		//the underlying container of this class is a vector of unique_ptrs so delete the copy operations
		MoveList(const MoveList& movelist) = delete;
		MoveList& operator=(const MoveList& movelist) = delete;

		//define iterators for range-based for loops and iterations of the vector.
		typedef typename std::vector<std::unique_ptr<ChessCommandBase>>::iterator iterator;
		typedef typename std::vector<std::unique_ptr<ChessCommandBase>>::const_iterator const_iterator;

		void push_back(std::unique_ptr<ChessCommandBase> command) { m_movelist.push_back(std::move(command)); }
		void sort() { std::sort(m_movelist.begin(), m_movelist.end(), [](const std::unique_ptr<ChessCommandBase>& list1, const std::unique_ptr<ChessCommandBase>& list2) {return(list1->get_to() < list2->get_to()); }); }

		inline const std::size_t size() const { return m_movelist.size(); }
		iterator begin() { return m_movelist.begin(); }
		iterator end() { return m_movelist.end(); }
		const_iterator begin() const { return m_movelist.begin(); }
		const_iterator end() const { return m_movelist.end(); }

		iterator erase(iterator location) { return(m_movelist.erase(location)); }
		
		const_iterator find_in_list(const int search_term) const { return std::find_if(m_movelist.begin(), m_movelist.end(), [search_term](const std::unique_ptr<ChessCommandBase>& command) -> bool { return command->get_to() == search_term; }); }
		iterator find_in_list(const int search_term){ return std::find_if(m_movelist.begin(), m_movelist.end(), [search_term](const std::unique_ptr<ChessCommandBase>& command) -> bool { return command->get_to() == search_term; }); }

		//need to expose this for sorting the move list based on origin tile.
		inline const int get_origin() const { return m_origin; }

		friend std::ostream& operator<<(std::ostream& os, const MoveList& movelist);

	private:
		int m_origin;
		std::vector<std::unique_ptr<ChessCommandBase>> m_movelist;
	};
}
#endif