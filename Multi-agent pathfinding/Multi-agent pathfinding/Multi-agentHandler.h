#pragma once
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "WorldBlock.h"
#include <iostream>

enum MovePatterns
{
	straightForward,//moves right/left till same column then up/down till same block
	stairs, 
	numberAdjacent
};

class MultiAgentHandler
{
public:
	static const int MAX_AGENTS = 10;
	Agent agentNumber[MAX_AGENTS];
	static const int WORLD_WIDTH = 24;
	static const int WORLD_HEIGHT = 16;

	WorldBlock worldBlocks[WORLD_WIDTH][WORLD_HEIGHT];
	MovePatterns currentPattern;

	MultiAgentHandler();
	void setPattern(MovePatterns t_move);
	void moveAgents();
	void draw(sf::RenderWindow& t_window);

	void straightForwFunc();
	void stairsFunc();
	void adjacentFunc();
	void simpleCheckVert(int index);
	void simpleCheckHorz(int index);
	void moveSingleAgent(int t_index);
};