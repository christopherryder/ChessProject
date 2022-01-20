#include "Board.h"
#include "MoveCommand.h"

void Chess::MoveCommand::execute(Board& board)
{
	board.move(m_from, m_to);
}

void Chess::MoveCommand::undo(Board& board)
{
	board.undo_move(m_to, m_from);
}

bool Chess::MoveCommand::validate(Board& board)
{
	bool is_valid{ true };
	
	//validate a move only if in check
	if (board.get_current_check_state()) {

		board.move(m_from, m_to);
		bool in_check = board.is_in_check();
		board.undo_move(m_to, m_from);

		//if in_check is true, move is not valid.f
		return !in_check;
	}

	return true;
}

void Chess::CaptureCommand::execute(Board& board)
{
	board.capture(m_to);
	board.move(m_from, m_to);
}

void Chess::CaptureCommand::undo(Board& board)
{
	board.undo_move(m_to, m_from);
	board.revive();
}

bool Chess::CaptureCommand::validate(Board& board)
{
	bool is_valid{ true };

	//validate only if the board is in check.
	if (board.get_current_check_state()) {

		//Captures need to be validated by explicitly making/unmaking the capture.
		execute(board);
		is_valid = !board.is_in_check();
		undo(board);
	}

	return is_valid;
}

bool Chess::KingMoveCommand::validate(Board& board)
{
	//needs to always validate, regardless of check state.
	board.move(m_from, m_to);
	bool in_check = board.is_in_check();
	board.undo_move(m_to, m_from);

	//if in_check is true, move is not valid.
	return !in_check;
}

bool Chess::KingCaptureCommand::validate(Board& board)
{
	//king moves must always be validated, captures by explicitly making the move.
	execute(board);
	bool in_check = board.is_in_check();
	undo(board);

	//if in_check is true, move is not valid.
	return !in_check;
}

void Chess::CastleCommand::execute(Board& board)
{
	board.move(m_from, m_to);
	board.move(m_castle_from, m_castle_to);
}

void Chess::CastleCommand::undo(Board& board)
{
	board.undo_move(m_to, m_from);
	board.undo_move(m_castle_to, m_castle_from);
}

bool Chess::CastleCommand::validate(Board& board)
{
	if (board.get_current_check_state())
		return false;

	for (auto sub_to{ m_from + m_vector }; sub_to != (m_to + m_vector); sub_to += m_vector) {

		//create a temporary sub-move command and execute them all.
		KingMoveCommand temp_move(m_from, sub_to);
		
		if(!temp_move.validate(board)) {

			//if any of the sub moves result in a check then the castle is not valid.
			return false;
		}
	}
	return true;
}

void Chess::DoublePushCommand::execute(Board& board)
{
	MoveCommand::execute(board);
	board.update_enpassant_position(m_to);
}

void Chess::DoublePushCommand::undo(Board& board)
{
	MoveCommand::undo(board);
	board.update_enpassant_position(0);
}

void Chess::EnpassantCommand::execute(Board& board)
{
	//capture at the enpassant position not the to position!
	board.capture(m_capture);
	board.move(m_from, m_to);
}

void Chess::EnpassantCommand::undo(Board& board)
{
	board.undo_move(m_to, m_from);
	board.revive();
}

bool Chess::EnpassantCommand::validate(Board& board)
{
	//enpassants need to always be validated to account for discovered checks
	execute(board);
	bool in_check = board.is_in_check();
	undo(board);

	//if in_check is true, move is not valid.
	return !in_check;
}

void Chess::PromotionCommand::execute(Board& board)
{
	//flags to the board, and by extension the game/event handler that a promotion is available.
	board.flag_promotion(std::pair<int, int>(m_from, m_to));
}

void Chess::PromotionCommand::undo(Board& board)
{
	board.undo_promote();
}

bool Chess::PromotionCommand::validate(Board& board)
{
	//just move the piece instead of executing the command, this can never be a capture so it is safe to do. (Pawns don't capture in the direction of motion).
	board.move(m_from, m_to);
	bool in_check = board.is_in_check();
	board.undo_move(m_to, m_from);

	return !in_check;
}
