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
	//uses currentPattern to determine which algorithm to use
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
	case recordedPath:
		adjacentPathFunc();
		break;
	case optimal:
		optimalPathFunc();
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
			//true if x distance is greater then y distance
			if (abs(agentNumber[i].current.x - agentNumber[i].endGoal.x) > abs(agentNumber[i].current.y - agentNumber[i].endGoal.y))
			{
				simpleCheckHorz(i);
				if (agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckVert(i);
				}
			}
			//true if agent hasn't decided to move (if it can't/shouldn't move left or right)
			if (agentNumber[i].currentDirection == Direction::stay)
			{
				simpleCheckVert(i);
				if (agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckHorz(i);
				}
				//true if agent hasn't decided to move up or down 
				if (agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckVert(i);
					if (agentNumber[i].currentDirection == Direction::stay)
					{
						simpleCheckHorz(i);
					}
				}
			}
		}
		moveSingleAgent(i);
	}
}

//moves each agent separately, so other agents are marked as obstacles
void MultiAgentHandler::adjacentFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].current != agentNumber[i].endGoal && agentNumber[i].path.size() == 0)//true if not at goal
		{
			for (int width = 0; width < WORLD_WIDTH; width++)
			{
				for (int height = 0; height < WORLD_HEIGHT; height++)
				{
					worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = false;
					worldBlocks[width][height].disToGoal = 999;//sets distance to goal higher then can ever be
				}
			}
			worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = true;//current spot is free
			worldBlocks[agentNumber[i].endGoal.x][agentNumber[i].endGoal.y].disToGoal = 0;//set goal to zero to base the rest of spots off
			int currentDis = 0;//active distance we are dealing with
			currentDis = numberGrid(currentDis, i, agentNumber[i].blockers);

			worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = false;//the spot the agent is on is blocked
			//true if not on right side + possible path marked, else ifs used for the same in other directions
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

void MultiAgentHandler::adjacentPathFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].path.size() == 0)//true if agent doesn't have a path yet or finished
		{
			if (agentNumber[i].current == agentNumber[i].endGoal)//if agent has made it to goal
			{
 				worldBlocks[agentNumber[i].endGoal.x][agentNumber[i].endGoal.y].passable = false;//makes finished agents obstacles
			}
			else
			{
				findPath(i);//creates a path with a numberd grid for the passed in agent number
				for (int i2 = 0; i2 < i; i2++)//loops through all previous agents
				{
					int pathLength = 0;//keeps track which tick we are dealing with
					//true while neither agent has made it to their goals and aren't the same agent
					while (pathLength < agentNumber[i].path.size() && pathLength < agentNumber[i2].path.size() && i != i2)
					{
						if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength) ||//true if both agents will move into the same square
							(pathLength > 0 && agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength - 1)))//true if first agent plans to move into another agents square
						{
							Blocker temp;
							temp.pathDis = worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].disToGoal - pathLength-1;
							temp.block = agentNumber[i].path.at(pathLength);
							temp.next = false;
							temp.whichAgents = sf::Vector2i(i,i2);
							if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength))
							{
								temp.next = true;
							}
							agentNumber[i].blockers.push_back(temp);
							agentNumber[i].path.clear();
							i--;
							i2 = i;
							pathLength = 99;
						}
						pathLength++;
					}
				}
			}
		}
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].current != agentNumber[i].endGoal && agentNumber[i].path.size() != 0)
		{
			agentNumber[i].current = agentNumber[i].path.front();
			agentNumber[i].path.erase(agentNumber[i].path.begin());
			agentNumber[i].setPos();//sets agent off current
		}
	}
}

void MultiAgentHandler::optimalPathFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].path.size() == 0)
		{
			if (agentNumber[i].current == agentNumber[i].endGoal)//if agent has made it to goal
			{
				worldBlocks[agentNumber[i].endGoal.x][agentNumber[i].endGoal.y].passable = false;//makes finished agents obstacles
			}
			else
			{
				findPath(i);//creates a path with a numberd grid for the passed in agent number
			}
		}
	}
	redoPath();
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].current != agentNumber[i].endGoal && agentNumber[i].path.size() != 0)
		{
			agentNumber[i].current = agentNumber[i].path.front();
			agentNumber[i].path.erase(agentNumber[i].path.begin());
			agentNumber[i].setPos();//sets agent off current
		}
	}
}

void MultiAgentHandler::redoPath()
{
	bool anyBlockers = false;
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		for (int i2 = 0; i2 < MAX_AGENTS; i2++)
		{
			int pathLength = 0;//keeps track which tick we are dealing with
			//true while neither agent has made it to their goals and aren't the same agent
			while (pathLength < agentNumber[i].path.size() && pathLength < agentNumber[i2].path.size() && i != i2)
			{
				if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength) ||
					(pathLength > 0 && agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength - 1)))
				{
					Blocker temp;
					//temp.pathDis = agentNumber[i].path.size() - pathLength;
					temp.block = agentNumber[i].path.at(pathLength);
					//temp.next = false;
					temp.whichAgents = sf::Vector2i(i, i2);
					if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(pathLength))
					{
						temp.next = true;
					}
					agentNumber[i].blockers.push_back(temp);
					pathLength = 99;
					anyBlockers = true;
				}
				pathLength++;
			}
			while (pathLength < agentNumber[i].path.size() && i != i2 && agentNumber[i2].path.size()>0)
			{
				if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(agentNumber[i2].path.size()-1))
				{
					Blocker temp;
					temp.pathDis = agentNumber[i].path.size() - pathLength;
					temp.block = agentNumber[i].path.at(pathLength);
					temp.next = false;
					temp.whichAgents = sf::Vector2i(i, i2);
					if (agentNumber[i].path.at(pathLength) == agentNumber[i2].path.at(agentNumber[i2].path.size() - 1))
					{
						temp.next = true;
					}
					agentNumber[i].blockers.push_back(temp);
				}
				pathLength++;
			}
		}
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (agentNumber[i].blockers.size() > 0)
		{
			int shortestDistance = agentNumber[i].path.size();
			agentNumber[i].path.clear();
			findPath(i);
			if (shortestDistance == agentNumber[i].path.size())
			{
				for (auto blocker : agentNumber[i].blockers)
				{
					std::vector<Blocker> newVector;
					for (auto blocker2 : agentNumber[blocker.whichAgents.y].blockers)
					{
						if (blocker2.whichAgents.y != i)
						{
							newVector.push_back(blocker2);
						}
					}
					agentNumber[blocker.whichAgents.y].blockers.clear();
					agentNumber[blocker.whichAgents.y].blockers = newVector;
				}
				agentNumber[i].blockers.clear();
			}
			else
			{
				int i2 = agentNumber[i].blockers.at(0).whichAgents.y;
				int shortestDistance2 = agentNumber[i2].path.size();
				agentNumber[i2].path.clear();
				findPath(i2);
				if (shortestDistance + agentNumber[i2].path.size() <= agentNumber[i].path.size() + shortestDistance2)
				{
					agentNumber[i].blockers.clear();
					agentNumber[i].path.clear();
					findPath(i);
					std::vector<Blocker> newVector;
					for (auto blocker2 : agentNumber[i2].blockers)
					{
						if (blocker2.whichAgents.y != i)
						{
							newVector.push_back(blocker2);
						}
					}
					agentNumber[i2].blockers.clear();
					agentNumber[i2].blockers = newVector;
				}
				else
				{
					std::vector<Blocker> newVector;
					for (auto blocker2 : agentNumber[i2].blockers)
					{
						if (blocker2.whichAgents.y != i)
						{
							newVector.push_back(blocker2);
						}
					}
					agentNumber[i2].blockers.clear();
					agentNumber[i2].blockers = newVector;
					agentNumber[i2].path.clear();
					findPath(i2);
					agentNumber[i].blockers.erase(agentNumber[i].blockers.begin());
				}
			}
		}
	}
	if (anyBlockers)
	{
		redoPath();
	}
}

void MultiAgentHandler::findPath(int i)
{
	if (agentNumber[i].current != agentNumber[i].endGoal && agentNumber[i].path.size() == 0)//true if not at goal
	{
		for (int width = 0; width < WORLD_WIDTH; width++)
		{
			for (int height = 0; height < WORLD_HEIGHT; height++)
			{
				worldBlocks[width][height].disToGoal = 999;//sets distance to goal higher then can ever be
			}
		}
		worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].passable = true;//current spot is free
		worldBlocks[agentNumber[i].endGoal.x][agentNumber[i].endGoal.y].disToGoal = 0;//set goal to zero to base the rest of spots off
		int currentDis = 0;//active distance we are dealing with
		currentDis = numberGrid(currentDis, i,agentNumber[i].blockers);

		sf::Vector2i currentBlock = agentNumber[i].current;
		int distance = currentDis;
		sf::Vector2i goal = agentNumber[i].endGoal;
		float bestDis = 99;
		sf::Vector2i changeBlock = currentBlock;
		sf::Vector2i checkBlock = changeBlock;
		while (distance > 0)
		{
			currentBlock = changeBlock; 
			for (int sides = 0; sides < 4; sides++)
			{
				checkBlock = currentBlock;
				switch (sides)
				{
				case 0:
					checkBlock.y--;
					break;
				case 1:
					checkBlock.x--;
					break;
				case 2:
					checkBlock.x++;
					break;
				case 3:
					checkBlock.y++;
					break;
				}

				if (checkBlock.x > -1 && checkBlock.x < WORLD_WIDTH && checkBlock.y > -1 && checkBlock.y < WORLD_HEIGHT &&
					worldBlocks[checkBlock.x][checkBlock.y].disToGoal == distance - 1)
				{
					float currentDis = sqrt(pow((checkBlock.x - goal.x), 2) + pow((checkBlock.y - goal.y), 2));
					if (currentDis < bestDis || (currentDis == bestDis && rand()%2==0))
					{
						int index = worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].disToGoal - distance;
						if (agentNumber[i].path.size() <= index)
						{
							agentNumber[i].path.push_back(sf::Vector2i(checkBlock.x, checkBlock.y));
						}
						else
						{
							agentNumber[i].path.at(index) = sf::Vector2i(checkBlock.x, checkBlock.y);
						}
						bestDis = currentDis;
						changeBlock = sf::Vector2i(checkBlock.x, checkBlock.y);
					}
				}
			}
			bestDis = 99;
			distance--;
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
	worldBlocks[agentNumber[t_index].current.x][agentNumber[t_index].current.y].passable = true;//current spot is free
	switch (agentNumber[t_index].currentDirection)//moves agent based off decided direction
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
	worldBlocks[agentNumber[t_index].current.x][agentNumber[t_index].current.y].passable = false;//new current spot is blocked
	agentNumber[t_index].setPos();//sets agent off current
}

void MultiAgentHandler::setUpAgent(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index)
{
	agentNumber[index].setUp(t_start, t_end, t_font, index);
}

int MultiAgentHandler::numberGrid(int currentDis, int i, std::vector<Blocker> t_blockers)
{
	while (currentDis < worldBlocks[agentNumber[i].current.x][agentNumber[i].current.y].disToGoal)//true if start is larger then current distance
	{
		for (auto eachBlock : agentNumber[i].blockers)
		{
			if (currentPattern != numberAdjacent)
			{
				worldBlocks[eachBlock.block.x][eachBlock.block.y].passable = false;
			}
		}
		for (int width = 0; width < WORLD_WIDTH; width++)
		{
			for (int height = 0; height < WORLD_HEIGHT; height++)
			{
				if (worldBlocks[width][height].disToGoal == currentDis)//true for spots at the current distance from goal
				{
					if (width != WORLD_WIDTH - 1 && worldBlocks[width + 1][height].disToGoal > currentDis&& //true if not on the right edge + true if right block is marked further then current
						worldBlocks[width + 1][height].passable)//true if right isn't blocked
					{
						worldBlocks[width + 1][height].disToGoal = currentDis + 1;//sets right to 1 longer then active
					}
					if (width != 0 && worldBlocks[width - 1][height].disToGoal > currentDis&&//same as before but for left side
						worldBlocks[width - 1][height].passable)
					{
						worldBlocks[width - 1][height].disToGoal = currentDis + 1;
					}
					if (height != WORLD_HEIGHT - 1 && worldBlocks[width][height + 1].disToGoal > currentDis&&//same as before but for the bottom
						worldBlocks[width][height + 1].passable)
					{
						worldBlocks[width][height + 1].disToGoal = currentDis + 1;
					}
					if (height != 0 && worldBlocks[width][height - 1].disToGoal > currentDis&&//same as before but for the top
						worldBlocks[width][height - 1].passable)
					{
						worldBlocks[width][height - 1].disToGoal = currentDis + 1;
					}
				}
			}
		}
		for (auto eachBlock : agentNumber[i].blockers)
		{
			if (currentPattern != numberAdjacent)/*(eachBlock.next && currentDis == eachBlock.pathDis-2)
				|| (eachBlock.next && currentDis == eachBlock.pathDis - 1))*/
			{
				worldBlocks[eachBlock.block.x][eachBlock.block.y].passable = true;
			}
		}
		currentDis++;//expand on the spots that have just been expanded
	}
	return currentDis;
}

bool MultiAgentHandler::allAtGoal()
{
	bool allHome = true;
	for (int i = 0; i < MAX_AGENTS&& allHome; i++)
	{
		if (!agentNumber[i].atGoal())
		{
			allHome = false;
		}
	}
	return allHome;
}

void MultiAgentHandler::changePath(bool t_increase)
{
	if (t_increase)
	{
		switch (currentPattern)
		{
		case straightForward:
			currentPattern = stairs;
			break;
		case stairs:
			currentPattern = numberAdjacent;
			break;
		case numberAdjacent:
			currentPattern = recordedPath;
			break;
		case recordedPath:
			currentPattern = optimal;
			break;
		case optimal:
			currentPattern = straightForward;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (currentPattern)
		{
		case straightForward:
			currentPattern = optimal;
			break;
		case stairs:
			currentPattern = straightForward;
			break;
		case numberAdjacent:
			currentPattern = stairs;
			break;
		case recordedPath:
			currentPattern = numberAdjacent;
			break;
		case optimal:
			currentPattern = recordedPath;
			break;
		default:
			break;
		}
	}
}

std::string MultiAgentHandler::pathName()
{
	switch (currentPattern)
	{
	case straightForward:
		return "Straight Forward";
		break;
	case stairs:
		return "Stairs";
		break;
	case numberAdjacent:
		return "Number Adjacent";
		break;
	case recordedPath:
		return "Recorded Path";
		break;
	case optimal:
		return "Optimal Path";
		break;
	default:
		break;
	}
	return std::string();
}

float MultiAgentHandler::averagePathSize()
{
	float average = 0.0f;
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		average += agentNumber[i].pathSize;
	}
	return average/ MAX_AGENTS;
}
