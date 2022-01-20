//satisfy forward declaration first.
#include "Piece.h"

#include "Board.h"

#include <cassert>
#include <string>
#include <sstream>

//attack logic
#include "ScanningPiece.h"
#include "King.h"
#include "Pawn.h"

Chess::Board::Board(const FEN& fen_notation) : m_player(std::make_shared<Player>("White", Faction::White)), m_enemy(std::make_shared<Player>("Black", Faction::Black))
{
	//begin parsing the fen_notation.
	//firstly make the board,
	std::string board_representation = build_board_representation(fen_notation.get_board_configuration());
	make_board_from_representation(board_representation);

	//now check the board is valid
	if (!is_board_valid()) {

		throw std::logic_error("Logic Error: Invalid board configuration.");
	}

	//using FEN notation may require a conversion from chess position to int or coordinate position, generate a map using the board we just made.
	m_conversion_table = generate_conversion_table();

	//parse the castling rights
	fen_notation.get_castling_rights();

	//parse the enpassant
	std::string enpassant_coordinate = fen_notation.get_enpassant_position();

	auto enpassant_position = convert_coordinate_to_position(enpassant_coordinate);
	
	if (enpassant_position.first)
		m_enpassant_position = enpassant_position.second;
	else
		m_enpassant_position = 0;

	//parse clocks
	m_half_move_clock = std::stoi(fen_notation.get_half_move_clock());
	m_full_move_clock = std::stoi(fen_notation.get_full_move_clock());

	//set castling permission.
	for (const auto& permission : fen_notation.get_castling_rights()) {

		switch (permission)
		{
		case 'K':
			(m_player->get_faction() == Faction::White) ? m_player->add_castle_permission(TileVector::east, true) : m_enemy->add_castle_permission(TileVector::east, true);
			break;

		case 'k':
			(m_player->get_faction() == Faction::Black) ? m_player->add_castle_permission(TileVector::east, true) : m_enemy->add_castle_permission(TileVector::east, true);
			break;

		case 'Q':
			(m_player->get_faction() == Faction::White) ? m_player->add_castle_permission(TileVector::west, true) : m_enemy->add_castle_permission(TileVector::west, true);
			break;

		case 'q':
			(m_player->get_faction() == Faction::Black) ? m_player->add_castle_permission(TileVector::west, true) : m_enemy->add_castle_permission(TileVector::west, true);
			break;

		default:
			break;
		}
	}

	//determine the order of the order of the player.
	if (fen_notation.get_player_to_play() == "w") {

		std::swap(m_player, m_enemy);
	}
}

Chess::Board::Board(Board&& board) noexcept
{
	m_player = std::move(board.m_player);
	m_enemy = std::move(board.m_enemy);
	m_pieces = std::move(board.m_pieces);
	m_captured_pieces = std::move(board.m_captured_pieces);
	m_board = std::move(board.m_board);
	m_captured_tiles = std::move(board.m_captured_tiles);
	m_moves = std::move(board.m_moves);
	m_move_history = std::move(board.m_move_history);
	m_conversion_table = std::move(board.m_conversion_table);

	m_enpassant_position = board.m_enpassant_position;
	m_half_move_clock = board.m_half_move_clock;
	m_full_move_clock = board.m_full_move_clock;
	m_in_check = board.m_in_check;
}

//Chess::Board::Board(Board&& board) noexcept = default;
Chess::Board& Chess::Board::operator=(Board&& board) noexcept = default;
Chess::Board::~Board() {}

const std::string Chess::Board::build_board_representation(const std::string board_representation) const
{
	//strategy:
	//fill in neccessary borders, then build a board by iterating it and using a switch to generate a tile from a char.
	//essentially add a line of '#', then for each line, '#'+line+'#' and then for the final line, '#'.

	std::string new_board_representation;

	//split the board string by '/'
	std::vector<std::string> row_representations;
	std::string row;

	std::stringstream s_stream(board_representation);

	while (std::getline(s_stream, row, '/')) {

		//add the border tile to each row,
		row = "#" + row + "#";
		row_representations.push_back(row);
	}

	//the width of the top/bottom 'border-tile' border.
	std::size_t board_border_height = ((m_board_y - m_game_board_x) / 2);

	//concatenate the string
	new_board_representation += std::string(board_border_height * m_board_x, '#');

	for (const auto& row : row_representations)
		new_board_representation += row;

	new_board_representation += std::string(board_border_height * m_board_x, '#');

	return new_board_representation;
}

void Chess::Board::make_board_from_representation(const std::string board_representation)
{
	if (!m_board.empty() || !m_pieces.empty()) {

		m_board.clear();
		m_pieces.clear();
	}
	//due to the regex we can assume perfect input.
	int position{ 0 };

	//firstly handle the tiles that do not have piece representations, border tiles and 'empty' tiles.
	for (auto iterator = board_representation.begin(); iterator != board_representation.end(); ++iterator)
	{
		if (*iterator == '#') {

			m_board.push_back(Tile(TileType::Border, Faction::NonAligned));
			++position;
		}
		else if (std::isdigit(*iterator)) {

			int spacing = static_cast<int>(*iterator - '0');

			for (int i{ 0 }; i < spacing; ++i)
				m_board.push_back(Tile(TileType::Empty, Faction::NonAligned));

			//don't forget to move the 'position' along by the number of empty spaces!
			position += spacing;
		}
		else {

			auto tile_piece_pair(m_piece_factory.create_tile_piece_pair(*iterator, position));
			m_board.push_back(tile_piece_pair.first);
			m_pieces.push_back(std::move(tile_piece_pair.second));
			++position;
		}
	}
}

const std::pair<bool, int> Chess::Board::convert_coordinate_to_position(std::string coordinate) const
{
	auto iterator = m_conversion_table.find(coordinate);

	if (iterator != m_conversion_table.end()) {

		return std::pair<bool, int>(true, iterator->second);
	}

	//if we couldn't convert the coordinate inform the use in the bool.
	return std::pair<bool, int>(false, 0);
}

std::vector<Chess::MoveList>::iterator Chess::Board::find_movelist(const int origin)
{
	auto movelist = std::find_if(m_moves.begin(), m_moves.end(), [origin](const MoveList& movelist) -> bool { return movelist.get_origin() == origin; });
	return movelist;
}

void Chess::Board::print_move_list()
{
	std::cout << "Moves Generated for " << m_player->get_name() << ":\n";
	for (const auto& moves : m_moves)
		std::cout << moves << "\n";
}

const Chess::BoardState Chess::Board::state() const
{
	if (m_half_move_clock == 50)
		return BoardState::Stalemate;

	if (m_moves.size() == 0) {

		if (m_in_check == true)
			return BoardState::Checkmate;
		else
			return BoardState::Stalemate;
	}

	return BoardState::Legal;
}

void Chess::Board::assign_piece_pins()
{
	//a pin denotes a piece whos directions of movement are restricted due to such a movement uncovering a check.
	for (const auto& piece : m_pieces) {

		if ((piece->get_faction() == m_enemy->get_faction())) {

			Pin pin{ piece->generate_pins(get_king_position(), *this) };

			//a non-zero pin-vector implies alignment was found and an enemy piece is infact pinned.
			if (pin.m_vector != 0) {

				const auto pinned_piece = find_piece_at(pin.m_position);
				assert((pinned_piece != m_pieces.end()) && "Pin position was not found in the piece list!");
				(*pinned_piece)->pin(pin.m_vector);
			}
		}
	}
}

void Chess::Board::generate_composite_movelist()
{
	if(m_moves.size()>0)
		m_moves.clear();

	//for every piece generate a set of all moves, movelist, and then append it to a vector of movelists.
	for (const auto& piece : m_pieces) {

		if (piece->get_faction() == m_player->get_faction()) {

			MoveList movelist = piece->generate_move_list(*this);

			if (movelist.size() > 0)
				m_moves.push_back(std::move(movelist));
		}
	}
}

bool Chess::Board::find_and_execute_move(const int from, const int to)
{
	//attempt to find the 'from' position as the movelist origin.
	auto movelist = std::find_if(m_moves.begin(), m_moves.end(), [from](const MoveList& movelist) -> bool { return movelist.get_origin() == from; });

	if (movelist != m_moves.end()) {

		//check if that movelist contains the 'to' position.
		auto move = movelist->find_in_list(to);
		if (move != movelist->end()) {

			//execute the command.
			execute_move_command(std::move(*move));
			return true;
		}
	}
	
	//return false if we could not execute the command.
	return false;
}

void Chess::Board::execute_move_command(std::unique_ptr<ChessCommandBase> command)
{
	command->execute(*this);

	if (m_board[command->get_to()] == TileType::Pawn) {

		m_half_move_clocks.push(m_half_move_clock);
		m_half_move_clock = 0;
	}
	else {

		++m_half_move_clock;
	}

	m_move_history.push_back(std::move(command));
}

bool Chess::Board::execute_undo_command()
{
	if (m_move_history.size() == 0) {

		std::cout << "Cannot undo move as no previous move exists!\n";
		return false;
	}

	// get last move from the move history stack.
	auto& command = m_move_history.back();

	if (m_board[command->get_to()] == TileType::Pawn) {

		int old_clock = m_half_move_clocks.top();

		m_half_move_clock = old_clock;
		m_half_move_clocks.pop();
	}
	else {

		--m_half_move_clock;
	}


	//undo it
	command->undo(*this);
	m_move_history.pop_back();

	return true;
}

void Chess::Board::move(const int from, const int to)
{
	//update the piece.
	auto piece = find_piece_at(from);
	(*piece)->move(to);

	//update the board
	m_board[to] = m_board[from];
	m_board[from].clear();
}

void Chess::Board::undo_move(const int from, const int to)
{
	//update the piece.
	auto piece = find_piece_at(from);
	(*piece)->undo_move(to);

	//update the board
	m_board[to] = m_board[from];
	m_board[from].clear();
}

void Chess::Board::revive()
{
	//the last-captured piece is at the back of the captured_piece list.
	m_pieces.push_back(std::move(m_captured_pieces.back()));
	m_captured_pieces.pop_back();

	int captured_position = m_pieces.back()->get_position();

	//reinstate the pieces internal board representation.
	m_board[captured_position] = std::move(m_captured_tiles.back());
	m_captured_tiles.pop_back();
}

void Chess::Board::capture(const int position)
{
	//move captured piece to capture list and remove its entry from piece list.
	auto captured = find_piece_at(position);

	assert(captured != m_pieces.end() && "Capture was not found in piecelist!");

	//hold onto the piece object as we may need it if the user undos a move, these objects hold 'meta-data' about the piece.
	m_captured_pieces.push_back(std::move(*captured));
	m_pieces.erase(captured);

	m_captured_tiles.push_back(m_board[position]);
	m_board[position].clear();
}

std::vector<std::unique_ptr<Chess::Piece>>::iterator Chess::Board::find_piece_at(const int position)
{
	return std::find_if(m_pieces.begin(), m_pieces.end(), [position](const std::unique_ptr<Piece>& piece) -> bool { return piece->get_position() == position; });
}

const int Chess::Board::get_king_position(const Player& player) const
{
	for (const auto& piece : m_pieces) {

		int piece_position{ piece->get_position() };

		if((m_board[piece_position] == TileType::King) && (piece->get_faction() == m_player->get_faction()))
			return (piece->get_position());
	}

	assert(false && "Could not find king position!");
	return 0;
}

const bool Chess::Board::is_position_captured(const int position)
{
	for (const auto& piece : m_pieces) {

		if (piece->get_faction() == m_enemy->get_faction()) {

			MoveList capture_list(piece->generate_capture_list(*this));

			//this only detects explicit capture commands, so won't tell you if any empty tile is attacked.
			for (const auto& capture : capture_list) {

				if (capture->get_to() == position)
					return true;
			}
		}
	}
	return false;
}

void Chess::Board::promote(char promotion)
{
	//load the promotion
	auto promotion_from_to = m_promotion_positions;

	//'capture' the pawn piece at 'from'
	capture(promotion_from_to.first);

	//make the new one at the second, the 'to' position.
	auto promotion_pair = m_piece_factory.create_tile_piece_pair(promotion, promotion_from_to.second);

	//move the new piece into piece_list
	m_pieces.push_back(std::move(promotion_pair.second));
	//move the new tile onto board.
	m_board[promotion_from_to.second] = promotion_pair.first;

	//add promotion 'to' position to the promotions stack
	m_promotions.push(promotion_from_to.second);

	//clear the promotion pair
	m_promotion_positions = std::pair<int, int>(0, 0);
}

void Chess::Board::undo_promote()
{
	//load last promotion
	auto promotion_to = m_promotions.top();

	//find the piece at the promotion position and delete it
	auto piece = find_piece_at(promotion_to);
	m_pieces.erase(piece);

	//clear its tile.
	m_board[promotion_to].clear();

	//restore pawn from dead-list.
	revive();

	//pop the promotion
	m_promotions.pop();
}

const std::unordered_map<std::string, int> Chess::Board::generate_conversion_table() const
{
	std::unordered_map<std::string, int> m_conversion_table;

	//don't worry about it, this chess 'board' is defined as being 8x8.
	std::vector<std::string> alphabet{ "a","b","c","d","e","f","g","h" };
	std::vector<int> position{ 8,7,6,5,4,3,2,1 };

	std::string chess_notation{};
	auto row_iterator = position.begin();
	auto column_iterator = alphabet.begin();

	for (size_t i = 0; i < m_board.size(); ++i) {

		//once we encounte the first non-border tile, we can begin assinging collumn indexes until encounter another
		if (m_board[i] != TileType::Border) {

			chess_notation = (*column_iterator + std::to_string(*row_iterator));
			m_conversion_table[chess_notation] = static_cast<int>(i);
			++column_iterator;
		}
		else {

			//if the alphabet iterator has reached the end then we know we can increment the row iterator
			if (column_iterator == alphabet.end())
				++row_iterator;

			column_iterator = alphabet.begin();
		}
	}
	return m_conversion_table;
}

const bool Chess::Board::is_board_valid() const
{
	int white_king_count{ 0 };
	int black_king_count{ 0 };

	for (const auto& tile : m_board) {

		if (tile == TileType::King) {

			if (tile == Faction::White)
				++white_king_count;
			else if (tile == Faction::Black)
				++black_king_count;
		}
	}

	return ((white_king_count == 1) && (black_king_count == 1));
}

void Chess::Board::pretty_print_board()
{
	//width of each 'square' is 8
	std::size_t horizontal_width = ((m_game_board_x * 8) + 1);
	std::string horizontal_border = std::string(horizontal_width, '#');

	//only want to print for iterators that are NOT border tiles.
	auto board_tile_iterator = m_board.begin();
	
	//want to highlight pieces with moves by ornamenting with [], need to be able to traverse the moves list in order to do this.
	std::sort(m_moves.begin(), m_moves.end(), [](const MoveList& movelist1, const MoveList& movelist2) {return(movelist1.get_origin() < movelist2.get_origin()); });
	auto movelist = m_moves.begin();

	std::cout << "       A       B       C       D       E       F       G       H\n";
	std::cout << "   " << horizontal_border << "\n";

	for (size_t i = 0; i < m_game_board_y; i++) {

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "       #";
		}
		std::cout << "\n";

		//print border token
		std::cout << " " << m_game_board_y - i << " #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			while (*board_tile_iterator == TileType::Border)
				++board_tile_iterator;

			std::cout << "  ";
			if (movelist != m_moves.end() && std::distance(m_board.begin(), board_tile_iterator) == movelist->get_origin()) {
				
				//surround pieces with a '[ ]' if they have moves.
				std::cout << "[" << *board_tile_iterator << "]";
				++movelist;
				++board_tile_iterator;
			}
			else {
				//pieces without moves are decorated with circular brackets.
				(*board_tile_iterator == m_player->get_faction()) ? std::cout << "(" << *board_tile_iterator << ")" : std::cout << " " << *board_tile_iterator << " ";
				++board_tile_iterator;
			}
			std::cout << "  #";

			if (j == m_game_board_x-1)
				std::cout << " " << (m_game_board_y - i);
		}
		std::cout << "\n";

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "       #";
		}
		std::cout << "\n";

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "########";
		}
		std::cout << "\n";
	}
	std::cout << "       A       B       C       D       E       F       G       H\n";
}
bool Chess::Board::find_and_execute_pretty_print(const int from)
{
	auto movelist = find_movelist(from);

	if (movelist == m_moves.end())
		return false;

	//movelist exists, so sort, we can advance the iterator until the next move position equals the next tile on the board.
	movelist->sort();

	//each box is 8 squares wide.
	std::size_t border_width{ 1 + (m_game_board_x * 8) };
	std::string horizontal_border = std::string(border_width, '#');

	//only want to print for iterators that are NOT border tiles.
	auto board_tile_iterator = m_board.begin();
	auto move_iterator = movelist->begin();

	std::cout << "       A       B       C       D       E       F       G       H\n";
	std::cout << "   " << horizontal_border << "\n";

	for (size_t i = 0; i < m_game_board_y; i++) {

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "       #";
		}
		std::cout << "\n";

		//print border token
		std::cout << " " << m_game_board_y - i << " #";
		for (size_t j = 0; j < m_game_board_x; j++)
		{
			//advance the iterator till we find a non-border tile.
			while (*board_tile_iterator == TileType::Border)
				++board_tile_iterator;

			std::cout << "  ";

			//if the tile is in the moves list print it as follows.
			if (move_iterator != movelist->end()) {

				//if the current tile position = the destination of a move iteator.
				if (std::distance(m_board.begin(), board_tile_iterator) == (*move_iterator)->get_to()) {

					(*board_tile_iterator != TileType::Empty) ? std::cout << "{" << *board_tile_iterator << "}" : std::cout << " " << "X" << " ";

					//advance the move iterator.
					++move_iterator;
					++board_tile_iterator;
				}
				else {

					//if we didn't find a match to the movelist we need to print anyway
					(*board_tile_iterator == m_player->get_faction()) ? std::cout << "[" << *board_tile_iterator << "]" : std::cout << " " << *board_tile_iterator << " ";
					++board_tile_iterator;
				}
			}
			else {

				//if we've reached the end of the move list we need to continue to print the rest of the board.
				(*board_tile_iterator == m_player->get_faction()) ? std::cout << "[" << *board_tile_iterator << "]" : std::cout << " " << *board_tile_iterator << " ";
				++board_tile_iterator;
			}

			std::cout << "  #";

			if (j == m_game_board_x - 1)
				std::cout << " " << (m_game_board_y - i);
		}
		std::cout << "\n";

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "       #";
		}
		std::cout << "\n";

		//print border token
		std::cout << "   #";
		for (size_t j = 0; j < m_game_board_x; j++) {

			std::cout << "########";
		}
		std::cout << "\n";
	}
	std::cout << "       A       B       C       D       E       F       G       H\n";

	return true;
}