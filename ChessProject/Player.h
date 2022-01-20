/*
Date: 03/05/2020 (refactor)
Author: Christopher Ryder
Email: Christopher.Ryder-2@student.manchester.ac.uk

Remit:
This class serves to represent the 'player' of the game and any 'personalised' properties, i.e castling permissions. This class was originally planned to be 
more useful. A 'turn timer' functionality would have represented the amount of time a player had left to make a move. Unfortunately not enough time!
*/

#ifndef __PLAYER_HEADER
#define __PLAYER_HEADER

#include <vector>
#include <string>

//for Faction
#include "Tile.h"

namespace Chess {

	class Player
	{
	public:

		Player(const std::string& name, Faction faction) : m_name(name), m_faction(faction) {}
		~Player() {}

		void add_castle_permission(const int vector, const bool permission);
		const bool can_castle_in_vector(const int vector) const;

		//harmlessly expose this for printing
		inline const std::string get_name() const { return m_name; }

		//need to expose this for determining if an empty position would be attacked.
		inline const Faction get_faction() const { return m_faction; }

	private:

		std::string m_name;
		Faction m_faction;

		std::vector<std::pair<int, bool>> m_castle_permissions;
	};
}
#endif