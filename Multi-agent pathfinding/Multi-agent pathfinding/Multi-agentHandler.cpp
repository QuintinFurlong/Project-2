#include "Multi-agentHandler.h"

MultiAgentHandler::MultiAgentHandler()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agentNumber[i].currentDirection = Direction::stay;
	}
}

void MultiAgentHandler::setPattern(MovePatterns t_move)
{
	currentPattern = t_move;
}

void MultiAgentHandler::moveAgents()
{
	switch (currentPattern)
	{
	case straightForward:
		simpleMovement();
		break;
	case two:
		break;
	case three:
		break;
	default:
		break;
	}
}

void MultiAgentHandler::draw(sf::RenderWindow& t_window)
{
	for (int width = 0; width < WORLD_WIDTH; width++)
	{
		for (int height = 0; height < WORLD_HEIGHT; height++)
		{
			if(worldBlocks[width][height].passable)
				worldBlocks[width][height].square.setFillColor(sf::Color::Green);
			else
				worldBlocks[width][height].square.setFillColor(sf::Color(0,100,0));
			t_window.draw(worldBlocks[width][height].square);
		}
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = false;
		agentNumber[i].draw(t_window);
	}
}

void MultiAgentHandler::simpleMovement()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agentNumber[i].currentDirection = Direction::stay;
		if (agentNumber[i].current != agentNumber[i].endGoal)//true if not at goal
		{
			if (agentNumber[i].current.x < agentNumber[i].endGoal.x &&//true if wants to move right
				worldBlocks[agentNumber[i].current.x + 1][agentNumber[i].current.y].passable)//true if can
			{
				agentNumber[i].currentDirection = Direction::right;
			}
			else if (agentNumber[i].current.x > agentNumber[i].endGoal.x &&//true if wants to move left
				worldBlocks[agentNumber[i].current.x - 1][agentNumber[i].current.y].passable)//true if can
			{
				agentNumber[i].currentDirection = Direction::left;
			}
			if (agentNumber[i].currentDirection == Direction::stay)
			{
				if (agentNumber[i].current.y < agentNumber[i].endGoal.y &&//true if wants to move down
					worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y + 1].passable)//true if can
				{
					agentNumber[i].currentDirection = Direction::down;
				}
				else if (agentNumber[i].current.y > agentNumber[i].endGoal.y&&//true if wants to move down
					worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y - 1].passable)//true if can
				{
					agentNumber[i].currentDirection = Direction::up;
				}
			}
		}
		moveSingleAgent(i);
	}
}

void MultiAgentHandler::moveSingleAgent(int t_index)
{
	worldBlocks[agentNumber[t_index].current.x][agentNumber[t_index].current.y].passable = true;
	switch (agentNumber[t_index].currentDirection)
	{
	case right:
		agentNumber[t_index].current.x++;
		break;
	case left:
		agentNumber[t_index].current.x--;
		break;
	case up:
		agentNumber[t_index].current.y--;
		break;
	case down:
		agentNumber[t_index].current.y++;
		break;
	case stay:
		break;
	default:
		break;
	}
	worldBlocks[agentNumber[t_index].current.x][agentNumber[t_index].current.y].passable = false;
	agentNumber[t_index].setPos();
}
