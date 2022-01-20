#include "Player.h"

#include <iostream>

void Chess::Player::add_castle_permission(const int vector, const bool permission)
{
	m_castle_permissions.push_back(std::pair<int, bool>(vector, permission));
}
 
const bool Chess::Player::can_castle_in_vector(const int vector) const
{
	for (const auto& permission : m_castle_permissions) {

		if (permission.first == vector)
			return permission.second;
	}

	return false;
}
