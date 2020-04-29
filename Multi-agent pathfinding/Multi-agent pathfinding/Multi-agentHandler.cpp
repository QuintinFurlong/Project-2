#include "Multi-agentHandler.h"

MultiAgentHandler::MultiAgentHandler()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		m_agentNumber[i].currentDirection = Direction::stay;
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
		m_agentNumber[i].draw(t_window);
	}
}

void MultiAgentHandler::straightForwFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		m_agentNumber[i].currentDirection = Direction::stay;
		if (m_agentNumber[i].current != m_agentNumber[i].endGoal)//true if not at goal
		{
			simpleCheckHorz(i);
			if (m_agentNumber[i].currentDirection == Direction::stay)
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
		m_agentNumber[i].currentDirection = Direction::stay;
		if (m_agentNumber[i].current != m_agentNumber[i].endGoal)//true if not at goal
		{
			//true if x distance is greater then y distance
			if (abs(m_agentNumber[i].current.x - m_agentNumber[i].endGoal.x) > abs(m_agentNumber[i].current.y - m_agentNumber[i].endGoal.y))
			{
				simpleCheckHorz(i);
				if (m_agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckVert(i);
				}
			}
			//true if agent hasn't decided to move (if it can't/shouldn't move left or right)
			if (m_agentNumber[i].currentDirection == Direction::stay)
			{
				simpleCheckVert(i);
				if (m_agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckHorz(i);
				}
				//true if agent hasn't decided to move up or down 
				if (m_agentNumber[i].currentDirection == Direction::stay)
				{
					simpleCheckVert(i);
					if (m_agentNumber[i].currentDirection == Direction::stay)
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
		if (m_agentNumber[i].current != m_agentNumber[i].endGoal && m_agentNumber[i].path.size() == 0)//true if not at goal
		{
			for (int width = 0; width < WORLD_WIDTH; width++)
			{
				for (int height = 0; height < WORLD_HEIGHT; height++)
				{
					worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].passable = false;
					worldBlocks[width][height].disToGoal = 999;//sets distance to goal higher then can ever be
				}
			}
			worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].passable = true;//current spot is free
			worldBlocks[m_agentNumber[i].endGoal.x][m_agentNumber[i].endGoal.y].disToGoal = 0;//set goal to zero to base the rest of spots off
			int currentDis = 0;//active distance we are dealing with
			currentDis = numberGrid(currentDis, i, m_agentNumber[i].blockers);

			worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].passable = false;//the spot the agent is on is blocked
			//true if not on right side + possible path marked, else ifs used for the same in other directions
			if (m_agentNumber[i].current.x != WORLD_WIDTH - 1 && worldBlocks[m_agentNumber[i].current.x + 1][m_agentNumber[i].current.y].disToGoal < 999)
			{
				m_agentNumber[i].currentDirection = Direction::right;
			}
			else if (m_agentNumber[i].current.x != 0 && worldBlocks[m_agentNumber[i].current.x - 1][m_agentNumber[i].current.y].disToGoal < 999)
			{
				m_agentNumber[i].currentDirection = Direction::left;
			}
			else if (m_agentNumber[i].current.y != WORLD_HEIGHT - 1 && worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y + 1].disToGoal < 999)
			{
				m_agentNumber[i].currentDirection = Direction::down;
			}
			else if (m_agentNumber[i].current.x != 0 && worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y - 1].disToGoal < 999)
			{
				m_agentNumber[i].currentDirection = Direction::up;
			}
			moveSingleAgent(i);
		}
	}
}

void MultiAgentHandler::adjacentPathFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (m_agentNumber[i].path.size() == 0)//true if agent doesn't have a path yet or finished
		{
			if (m_agentNumber[i].current == m_agentNumber[i].endGoal)//if agent has made it to goal
			{
 				worldBlocks[m_agentNumber[i].endGoal.x][m_agentNumber[i].endGoal.y].passable = false;//makes finished agents obstacles
			}
			else
			{
				findPath(i);//creates a path with a numberd grid for the passed in agent number
			}
		}
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (m_agentNumber[i].current != m_agentNumber[i].endGoal && m_agentNumber[i].path.size() != 0)
		{
			m_agentNumber[i].current = m_agentNumber[i].path.front();
			m_agentNumber[i].path.erase(m_agentNumber[i].path.begin());
			m_agentNumber[i].setPos();//sets agent off current
		}
	}
}

void MultiAgentHandler::optimalPathFunc()
{
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (m_agentNumber[i].path.size() == 0)
		{
			if (m_agentNumber[i].current == m_agentNumber[i].endGoal)//if agent has made it to goal
			{
				worldBlocks[m_agentNumber[i].endGoal.x][m_agentNumber[i].endGoal.y].passable = false;//makes finished agents obstacles
			}
			else
			{
				findPath(i);//creates a path with a numberd grid for the passed in agent number
				if (i == MAX_AGENTS-1)//has it done once per run after all agents have a desired path
				{
					redoPath();
				}
			}
		}
	}
	//moves agents based off their paths
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (m_agentNumber[i].current != m_agentNumber[i].endGoal && m_agentNumber[i].path.size() != 0)
		{
			m_agentNumber[i].current = m_agentNumber[i].path.front();
			m_agentNumber[i].path.erase(m_agentNumber[i].path.begin());
			m_agentNumber[i].setPos();//sets agent off current
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
			while (pathLength < m_agentNumber[i].path.size() && pathLength < m_agentNumber[i2].path.size() && i != i2)
			{
				if (m_agentNumber[i].path.at(pathLength) == m_agentNumber[i2].path.at(pathLength) ||//true if both agents will move into the same square
					(pathLength > 0 && m_agentNumber[i].path.at(pathLength) == m_agentNumber[i2].path.at(pathLength - 1)))//true if first agent plans to move into another agents square
				{//records the intersection
					Blocker temp;
					temp.block = m_agentNumber[i].path.at(pathLength);
					temp.whichAgents = sf::Vector2i(i, i2);
					m_agentNumber[i].blockers.push_back(temp);
					pathLength = 99;
					anyBlockers = true;
				}
				pathLength++;
			}
			while (pathLength < m_agentNumber[i].path.size() && i != i2 && m_agentNumber[i2].path.size()>0)//stops paths going through finished agents
			{
				if (m_agentNumber[i].path.at(pathLength) == m_agentNumber[i2].path.at(m_agentNumber[i2].path.size()-1))
				{
					Blocker temp;
					temp.block = m_agentNumber[i].path.at(pathLength);
					temp.whichAgents = sf::Vector2i(i, i2);
					m_agentNumber[i].blockers.push_back(temp);
				}
				pathLength++;
			}
		}
	}
	for (int i = 0; i < MAX_AGENTS; i++)
	{
		if (m_agentNumber[i].blockers.size() > 0)//only agents with blockers
		{
			int shortestDistance = m_agentNumber[i].path.size();
			m_agentNumber[i].path.clear();
			findPath(i);//finds new path for 'i' with blockers
			if (shortestDistance == m_agentNumber[i].path.size())//true if blockers don't increase time
			{
				for (auto blocker : m_agentNumber[i].blockers)
				{
					//removes blockers from other agents associated with 'i'
					std::vector<Blocker> newVector;
					for (auto blocker2 : m_agentNumber[blocker.whichAgents.y].blockers)
					{
						if (blocker2.whichAgents.y != i)
						{
							newVector.push_back(blocker2);
						}
					}
					m_agentNumber[blocker.whichAgents.y].blockers.clear();
					m_agentNumber[blocker.whichAgents.y].blockers = newVector;
				}
				m_agentNumber[i].blockers.clear();
			}
			else
			{
				while (m_agentNumber[i].blockers.size() > 0)//loops through all blockers
				{
					int i2 = m_agentNumber[i].blockers.at(0).whichAgents.y;//gets the index of first blocker agent
					int shortestDistance2 = m_agentNumber[i2].path.size();
					m_agentNumber[i2].path.clear();
					findPath(i2);
					//compares which altered path is worst for overall time
					if (shortestDistance + m_agentNumber[i2].path.size() <= m_agentNumber[i].path.size() + shortestDistance2
						&& !m_agentNumber[i2].blockers.empty())//incase of finished agents
					{
						//removes first blocker for 'i' and re does the path
						m_agentNumber[i].blockers.erase(m_agentNumber[i].blockers.begin());
						m_agentNumber[i].path.clear();
						findPath(i);
						std::vector<Blocker> newVector;//removes 'i' blocker from 'i2'
						for (auto blocker2 : m_agentNumber[i2].blockers)
						{
							if (blocker2.whichAgents.y != i)
							{
								newVector.push_back(blocker2);
							}
						}
						m_agentNumber[i2].blockers.clear();
						m_agentNumber[i2].blockers = newVector;
					}
					else
					{
						std::vector<Blocker> newVector;//removes 'i' blocker from 'i2'
						for (auto blocker2 : m_agentNumber[i2].blockers)
						{
							if (blocker2.whichAgents.y != i)
							{
								newVector.push_back(blocker2);
							}
						}
						m_agentNumber[i2].blockers.clear();
						m_agentNumber[i2].blockers = newVector;
						m_agentNumber[i2].path.clear();
						findPath(i2);//re does the path and removes first blocker for 'i'
						m_agentNumber[i].blockers.erase(m_agentNumber[i].blockers.begin());
					}
				}
			}
		}
	}
	if (anyBlockers)//if there was a blocker redoes test and handling
	{
		redoPath();
	}
}

void MultiAgentHandler::findPath(int i)
{
	if (m_agentNumber[i].current != m_agentNumber[i].endGoal && m_agentNumber[i].path.size() == 0)//true if not at goal
	{
		for (int width = 0; width < WORLD_WIDTH; width++)
		{
			for (int height = 0; height < WORLD_HEIGHT; height++)
			{
				worldBlocks[width][height].disToGoal = 999;//sets distance to goal higher then can ever be
			}
		}
		worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].passable = true;//current spot is free
		worldBlocks[m_agentNumber[i].endGoal.x][m_agentNumber[i].endGoal.y].disToGoal = 0;//set goal to zero to base the rest of spots off
		int currentDis = 0;//active distance we are dealing with
		currentDis = numberGrid(currentDis, i,m_agentNumber[i].blockers);

		sf::Vector2i currentBlock = m_agentNumber[i].current;
		int distance = currentDis;
		sf::Vector2i goal = m_agentNumber[i].endGoal;
		float bestDis = 99;
		sf::Vector2i changeBlock = currentBlock;
		sf::Vector2i checkBlock = changeBlock;
		while (distance > 0)//loops until goal is reached
		{
			currentBlock = changeBlock; 
			for (int sides = 0; sides < 4; sides++)//goes through all connecting squares to find path
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
						int index = worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].disToGoal - distance;
						if (m_agentNumber[i].path.size() <= index)
						{
							m_agentNumber[i].path.push_back(sf::Vector2i(checkBlock.x, checkBlock.y));//sets 'checkBlock' to next on path
						}
						else
						{
							m_agentNumber[i].path.at(index) = sf::Vector2i(checkBlock.x, checkBlock.y);//replaces part of the path with 'checkBlock'
						}
						bestDis = currentDis;
						changeBlock = sf::Vector2i(checkBlock.x, checkBlock.y);//sets next active to the one just checked
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
	if (m_agentNumber[i].current.y < m_agentNumber[i].endGoal.y &&//true if wants to move down
		worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y + 1].passable)//true if can
	{
		m_agentNumber[i].currentDirection = Direction::down;
	}
	else if (m_agentNumber[i].current.y > m_agentNumber[i].endGoal.y&&//true if wants to move down
		worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y - 1].passable)//true if can
	{
		m_agentNumber[i].currentDirection = Direction::up;
	}
}

void MultiAgentHandler::simpleCheckHorz(int i)
{
	if (m_agentNumber[i].current.x < m_agentNumber[i].endGoal.x &&//true if wants to move right
		worldBlocks[m_agentNumber[i].current.x + 1][m_agentNumber[i].current.y].passable)//true if can
	{
		m_agentNumber[i].currentDirection = Direction::right;
	}
	else if (m_agentNumber[i].current.x > m_agentNumber[i].endGoal.x&&//true if wants to move left
		worldBlocks[m_agentNumber[i].current.x - 1][m_agentNumber[i].current.y].passable)//true if can
	{
		m_agentNumber[i].currentDirection = Direction::left;
	}
}

void MultiAgentHandler::moveSingleAgent(int t_index)
{
	worldBlocks[m_agentNumber[t_index].current.x][m_agentNumber[t_index].current.y].passable = true;//current spot is free
	switch (m_agentNumber[t_index].currentDirection)//moves agent based off decided direction
	{
	case right:
		m_agentNumber[t_index].current.x++;
		break;
	case left:
		m_agentNumber[t_index].current.x--;
		break;
	case up:
		m_agentNumber[t_index].current.y--;
		break;
	case down:
		m_agentNumber[t_index].current.y++;
		break;
	case stay:
		break;
	default:
		break;
	}
	worldBlocks[m_agentNumber[t_index].current.x][m_agentNumber[t_index].current.y].passable = false;//new current spot is blocked
	m_agentNumber[t_index].setPos();//sets agent off current
}

void MultiAgentHandler::setUpAgent(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index)
{
	m_agentNumber[index].setUp(t_start, t_end, t_font, index);
}

int MultiAgentHandler::numberGrid(int currentDis, int i, std::vector<Blocker> t_blockers)
{
	while (currentDis < worldBlocks[m_agentNumber[i].current.x][m_agentNumber[i].current.y].disToGoal)//true if start is larger then current distance
	{
		for (auto eachBlock : m_agentNumber[i].blockers)
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
		for (auto eachBlock : m_agentNumber[i].blockers)
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
		if (!m_agentNumber[i].atGoal())
		{
			allHome = false;
		}
	}
	return allHome;
}
//allows the pattern to change during runtime
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
		average += m_agentNumber[i].pathSize;
	}
	return average/ MAX_AGENTS;
}
