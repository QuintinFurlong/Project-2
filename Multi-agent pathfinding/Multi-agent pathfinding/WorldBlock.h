#pragma once
#include <SFML/Graphics.hpp>
class WorldBlock
{
public:
	sf::RectangleShape square;
	bool passable;
	int disToGoal;
};
