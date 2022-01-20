#include "InputHandler.h"


Chess::InputHandler::InputHandler(std::shared_ptr<Board> board_handle) : m_board_handle(board_handle) {}

void Chess::InputHandler::listen(std::istream& stream)
{
	std::cout << "Please enter a move: ";

	std::string input;
	std::getline(stream, input, '\n');

	//transform the input to all lower case using a lambda
	std::transform(input.begin(), input.end(), input.begin(),
		[](unsigned char c) { return std::tolower(c); });

	//if the dispatch was not successful, take another command.
	if (!dispatch_command(input))
		listen(std::cin);
}

void Chess::InputHandler::get_promotion()
{
	std::cout << "Please enter the initial (case-sensitive!) of the piece type you wish to promote to:\n";

	
	std::string promotion;

	while (std::getline(std::cin, promotion, '\n')) {

		if (promotion.length() != 1) {

			std::cout << "Please enter just the initial of the piece!\n";
		}
		else {

			std::size_t found = (m_board_handle->get_player_faction() == Faction::White) ? promotion.find_first_of("BNQR") : promotion.find_first_of("bnqr");

			if (found != std::string::npos) {
				m_board_handle->promote(promotion[found]);
				break;
			}
			else {
				std::cout << "Invalid token entered! Please enter one of either ";
				(m_board_handle->get_player_faction() == Faction::White) ? std::cout << "'B' 'N' 'Q' 'R'\n" : std::cout << "'b' 'n' 'q' 'r'\n";
			}
		}
	}
}

//if you ever wanted to see why medium-compact brace notation looks bad, check this function!
bool Chess::InputHandler::dispatch_command(const std::string& command_input)
{
	bool stop_listening{ false };

	//need to attempt to look up the command in the map, don't see how this is easily possible for the input because its so varied. I also struggle to pass parameters to command.
	if (try_execute_move(command_input)) {

		//if the move was 'successfull' the handler can stop listening to the current player.
		stop_listening = true;
	}
	else if (try_execute_print_piece_moves(command_input)) {

		//asking for the moves to be printed doesn't terminate a turn.
		stop_listening = false;
	}
	else if (command_input == "undo") {

		//undoing a turn terminates the move for that player, if it was successful.
		stop_listening = m_board_handle->execute_undo_command();
	}
	else if (command_input == "help") {

		std::cout << "\nHelp:\n"
			<< "To enter a move please type it in the format: 'a1;a2' (Two chessboard coordinates separated by a semi-colon).\n"
			<< "Pieces belonging to the playing faction are denoted by the initial, representing the piece type encased in square backets\ne.g: [R] = White rook.\n\n"
			<< "To view the available moves of a specific piece please enter the coordinate of the piece followed by a question mark:\nE.g: 'a1?'.\n\n"
			<< "To resign a game please enter 'resign'.\n"
			<< "To quit a game please enter 'quit'.\n\n";

		//asking for help doesn't terminate a move.
		stop_listening = false;
	}
	else if (command_input == "quit") {

		m_state = InputHandlerState::Quit;
		stop_listening = true;
	}
	else if (command_input == "resign") {

		m_state = InputHandlerState::Resign;
		stop_listening = true;
	}
	else {

		std::cout << "Command was not executed. Type 'help' for help.\n";
		stop_listening = false;
	}

	return stop_listening;
}

bool Chess::InputHandler::try_execute_move(const std::string& command_input)
{
	bool command_success{ false };

	if (std::regex_match(command_input, m_move_regex))
	{
		//this is a valid move command.
		bool move_success{ false };

		std::smatch matches;
		std::regex_search(command_input, matches, m_move_regex);

		std::string algebraic_from(matches[1]);
		std::string algebraic_to(matches[2]);

		auto from_iterator = m_board_handle->convert_coordinate_to_position(algebraic_from);

		if (from_iterator.first)
		{
			auto to_iterator = m_board_handle->convert_coordinate_to_position(algebraic_to);

			if (to_iterator.first)
			{
				int position_from = from_iterator.second;
				int position_to = to_iterator.second;

				bool move_success = m_board_handle->find_and_execute_move(position_from, position_to);

				if (!move_success)
					std::cout << "Invalid Move: [" << algebraic_from << ", " << algebraic_to << "].\n";
				
				command_success = move_success;
			}
		}

	}

	return command_success;
}

bool Chess::InputHandler::try_execute_print_piece_moves(const std::string& command_input)
{
	bool command_success{ false };

	if (std::regex_match(command_input, m_show_move_regex))
	{
		//this is a valid show move command.
		bool move_success{ false };

		std::smatch matches;
		std::regex_search(command_input, matches, m_show_move_regex);

		std::string algebraic_from(matches[1]);

		auto from_iterator = m_board_handle->convert_coordinate_to_position(algebraic_from);

		if (from_iterator.first)
		{

			int position_from = from_iterator.second;

			bool move_success = m_board_handle->find_and_execute_pretty_print(position_from);

			if (!move_success)
				std::cout << "Invalid Selection: [" << algebraic_from << "] has no moves!\n";

			command_success = move_success;
		}
	}

	return command_success;
}


