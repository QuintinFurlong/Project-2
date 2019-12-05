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
		straightForwFunc();
		break;
	case stairs:
		stairsFunc();
		break;
	case numberAdjacent:
		adjacentFunc();
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

void MultiAgentHandler::straightForwFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agentNumber[i].currentDirection = Direction::stay;
		if (agentNumber[i].current != agentNumber[i].endGoal)//true if not at goal
		{
			simpleCheckHorz(i);
			if (agentNumber[i].currentDirection == Direction::stay)
			{
				simpleCheckVert(i);
			}
		}
		moveSingleAgent(i);
	}
}

void MultiAgentHandler::stairsFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		agentNumber[i].currentDirection = Direction::stay;
		if (agentNumber[i].current != agentNumber[i].endGoal)//true if not at goal
		{
			if (abs(agentNumber[i].current.x - agentNumber[i].endGoal.x) > abs(agentNumber[i].current.y - agentNumber[i].endGoal.y))
			{
				simpleCheckHorz(i);
				if (agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckVert(i);
				}
			}
			else
			{
				simpleCheckVert(i);
				if (agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckHorz(i);
				}
			}
		}
		moveSingleAgent(i);
	}
}

void MultiAgentHandler::adjacentFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].current != agentNumber[i].endGoal)//true if not at goal
		{
			for (int width = 0; width < WORLD_WIDTH; width++)
			{
				for (int height = 0; height < WORLD_HEIGHT; height++)
				{
					worldBlocks[width][height].disToGoal = 999;
				}
			}
			worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = true;
			worldBlocks[agentNumber[i].endGoal.x][agentNumber[i].endGoal.y].disToGoal = 0;
			int currentDis = 0;
			while (currentDis < worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].disToGoal)
			{
				for (int width = 0; width < WORLD_WIDTH; width++)
				{
					for (int height = 0; height < WORLD_HEIGHT; height++)
					{
						if (worldBlocks[width][height].disToGoal == currentDis)
						{
							if (width != WORLD_WIDTH - 1 && worldBlocks[width + 1][height].disToGoal > currentDis && 
								worldBlocks[width + 1][height].passable)
							{
								worldBlocks[width + 1][height].disToGoal = currentDis + 1;
							}
							if (width != 0 && worldBlocks[width - 1][height].disToGoal > currentDis&&
								worldBlocks[width - 1][height].passable)
							{
								worldBlocks[width - 1][height].disToGoal = currentDis + 1;
							}
							if (height != WORLD_HEIGHT - 1 && worldBlocks[width][height + 1].disToGoal > currentDis&&
								worldBlocks[width][height + 1].passable)
							{
								worldBlocks[width][height + 1].disToGoal = currentDis + 1;
							}
							if (height != 0 && worldBlocks[width][height - 1].disToGoal > currentDis&&
								worldBlocks[width][height - 1].passable)
							{
								worldBlocks[width][height - 1].disToGoal = currentDis + 1;
							}
						}
					}
				}
				currentDis++;
			}
			worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = false;
			if (agentNumber[i].current.x != WORLD_WIDTH - 1 && worldBlocks[agentNumber[i].current.x + 1][agentNumber[i].current.y].disToGoal < 999)
			{
				agentNumber[i].currentDirection = Direction::right;
			}
			else if (agentNumber[i].current.x != 0 && worldBlocks[agentNumber[i].current.x - 1][agentNumber[i].current.y].disToGoal < 999)
			{
				agentNumber[i].currentDirection = Direction::left;
			}
			else if (agentNumber[i].current.y != WORLD_HEIGHT - 1 && worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y + 1].disToGoal < 999)
			{
				agentNumber[i].currentDirection = Direction::down;
			}
			else if (agentNumber[i].current.x != 0 && worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y - 1].disToGoal < 999)
			{
				agentNumber[i].currentDirection = Direction::up;
			}
			moveSingleAgent(i);
		}
	}
}

void MultiAgentHandler::simpleCheckVert(int i)
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

void MultiAgentHandler::simpleCheckHorz(int i)
{
	if (agentNumber[i].current.x < agentNumber[i].endGoal.x &&//true if wants to move right
		worldBlocks[agentNumber[i].current.x + 1][agentNumber[i].current.y].passable)//true if can
	{
		agentNumber[i].currentDirection = Direction::right;
	}
	else if (agentNumber[i].current.x > agentNumber[i].endGoal.x&&//true if wants to move left
		worldBlocks[agentNumber[i].current.x - 1][agentNumber[i].current.y].passable)//true if can
	{
		agentNumber[i].currentDirection = Direction::left;
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
