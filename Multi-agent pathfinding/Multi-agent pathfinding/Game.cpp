/// <summary>
/// @author Quintin
/// </summary>

#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ (aHandler.WORLD_WIDTH+2)* BLOCK_SIZE, (aHandler.WORLD_HEIGHT+2) * BLOCK_SIZE, 32U }, "Project 2 Quitnin" },
	m_exitGame{false}
{
	time = 0;
	oldTime = 0;
	newTime = 0;
	aHandler.setPattern(MovePatterns::recordedPath);

	setupFontAndText(); 
	setupWorldAndAgents();
}

Game::~Game()
{
	
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyReleased == newEvent.type) //user pressed a key
		{
			processKeysRelease(newEvent);
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}

void Game::processKeysRelease(sf::Event t_event)
{
	if (sf::Keyboard::Space == t_event.key.code)
	{
		newTime++;
	}
	if (sf::Keyboard::R == t_event.key.code)
	{
		setupWorldAndAgents();
		time = 0;
		oldTime = 0;
		newTime = 0;
		m_timeText.setString(std::to_string(time));
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::P == t_event.key.code)
	{
		newTime++;
	}
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_window.close();
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	//newTime += t_deltaTime.asSeconds();
	if (newTime - 1 >= oldTime)//time != oldTime 
	{
		m_timeText.setString(std::to_string(time));
		aHandler.moveAgents();
		oldTime = time;
		oldTime = newTime;
	}
	m_timeText.setString(std::to_string(newTime));
	if (aHandler.allAtGoal())
	{
		newTime = 0;
		setupWorldAndAgents();
		time = 0;
		oldTime = 0;
		
	}
}


void Game::render()
{
	m_window.clear(sf::Color::White);

	aHandler.draw(m_window);
	m_window.draw(m_timeText);

	m_window.display();
}


void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_timeText.setFont(m_ArialBlackfont);
	m_timeText.setString(std::to_string(time));
	m_timeText.setPosition(40.0f, 0.0f);
	m_timeText.setCharacterSize(80U);
	m_timeText.setFillColor(sf::Color::Black);

}

void Game::setupWorldAndAgents()
{
	for (int width = 0; width < aHandler.WORLD_WIDTH; width++)
	{
		for (int height = 0; height < aHandler.WORLD_HEIGHT; height++)
		{
			aHandler.worldBlocks[width][height].square.setPosition((width + 1) * BLOCK_SIZE, (height + 1) * BLOCK_SIZE);
			aHandler.worldBlocks[width][height].square.setSize(sf::Vector2f(BLOCK_SIZE-1, BLOCK_SIZE-1));
			aHandler.worldBlocks[width][height].square.setFillColor(sf::Color::Green);
			aHandler.worldBlocks[width][height].square.setOutlineThickness(1);
			aHandler.worldBlocks[width][height].square.setOutlineColor(sf::Color::Black);
			aHandler.worldBlocks[width][height].passable = true;
		}
	}
	aHandler.setUpAgent(sf::Vector2i(5,4), sf::Vector2i(7, 15), &m_ArialBlackfont, 0);
	aHandler.setUpAgent(sf::Vector2i(11, 15), sf::Vector2i(16, 15), &m_ArialBlackfont, 1);
	aHandler.setUpAgent(sf::Vector2i(5, 8), sf::Vector2i(5, 3), &m_ArialBlackfont, 2);
	aHandler.setUpAgent(sf::Vector2i(0, 0), sf::Vector2i(22, 0), &m_ArialBlackfont, 3);
	aHandler.setUpAgent(sf::Vector2i(0, 15), sf::Vector2i(22, 15), &m_ArialBlackfont, 4);
	aHandler.setUpAgent(sf::Vector2i(20,9), sf::Vector2i(22, 5), &m_ArialBlackfont, 5);
	aHandler.setUpAgent(sf::Vector2i(17,2), sf::Vector2i(12, 5), &m_ArialBlackfont, 6);
	aHandler.setUpAgent(sf::Vector2i(1, 5), sf::Vector2i(22, 6), &m_ArialBlackfont, 7);
	aHandler.setUpAgent(sf::Vector2i(21, 8), sf::Vector2i(2, 15), &m_ArialBlackfont, 8);
	aHandler.setUpAgent(sf::Vector2i(15, 15), sf::Vector2i(10, 15), &m_ArialBlackfont, 9);

	aHandler.worldBlocks[3][15].passable = false;
	aHandler.worldBlocks[3][14].passable = false;

	aHandler.worldBlocks[4][4].passable = false;
	aHandler.worldBlocks[4][5].passable = false;
	aHandler.worldBlocks[4][6].passable = false;
	aHandler.worldBlocks[4][7].passable = false;
	aHandler.worldBlocks[4][8].passable = false;
	aHandler.worldBlocks[6][4].passable = false;
	aHandler.worldBlocks[6][5].passable = false;
	aHandler.worldBlocks[6][6].passable = false;
	aHandler.worldBlocks[6][7].passable = false;
	aHandler.worldBlocks[6][8].passable = false;
	aHandler.worldBlocks[6][3].passable = false;
}
