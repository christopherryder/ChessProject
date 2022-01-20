#include "King.h"

Chess::MoveList Chess::King::generate_move_list(Board& board, const std::vector<int> move_vectors)
{
	MoveList move_list(m_position);

	//all king moves must be validated!
	for (const auto vector : move_vectors) {

		int destination_position{ m_position + vector };

		if (board[destination_position] == TileType::Empty) {

			KingMoveCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<KingMoveCommand>(command));
		}
		else if ((board[destination_position] != m_faction) && board[destination_position] != Faction::NonAligned) {

			//captures are added as a separate command.
			KingCaptureCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<KingCaptureCommand>(command));
		}
	}

	//cycle the castle vectors.
	for (const auto vector : m_castle_vectors) {

		//check if the castle is legal by game-rules.
		if (m_move_count != 0 || (board.can_castle_in_vector(vector) == false))
			continue;

		int destination_position{ m_position + vector };

		while (board[destination_position] != TileType::Border) {

			//found a suitable castling rook. 
			if (board[destination_position] == m_faction && board[destination_position] == TileType::Rook) {

				CastleCommand command(m_position, (m_position + (m_castle_range * vector)), destination_position, ((m_position - vector) + (m_castle_range * vector)), vector);
				if (command.validate(board))
					move_list.push_back(std::make_unique<CastleCommand>(command));
			}
			else if (board[destination_position] == m_faction) {

				//occupied by a friendly piece, we can't castle /through/ it.
				break;
			}

			destination_position += vector;
		}
	}

	return move_list;
}
