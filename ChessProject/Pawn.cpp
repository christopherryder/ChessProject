#include "Pawn.h"

Chess::MoveList Chess::Pawn::generate_capture_list(Board& board) const
{
	MoveList capture_list(m_position);

	//Not sensible to add enpassant captures to this list as they can't check a king.
	for (const auto vector : m_attack_vectors) {

		int destination_position{ m_position + vector };

		if (board[destination_position] != m_faction) {

			capture_list.push_back(std::make_unique<CaptureCommand>(m_position, destination_position));
		}
	}

	return capture_list;
}

Chess::MoveList Chess::Pawn::generate_move_list(Board& board, const std::vector<int> move_vectors)
{
	MoveList move_list(m_position);

	for (const auto vector : move_vectors) {

		int destination_position{ m_position + vector };

		//if the position two along is a border then this is the realm of promotions.
		if (board[destination_position] == TileType::Empty && board[destination_position+vector] != TileType::Border) {

			MoveCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<MoveCommand>(command));
		}

		//consider a double push/promotion
		destination_position += vector;

		if (board[destination_position] == TileType::Empty) {

			if (m_move_count == 0) {

				DoublePushCommand command(m_position, destination_position);
				if (command.validate(board))
					move_list.push_back(std::make_unique<DoublePushCommand>(command));
			}
		}
		else if (board[destination_position] == TileType::Border) {

			PromotionCommand command(m_position, (destination_position - vector));
			if (command.validate(board))
				move_list.push_back(std::make_unique<PromotionCommand>(command));
		}
	}

	//consider attacks
	for (const auto& vector : m_attack_vectors) {

		int destination_position = m_position + vector;

		//if the tile is not in our faction and not un-aligned, attack.
		if ((board[destination_position] != m_faction) && (board[destination_position] != Faction::NonAligned)) {

			CaptureCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<CaptureCommand>(command));
		}
	}

	//consider enpassants.
	for (const auto& vector : m_enpassant_vectors) {

		//we capture the piece to the left or the right.
		int capture_position = m_position + vector;

		//we move to the position behind the capture.
		int destination_position = capture_position + get_move_direction();

		if (board.can_capture_enpassant(capture_position)) {

			EnpassantCommand command(m_position, destination_position, capture_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<EnpassantCommand>(command));
		}
	}

	if (m_pinned) {

		m_pinned_move_vectors.clear();
		m_pinned = false;
	}

	return move_list;
}
