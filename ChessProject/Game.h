/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to represent an overall 'manager' of the game and will interface with the input handler and the board. This class determines the lifespan
of the game.
*/

#ifndef __GAME_HEADER
#define __GAME_HEADER

#include <vector>
#include <unordered_map>
#include <string>

#include "FEN.h"
#include "Board.h"
#include "Player.h"

#include "InputHandler.h"

namespace Chess {

	class Game 
	{
	public:
		Game(const FEN& game_state);

		void play();

		inline const bool try_execute_move(const int from, const int to) const { return m_board->find_and_execute_move(from, to); }
		inline const bool try_execute_undo_move() const { return m_board->execute_undo_command(); }

	private:

		void end_game(const BoardState state);
		void resign();
		void quit();
		void checkmate();
		void stalemate();

		const bool is_game_over();
		bool m_game_ended{ false };

		//share the board with the input_handler
		std::shared_ptr<Board> m_board;

		InputHandler m_handler;
	};
}
#endif