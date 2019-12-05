#pragma once
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "WorldBlock.h"

enum MovePatterns
{
	straightForward,
	two, 
	three
};

class MultiAgentHandler
{
public:
	static const int MAX_AGENTS = 5;
	Agent agentNumber[MAX_AGENTS];
	static const int WORLD_WIDTH = 24;
	static const int WORLD_HEIGHT = 16;

	WorldBlock worldBlocks[WORLD_WIDTH][WORLD_HEIGHT];
	MovePatterns currentPattern;

	MultiAgentHandler();
	void setPattern(MovePatterns t_move);
	void moveAgents();
	void draw(sf::RenderWindow& t_window);
	void simpleMovement();
	void moveSingleAgent(int t_index);
};