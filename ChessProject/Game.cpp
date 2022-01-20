#include "Game.h"

Chess::Game::Game(const FEN& game_state) : m_handler(m_board)
{
	m_board = std::make_shared<Board>(game_state);
	m_handler = InputHandler(m_board);
	play();
}

void Chess::Game::checkmate()
{
	std::cout << "\nGame over! Checkmate! " << m_board->get_enemy().get_name() << " is victorious!\n";
}

void Chess::Game::stalemate()
{
	std::cout << "\nGame over! Stalemate...\n";
}

void Chess::Game::resign()
{
	std::cout << m_board->get_player().get_name() << " has resigned! " << m_board->get_enemy().get_name() << " is victorious!\n";
}

void Chess::Game::quit()
{
	std::cout << "Quitting game...\n";
}

const bool Chess::Game::is_game_over()
{
	InputHandlerState state = m_handler.get_handler_state();

	if (state == InputHandlerState::Active) {
		return false;
	}
	if (state == InputHandlerState::Quit) {
		quit();
		return true;
	}
	else if(state == InputHandlerState::Resign) {
		resign();
		return true;
	}
	else {

		std::cout << "Input handler stopped listening with unknown state.\n";
		return true;
	}
}

void Chess::Game::end_game(const BoardState state)
{
	if (state == BoardState::Checkmate)
		checkmate();
	else if (state == BoardState::Stalemate)
		stalemate();
	else
		std::cout << "Game ended - unknown lose-state.\n";

	m_game_ended = true;
}

void Chess::Game::play()
{
	std::cout << "\n\nThe game has begun: Please enter 'help' for a list of commands.\n\n";
	//check for quits and mates.
	while (!is_game_over())
	{
		//swap the players around
		m_board->rotate_players();
		m_board->determine_current_check_state();
		m_board->assign_piece_pins();
		m_board->generate_composite_movelist();
		m_board->pretty_print_board();

		//take input if there is no mates.
		BoardState state = m_board->state();
		if (state == BoardState::Legal) {

			std::cout << m_board->get_player().get_name() << " to play! " << m_board->get_enemy().get_name() << " is the enemy...\n";
			if (m_board->get_current_check_state())
				std::cout << "You are in Check!\n";

			//let the input handler deal with input.
			m_handler.listen(std::cin);

			//if the input resulted in a promotion, let the input handler deal with it.
			if (m_board->is_promotion()) {

				m_handler.get_promotion();
			}
		}
		else {

			end_game(state);
			break;
		}
	}
}

