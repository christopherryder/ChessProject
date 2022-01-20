/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
Represent a bare-bones command object for use in the command-pattern design pattern.
*/

#ifndef __COMMAND_HEADER
#define __COMMAND_HEADER

template<class Context>
class CommandInterface
{
public:
	virtual ~CommandInterface() = 0;

	//define methods for how a 'command' will be done and undone.
	virtual void execute(Context& board) = 0;
	virtual void undo(Context& board) = 0;
};
template<class Context>
inline CommandInterface<Context>::~CommandInterface() = default;

#endif