#include <iostream>
#include <string>
#include <sstream>

#include "Game.h"
#include "FEN.h"

const int get_int_in_range(int min, int max)
{
	double positive_int{};
	std::string user_input{};

	while (std::getline(std::cin, user_input,'\n')) {
		std::stringstream s_stream(user_input);

		if (s_stream >> positive_int) {
			std::size_t find_decimal = user_input.find('.');
			//if we were able to pass the entire stream to the int, and it obeys our coniditons then we can accept it.
			if (s_stream.eof() && positive_int >= min && positive_int <= max && find_decimal == std::string::npos)
				break;
		}
		std::cout << "\nPlease enter a valid selection, an integer between " << min << " and " << max << ". " << "You entered: '" << user_input << "'.\n";
	}
	return static_cast<int>(positive_int);
}

void game_from_fen()
{
	std::cout << "Please enter exactly the FEN expression or enter 'q' to cancel:\n";
	std::string user_input{};

	while (std::getline(std::cin, user_input, '\n'))
	{
		std::cout << "User input : " << user_input << "\n";

		if (user_input == "q" || user_input == "Q")
			break;
		
		//FEN creation may fail if the regex cannot match the expression.
		try {
			FEN fen(user_input);
			Chess::Game game(fen);
		}
		catch (std::exception& e) {

			std::cout << "Error: " << e.what() << "\n";
		}
		std::cout << "Invalid FEN! please either enter another or 'q' to cancel.\n";
	}
}

int main()
{
	std::cout << "--------------------------------\n"
		<< "   _____ _                   \n"
		<< "  / ____| |                  \n"
		<< " | |    | |__   ___  ___ ___ \n"
		<< " | |    |  _ \\ / _ \\/ __/ __|\n"
		<< " | |____| | | |  __/\\__ \\__ \\\n"
		<< "  \\_____|_| |_|\\___||___/___/\n"
		<< "--------------------------------\n\n"
		<< "---------------------------------------------------\n"
		<< "Author: Christopher Ryder                          \n"
		<< "Email: Christopher.Ryder-2@student.manchester.ac.uk\n"
		<< "---------------------------------------------------\n";

	bool run{ true };

	while (run)
	{

		//ADD STALEMATE, CHECKMATE, 50MOVE DRAW.
		std::cout << "\nWelcome to the chess driver program. This program was designed to be a full implementation of standard chess\n"
			<< "and it's associated rules.\n\n"
			<< "In order to showcase this several 'chess games' have been preprepared and are available to select from the list\n"
			<< "below:\n\n"
			<< "Note: This program builds chess boards using Forsyth-Edward Notation (FEN), see:\nhttps://en.wikipedia.org/wiki/Forsyth-Edwards_Notation\n\n"
			<< "[0]. Create Chess game from custom FEN.\n"
			<< "[1]. Standard Chess. [FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1]\n"
			<< "--------------------------------------------------------------------\n"
			<< "[2].  Debug: Enpassant Check Evasion: [Double push the white pawn to check, black can evade by enpassant capture.]\n"
			<< "[3].  Debug: Double Check: Black is in double check, and hence only king moves are legal.\n"
			<< "[4].  Debug: King Check Evasions: Black is in check and thus querying its moves will only display moves which remove the check.\n"
			<< "[5].  Debug: King Check Evasions: Black King is in check by a white rook and should understand that moves in the same vector of the check are invalid.\n"
			<< "[6].  Debug: King Single Check Evasions: Black king is in check, however a move by a friendly piece can remedy this, and is such valid.\n"
			<< "[7].  Debug: Pinned Pieces: Black's rook must be restrained to moving in one vector only.\n"
			<< "[8].  Debug: Enpassant Discovered Check: If black were to make the enpassant capture, it would result in a check.\n"
			<< "[9].  Debug: Castling: Kings can castle, but only once. They can not castle through tiles that would be attacked or if in check.\n"
			<< "[10]. Debug: Promotion: Pawns reaching the end of the board can promote themselves into another pieces (NOT pawn, NOT, king).\n"
			<< "[11]. Debug: Promotion Check Evasion: Moving a pawn to promotion is a valid check evasion.\n"
			<< "[12]. Debug: Fifty Move draw.\n"
			<< "[13]. Debug: Stalemate.\n"
			<< "[14]. Exit.\n"
			<< "\nPlease select an option:\n";

		int selection = get_int_in_range(0, 14);

		switch (selection)
		{
		//custom FEN game.
		case 0:
		{
			game_from_fen();
			break;
		}
		//standard game.
		case 1:
		{
			FEN fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			Chess::Game game(fen);
			break;
		}

		//debug: enpassant check evasion
		case 2:
		{
			FEN fen("8/8/8/2k5/4p3/8/3P4/4K3 w - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//double check
		case 3:
		{
			FEN fen("4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//check evasion
		case 4:
		{
			FEN fen("4k3/8/6B1/8/8/8/8/2KR4 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//check evasion
		case 5:
		{
			FEN fen("8/8/4k3/8/8/8/8/4RK2 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//single check evasion
		case 6:
		{
			FEN fen("8/8/4k1n1/b7/8/8/8/4RK2 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//pinned piece
		case 7:
		{
			FEN fen("4k3/8/4r3/8/8/8/4Q3/4K3 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//enpassant discovered check
		case 8:
		{
			FEN fen("8/8/8/8/k4p1Q/8/4P3/4K3 w - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//castling
		case 9:
		{
			FEN fen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
			Chess::Game game(fen);
			break;
		}
		//pawn promotion
		case 10:
		{
			FEN fen("4k3/2P5/8/8/8/8/6p1/4K3 w - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//pawn promotion check evasion
		case 11:
		{
			FEN fen("K7/1P4rk/8/8/8/8/8/8 b - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//fifty move draw
		case 12:
		{
			FEN fen("8/8/3k4/8/8/3K4/8/8 w - - 49 1");
			Chess::Game game(fen);
			break;
		}
		//stalemate
		case 13:
		{
			FEN fen("4k3/4p3/4Q3/2B5/8/8/8/4K3 w - - 0 1");
			Chess::Game game(fen);
			break;
		}
		//exit
		case 14:
		{
			std::cout << "Thanks for playing!\n";
			run = false;
			break;
		}

		default:
		{
			std::cout << "Unknown command entered!\n";
			break;
		}

		}
	}

	return 0;
}