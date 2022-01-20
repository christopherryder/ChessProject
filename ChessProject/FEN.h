/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class implements the methods to extract and parse FEN game notation using Regular Expressions.

Forsyth Edwards notation:
describes the board first, lower-case corresponding to black pieces, upper to white. 
The board is described in ranks, each rank separated by a '/', where numbers represent a number of spaces.
the next charachter, either a w/b represents whos turn it is. (white/black).
the next sequence represents castling rights: KQkq implies full castling rights for both sides, Kk implies only king-side castling for both sides, '-' implies no castling rights.
the next sequence is an algebraic chess position, (a1->h8) and represents wether there is currently a pawn available for enpassant captures, '-' indicates no enpassant position.
finally the two numbers represent the half and full move clock respectively.
*/

#ifndef __FEN_HEADER
#define __FEN_HEADER

#include <string>
#include <regex>

class FEN
{
public:
	FEN(const std::string game_configuration);
	~FEN() {}

	bool find_matching_expression();

	//extract information from our correctly parsed regex.
	const std::string get_board_configuration() const { return m_matches[1]; }
	const std::string get_player_to_play() const { return m_matches[2]; }
	const std::string get_castling_rights() const { return m_matches[3]; }
	const std::string get_enpassant_position() const { return m_matches[4]; }
	const std::string get_half_move_clock() const { return m_matches[5]; }
	const std::string get_full_move_clock() const { return m_matches[6]; }

private:

	std::smatch m_matches;
	std::string m_game_configuration;

	//https://regexr.com/ designed/tested the regex here.
	std::regex m_forsyth_edwards{ R"(((?:\w{1,8}\/){7}\w{1,8})\s([wb])\s([KQkq]{1,4}|\-)\s([abcdefgh][12345678]|\-)\s([0123456789]+)\s([0123456789]+))" };
};

#endif