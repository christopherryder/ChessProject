#include "ScanningPiece.h"

Chess::MoveList Chess::ScanningPiece::generate_capture_list(Board& board) const
{
	MoveList capture_list(m_position);

	for (const auto& vector : m_move_vectors) {

		//if the piece is a scanning piece we need to 'keep moving' through the tiles.
		int destination_position{ m_position + vector };

		//cycle empty tiles
		while (board[destination_position] == TileType::Empty)
			destination_position += vector;

		//if the next tile which is not empty is a valid attack add it to the attack list
		if (board[destination_position] != m_faction && board[destination_position] != Faction::NonAligned) {

			capture_list.push_back(std::make_unique<CaptureCommand>(m_position, destination_position));
		}
	}

	return capture_list;
}

const int Chess::ScanningPiece::get_alignment_vector(const Board& board, const int position) const
{
	//scan each direction until we reach the border in order to determine if we're aligned with the given position or not.
	for (const auto& vector : m_move_vectors) {

		int destination_position{ m_position + vector };
		while (board[destination_position] != TileType::Border) {

			if (destination_position == position)
				return vector;

			destination_position += vector;
		}
	}
	return 0;
}

const Chess::Pin Chess::ScanningPiece::generate_pins(const int enemy_king_position, const Board& board) const
{
	Pin pin;

	int block_count{ 0 };
	int block_position{ 0 };

	int alignment_vector{ get_alignment_vector(board, enemy_king_position) };

	//we are aligned
	if (alignment_vector != 0) {

		int destination_positon{ m_position + alignment_vector };

		//scan until we encounter the 'aligned' king but we must also not be blocked by a friendly tile.
		while (board[destination_positon] != TileType::King && board[destination_positon] != m_faction) {

			if (board[destination_positon] != m_faction && board[destination_positon] != Faction::NonAligned) {

				block_position = destination_positon;
				++block_count;
			}
			destination_positon += alignment_vector;
		}
	}

	//only one piece lies between the scanning piece and the enemy king, and so if it moves it would expose check.
	if (block_count == 1) {

		pin.m_position = block_position;
		pin.m_vector = alignment_vector;
		return pin;
	}

	//if not pinned, return the empty pin
	return pin;
}

Chess::MoveList Chess::ScanningPiece::generate_move_list(Board& board, const std::vector<int> move_vectors)
{
	MoveList move_list(m_position);

	for (const auto& vector : move_vectors) {

		//if the piece is a scanning piece we need to 'keep moving' through the tiles.
		int destination_position{ m_position + vector };

		//only add empty tiles
		while (board[destination_position] == TileType::Empty) {

			MoveCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<MoveCommand>(command));

			destination_position += vector;
		}
		//scanning pieces are allowed one capture.
		if (board[destination_position] != m_faction && board[destination_position] != Faction::NonAligned) {

			CaptureCommand command(m_position, destination_position);
			if (command.validate(board))
				move_list.push_back(std::make_unique<CaptureCommand>(command));
		}
	}

	//if pinned, reset pin-status (pins are calculated every turn).
	if (m_pinned) {

		m_pinned_move_vectors.clear();
		m_pinned = false;
	}

	return move_list;
}
