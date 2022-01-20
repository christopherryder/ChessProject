/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to keep an 'internal board representation' of the chess game, and to determine the validity of the board and any moves.
*/

#ifndef __BOARD_HEADER
#define __BOARD_HEADER

#include "BoardBase.h"

#include <iostream>
#include <iomanip>

#include <unordered_map>
#include <vector>
#include <stack>

#include "MoveCommand.h"
#include "MoveList.h"
#include "Player.h"
#include "Tile.h"
#include "PieceFactory.h"

#include "FEN.h"

namespace Chess {

	class Piece;

	enum class BoardState
	{
		Legal,
		Checkmate,
		Stalemate
	};

	//Define the size of the 'internal' board.
	class Board : public BoardBase<10, 12, Chess::Tile>
	{
	public:

		//A board can only be made from a valid FEN string.
		Board(const FEN& fen_notation);
		Board(Board&& board) noexcept;
		Board& operator=(Board&& board) noexcept;

		//board contains a list of move commands (unique_ptrs) hence delete copy operations.
		Board(const Board& board) = delete;
		Board& operator=(const Board& board) = delete;
		
		~Board();

		//---
		const std::string build_board_representation(const std::string board_representation) const;
		void make_board_from_representation(const std::string board_representation);
		const std::pair<bool, int> convert_coordinate_to_position(std::string coordinate) const;

		//Methods for generating piece moves.
		void generate_composite_movelist();
		void assign_piece_pins();

		//the concept of turns is fulfilled using this:
		void rotate_players() { std::swap(m_player, m_enemy); }
		const Player& get_player() const { return *m_player; }
		const Player& get_enemy() const { return *m_enemy; }

		std::vector<MoveList>::iterator find_movelist(const int origin);
		std::vector<std::unique_ptr<Piece>>::iterator find_piece_at(const int position);
		bool find_and_execute_move(const int from, const int to);

		//print methods:
		bool find_and_execute_pretty_print(const int from);
		void pretty_print_board();
		void print_move_list();

		//methods related to game legality:
		const Faction get_player_faction() const { return m_player->get_faction(); }
		const BoardState state() const;

		//Need to expose the half move clock as if it ticks to 50 the game is over (stalemate)
		const int get_half_move_clock() const { return m_half_move_clock; }
		
		void determine_current_check_state() { m_in_check = is_in_check(); }
		const bool get_current_check_state() { return m_in_check; }
		
		inline const int get_king_position() const { return get_king_position(*m_player); }
		const int get_king_position(const Player& player) const;

		const bool is_in_check() { return is_position_captured(get_king_position()); }
		const bool is_position_captured(const int position);
		
		//Methods related to Command Pattern:
		void move(const int from, const int to);
		void undo_move(const int from, const int to);
		//restore the piece at the top of the dead pieces stack.
		void revive();
		void capture(const int position);

		void execute_move_command(std::unique_ptr<ChessCommandBase> command);
		bool execute_undo_command();

		//for the CastleCommand
		inline const bool can_castle_in_vector(int vector) const { return m_player->can_castle_in_vector(vector); }
		
		//for the EnpassantCommand
		void update_enpassant_position(const int position) { m_enpassant_position = position; }
		inline const bool can_capture_enpassant(const int position) const { return (position == m_enpassant_position); }

		//for the PromotionCommand
		void flag_promotion(const std::pair<int, int> promotion_from_to) { m_promotion_positions = promotion_from_to; }
		inline const bool is_promotion() const { return ((m_promotion_positions.first != 0) && m_promotion_positions.second != 0); }
		void promote(char promotion);
		void undo_promote();

		//define a method to be able to easily convert from chess notation to position coordinate
		const std::unordered_map<std::string, int> generate_conversion_table() const;

		//constant, read only access to board tiles
		virtual const Tile& operator[](const int position) const override { return m_board[position]; }

	protected:

		//each player must have a king.
		virtual const bool is_board_valid() const override;

	private:

		//standard chess is played by two players.
		std::shared_ptr<Player> m_player;
		std::shared_ptr<Player> m_enemy;

		//is the current 'board-state' in check
		bool m_in_check;

		//location of a possible enpassant piece, dictated by FEN notation
		int m_enpassant_position;

		//location of the piece to be promoted and the tile it promotes on.
		std::pair<int, int> m_promotion_positions{ std::pair<int,int>(0,0) };
		std::stack<int, std::vector<int>> m_promotions;

		//number of 'half-moves' used for the 50-turn draw rule.
		int m_half_move_clock{ 0 };
		std::stack<int, std::vector<int>> m_half_move_clocks;

		//total number of moves made in the game.
		int m_full_move_clock{ 1 };

		//this differs from m_board_x as this is the size of the /VISIBLE/ game board, not including borders.
		std::size_t m_game_board_x{ 8 };
		std::size_t m_game_board_y{ 8 };

		std::vector<Tile> m_board;
		std::vector<Tile> m_captured_tiles;

		//factory to refactor some of the 'generate piece' clutter.
		PieceFactory m_piece_factory;
		std::vector<std::unique_ptr<Piece>> m_pieces;
		std::vector<std::unique_ptr<Piece>> m_captured_pieces;

		//each piece generates a movelist of possible moves.
		std::vector<MoveList> m_moves;
		//after execution, store each move command here. This allows for easy undoing thanks to command-pattern!
		std::vector<std::unique_ptr<ChessCommandBase>> m_move_history;

		//a conversion table for converting between algebraic notation, (a1,a2...) to a vector index in the board.
		std::unordered_map<std::string, int> m_conversion_table;
	};
}

#endif