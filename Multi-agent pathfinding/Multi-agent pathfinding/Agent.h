#pragma once
#include <SFML/Graphics.hpp>
#include "Blocker.h"
enum Direction
{
	right, left, up, down, stay
};

class Agent
{
public:
	sf::RectangleShape body, goal;
	sf::Vector2i current, endGoal, nextMove;
	sf::Text currentText, endText;
	Direction currentDirection;
	std::vector<sf::Vector2i> path;
	std::vector<Blocker> blockers;
	int index, pathSize;

	Agent();
	void setUp(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index);
	void setPos();
	void draw(sf::RenderWindow& t_window);
	bool atGoal();
	static const int BLOCK_SIZE = 50;
};