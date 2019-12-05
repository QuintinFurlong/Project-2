#pragma once
#include <SFML/Graphics.hpp>

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

	Agent();
	void setUp(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index);
	void move(sf::Vector2i t_path);
	void setPos();
	void draw(sf::RenderWindow& t_window);
};