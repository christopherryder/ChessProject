#include "FEN.h"

FEN::FEN(const std::string game_configuration) : m_game_configuration(game_configuration)
{
	if (!find_matching_expression())
	{
		throw std::logic_error("Invalid FEN expression! Cannot make game from an invalid FEN expression.");
	}
}

bool FEN::find_matching_expression()
{
	//attempt to find the matches using the regex, if we found a match it and any sub matches are stored in m_matches.
	if (std::regex_search(m_game_configuration, m_matches, m_forsyth_edwards))
	{
		return true;
	}

	return false;
}
