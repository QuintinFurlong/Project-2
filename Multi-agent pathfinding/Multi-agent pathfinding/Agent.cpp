#include "Agent.h"

Agent::Agent()
{
	body.setFillColor(sf::Color::Red);
	body.setSize(sf::Vector2f(50, 50));
	goal.setFillColor(sf::Color(80,80,255));
	goal.setSize(sf::Vector2f(50, 50));

	currentText.setCharacterSize(35);
	currentText.setFillColor(sf::Color::Black);
	endText.setCharacterSize(35);
	endText.setFillColor(sf::Color::Black);
}

void Agent::setUp(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index)
{
	current = t_start;
	endGoal = t_end;
	body.setPosition((t_start.x + 1) * 100, ((t_start.y + 1) * 100) + 25);
	goal.setPosition((t_end.x + 1) * 100, (t_end.y + 1) * 100);
	goal.move(50, 25);

	currentText.setFont(*t_font);
	currentText.setString(std::to_string(index));
	currentText.setPosition(body.getPosition());
	endText.setFont(*t_font);
	endText.setString(std::to_string(index));
	endText.setPosition(goal.getPosition());
}

void Agent::move(sf::Vector2i t_path)
{
	body.move(t_path.x * 100, t_path.y * 100);
	currentText.setPosition(body.getPosition());
}

void Agent::setPos()
{
	body.setPosition((current.x + 1) * 100, ((current.y + 1) * 100)+25);
	currentText.setPosition(body.getPosition());
}

void Agent::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
	t_window.draw(goal);
	t_window.draw(currentText);
	t_window.draw(endText);
}
