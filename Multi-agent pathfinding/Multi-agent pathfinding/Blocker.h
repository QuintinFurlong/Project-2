#pragma once
#include <SFML/Graphics.hpp>
class Blocker
{
public:
	int pathDis;
	sf::Vector2i block, whichAgents;
	bool next;//true if two agents plan to move into the same block
};