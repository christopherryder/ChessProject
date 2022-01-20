/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to try and decouple user input from the game/board methods! This would allow for easy porting to other systems, such as a GUI, e.g using SFML or Universal Chess Interface.
*/

#ifndef __GAMEINPUTHANDLER_HEADER
#define __GAMEINPUTHANDLER_HEADER

#include <string>
#include <regex>
#include <sstream>

#include "Board.h"

namespace Chess {

	enum class InputHandlerState
	{
		Active,
		Resign,
		Quit
	};

	class InputHandler
	{
	public:
		InputHandler(std::shared_ptr<Board> board_handle);
		~InputHandler() {}

		void listen(std::istream& stream);
		void get_promotion();
		bool dispatch_command(const std::string& command_input);

		const InputHandlerState get_handler_state() const { return m_state; }

		//need to test moves which require regexes explicitly, i think. don't think there is an easy way to forward parameters with commands?
		bool try_execute_move(const std::string& command_input);
		bool try_execute_print_piece_moves(const std::string& command_input);

	private:

		InputHandlerState m_state{ InputHandlerState::Active };

		std::shared_ptr<Board> m_board_handle;

		//is the command for a move or to show the moves?
		std::regex m_move_regex{ R"((\w{2})\;(\w{2}))" };
		std::regex m_show_move_regex{ R"((\w{2})\?)" };

		std::string m_undo_query{ "undo" };
		std::string m_pieces_query{ "?" };
		std::string m_resign_query{ "resign" };
		std::string m_quit_query{ "quit" };
	};
}

#endif