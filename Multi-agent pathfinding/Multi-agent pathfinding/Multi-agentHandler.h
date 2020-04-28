#pragma once
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "WorldBlock.h"
#include <iostream>

enum MovePatterns
{
	straightForward,//moves right/left till same column then up/down till same block
	stairs, //moves right/left if can't then up/down till same block
	numberAdjacent,//Dijkstra's algorithm
	recordedPath,//does a dijkstra for each agent and keeps previous paths as obstales
	optimal
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
	void adjacentPathFunc();
	void optimalPathFunc();
	void redoPath();
	void findPath(int i);
	void simpleCheckVert(int index);
	void simpleCheckHorz(int index);
	void moveSingleAgent(int t_index);
	void setUpAgent(sf::Vector2i t_start, sf::Vector2i t_end, sf::Font* t_font, int index);
	int numberGrid(int currentDis, int i, std::vector<Blocker> t_blockers);
	bool allAtGoal();
	void changePath(bool t_increase);
	std::string pathName();
	float averagePathSize();
};