#include "Piece.h"

#include <cassert>
#include <algorithm>

Chess::MoveList Chess::Piece::generate_move_list(Board& board)
{
	//overloaded function generates movelist with a certain set of vectors depending on wether the piece is pinned or not.
	if (m_pinned)
		return generate_move_list(board, m_pinned_move_vectors);

	return generate_move_list(board, m_move_vectors);
}

Chess::MoveList Chess::Piece::generate_capture_list(Board& board) const
{
	MoveList capture_list(m_position);

	for (const auto vector : m_move_vectors) {

		int destination_position{ m_position + vector };

		if (board[destination_position] != m_faction && board[destination_position] != Faction::NonAligned) {

			capture_list.push_back(std::make_unique<MoveCommand>(m_position, destination_position));
		}
	}

	return capture_list;
}

const int Chess::Piece::get_alignment_vector(const Board& board, const int position) const
{
	//it's not sensical for non-scanning pieces to be aligned, they can only attack/move in a unit vector.
	return 0;
}

const Chess::Pin Chess::Piece::generate_pins(const int enemy_king_position, const Board& board) const
{
	//return the empty pin, non scanning pieces can not pin units.
	return Pin();
}

void Chess::Piece::pin(const int restriction_vector)
{
	std::vector<int> restriction{ restriction_vector, -restriction_vector };

	m_pinned = true;

	//this is perfectly valid as the move_vectors are initialised in sorted order by design. but just incase...
	assert(std::is_sorted(m_move_vectors.begin(), m_move_vectors.end()) && "Move vectors are not sorted!");

	//cycle the move vector and only add to the pinned vector the elements in common with the pin and the original move vector.
	for (const auto& vector : restriction) {

		auto iterator = std::find(m_move_vectors.begin(), m_move_vectors.end(), vector);
		if (iterator != m_move_vectors.end())
			m_pinned_move_vectors.push_back(*iterator);
	}
}

Chess::MoveList Chess::Piece::generate_move_list(Board& board, const std::vector<int> move_vectors)
{
	MoveList move_list(m_position);

	for (const auto vector : move_vectors) {

		int destination_position{ m_position + vector };

		if (board[destination_position] == TileType::Empty || ((board[destination_position] != m_faction) && board[destination_position] != Faction::NonAligned))
		{
			MoveCommand command(m_position, destination_position);
			if(command.validate(board))
				move_list.push_back(std::make_unique<MoveCommand>(command));
		}
		else if ((board[destination_position] != m_faction) && board[destination_position] != Faction::NonAligned)
		{
			CaptureCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<CaptureCommand>(command));
		}
	}

	if (m_pinned)
	{
		m_pinned_move_vectors.clear();
		m_pinned = false;
	}

	return move_list;
}
