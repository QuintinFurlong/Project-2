#include "Agent.h"

Agent::Agent()
{
	body.setFillColor(sf::Color::Red);
	body.setSize(sf::Vector2f(BLOCK_SIZE/2, BLOCK_SIZE/2));
	goal.setFillColor(sf::Color(80,80,255));
	goal.setSize(sf::Vector2f(BLOCK_SIZE/2, BLOCK_SIZE/2));

	currentText.setCharacterSize(20);
	currentText.setFillColor(sf::Color::Black);
	endText.setCharacterSize(20);
	endText.setFillColor(sf::Color::Black);
	index = -1;
}

void Agent::setUp(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int t_index)
{
	current = t_start;
	endGoal = t_end;
	path.clear();
	body.setPosition((t_start.x + 1) * BLOCK_SIZE, ((t_start.y + 1) * BLOCK_SIZE) + BLOCK_SIZE/4);
	goal.setPosition((t_end.x + 1) * BLOCK_SIZE, (t_end.y + 1) * BLOCK_SIZE);
	goal.move(BLOCK_SIZE/2, BLOCK_SIZE/4);
	currentDirection = Direction::stay;

	currentText.setFont(*t_font);
	currentText.setString(std::to_string(t_index));
	currentText.setPosition(body.getPosition());
	endText.setFont(*t_font);
	endText.setString(std::to_string(t_index));
	endText.setPosition(goal.getPosition());
	index = t_index;
	pathSize = 0;
}

void Agent::setPos()
{
	body.setPosition((current.x + 1) * BLOCK_SIZE, ((current.y + 1) * BLOCK_SIZE)+ BLOCK_SIZE/4);
	currentText.setPosition(body.getPosition()); 
	pathSize++;
	endText.setString(std::to_string(index) + " : " + std::to_string(pathSize));
}

void Agent::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
	t_window.draw(goal);
	t_window.draw(currentText);
	t_window.draw(endText);
}

bool Agent::atGoal()
{	
	return current == endGoal;
}
