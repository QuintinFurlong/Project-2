/// <summary>
/// @author Quintin
/// </summary>

#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ (aHandler.WORLD_WIDTH+2)* 100U, (aHandler.WORLD_HEIGHT+2) * 100U, 32U }, "Project 2 Quitnin" },
	m_exitGame{false}
{
	time = 0;
	oldTime = 0;
	aHandler.setPattern(MovePatterns::straightForward);

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
			processKeys(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Space == t_event.key.code)
	{
		time++;
		m_timeText.setString(std::to_string(time));
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	if (time != oldTime)
	{
		aHandler.moveAgents();
		oldTime = time;
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
			aHandler.worldBlocks[width][height].square.setPosition((width + 1) * 100, (height + 1) * 100);
			aHandler.worldBlocks[width][height].square.setSize(sf::Vector2f(99, 99));
			aHandler.worldBlocks[width][height].square.setFillColor(sf::Color::Green);
			aHandler.worldBlocks[width][height].square.setOutlineThickness(1);
			aHandler.worldBlocks[width][height].square.setOutlineColor(sf::Color::Black);
			aHandler.worldBlocks[width][height].passable = true;
		}
	}
	
	aHandler.agentNumber[0].setUp(sf::Vector2i(5,5), sf::Vector2i(7, 15), &m_ArialBlackfont, 0);
	aHandler.agentNumber[1].setUp(sf::Vector2i(5, 15), sf::Vector2i(15, 15), &m_ArialBlackfont, 1);
	aHandler.agentNumber[2].setUp(sf::Vector2i(15, 5), sf::Vector2i(5, 15), &m_ArialBlackfont, 2);
	aHandler.agentNumber[3].setUp(sf::Vector2i(0, 0), sf::Vector2i(22, 15), &m_ArialBlackfont, 3);
	aHandler.agentNumber[4].setUp(sf::Vector2i(0, 15), sf::Vector2i(22, 0), &m_ArialBlackfont, 4);
}
