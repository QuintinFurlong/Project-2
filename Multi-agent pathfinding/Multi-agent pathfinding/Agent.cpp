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
}

void Agent::setUp(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index)
{
	current = t_start;
	endGoal = t_end;
	path.clear();
	body.setPosition((t_start.x + 1) * BLOCK_SIZE, ((t_start.y + 1) * BLOCK_SIZE) + BLOCK_SIZE/4);
	goal.setPosition((t_end.x + 1) * BLOCK_SIZE, (t_end.y + 1) * BLOCK_SIZE);
	goal.move(BLOCK_SIZE/2, BLOCK_SIZE/4);
	currentDirection = Direction::stay;

	currentText.setFont(*t_font);
	currentText.setString(std::to_string(index));
	currentText.setPosition(body.getPosition());
	endText.setFont(*t_font);
	endText.setString(std::to_string(index));
	endText.setPosition(goal.getPosition());
}

void Agent::move(sf::Vector2i t_path)
{
	body.move(t_path.x * BLOCK_SIZE, t_path.y * BLOCK_SIZE);
	currentText.setPosition(body.getPosition());
}

void Agent::setPos()
{
	body.setPosition((current.x + 1) * BLOCK_SIZE, ((current.y + 1) * BLOCK_SIZE)+ BLOCK_SIZE/4);
	currentText.setPosition(body.getPosition());
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
