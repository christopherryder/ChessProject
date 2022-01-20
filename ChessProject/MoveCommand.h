/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to implement the 'command pattern' design pattern in the context of chess peices moving on a given board (the context)
*/

#ifndef __MOVECOMMAND_HEADER
#define __MOVECOMMAND_HEADER

#include "Command.h"

namespace Chess
{
	//inherently linked to the board class, so can't include board directly in this header.
	class Board;

	class ChessCommandBase : public CommandInterface<Board>
	{
	public:
		ChessCommandBase(const int from, const int to) : m_from(from), m_to(to) {}
		virtual ~ChessCommandBase() = 0;

		//chess commands need to be validated by the board
		virtual bool validate(Board& board) = 0;

		//need to expose for sorting/find_if.
		inline const int get_to() const { return m_to; }

	protected:
		int m_to;
		int m_from;
	};
	inline ChessCommandBase::~ChessCommandBase() = default;

	class MoveCommand : public ChessCommandBase
	{
	public:

		MoveCommand(const int from, const int to) : ChessCommandBase(from, to) {}
		virtual ~MoveCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;
		virtual bool validate(Board& board) override;
	};

	class CaptureCommand : public ChessCommandBase
	{
	public:

		CaptureCommand(const int from, const int to) : ChessCommandBase(from, to) {}
		virtual ~CaptureCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;
		virtual bool validate(Board& board) override;
	};

	class KingMoveCommand : public MoveCommand
	{
	public:

		KingMoveCommand(const int from, const int to) : MoveCommand(from,to) {}
		virtual ~KingMoveCommand() {}

		//needs to validate every time.
		virtual bool validate(Board& board) override;
	};

	class KingCaptureCommand : public CaptureCommand
	{
	public:

		KingCaptureCommand(const int from, const int to) : CaptureCommand(from, to) {}
		virtual ~KingCaptureCommand() {}

		//needs to validate every time.
		virtual bool validate(Board& board) override;
	};

	class CastleCommand : public MoveCommand
	{
	public:

		CastleCommand(const int from, const int to, const int castle_from, const int castle_to, const int vector) : MoveCommand(from, to), m_castle_from(castle_from), m_castle_to(castle_to), m_vector(vector) {}
		virtual ~CastleCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;

		//requires unique_validation.
		virtual bool validate(Board& board) override;

	private:

		int m_castle_from;
		int m_castle_to;

		//need to hold the vector of the castle to compare against the FEN flag.
		int m_vector;
	};

	//double pushes are when pawns can initially move two tiles.
	class DoublePushCommand : public MoveCommand
	{
	public:
		DoublePushCommand(const int from, const int to) : MoveCommand(from, to) {}
		virtual ~DoublePushCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;

		//inherits the Move validate command.
	};

	//enpassant captures of pawns having immediately moved two-tiles via pawn.
	class EnpassantCommand : public CaptureCommand
	{
	public:

		EnpassantCommand(const int from, const int to, const int capture) : CaptureCommand(from, to), m_capture(capture) {}
		virtual ~EnpassantCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;

		//needs to validate every time.
		virtual bool validate(Board& board) override;

	private:
		int m_capture;
	};

	//a pawn reaching the end of the board can upgrade to another piece.
	class PromotionCommand : public MoveCommand
	{
	public:

		PromotionCommand(const int from, const int to) : MoveCommand(from, to) {}
		virtual ~PromotionCommand() {}

		virtual void execute(Board& board) override;
		virtual void undo(Board& board) override;

		//special case of validate, we just need to check if the presence of the pawn on the back row blocks a check.
		virtual bool validate(Board& board) override;

	protected:

		//the co-validation method in 'board'.


	};
}

#endif