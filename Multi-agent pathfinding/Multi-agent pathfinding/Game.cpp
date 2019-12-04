/// <summary>
/// @author Quintin
/// </summary>

#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ (WORLD_WIDTH+2)* 100U, (WORLD_HEIGHT+2) * 100U, 32U }, "SFML Game" },
	m_exitGame{false}
{
	setupFontAndText(); 
	for (int width = 0;width < WORLD_WIDTH;width++)
	{
		for (int height = 0; height < WORLD_HEIGHT; height++)
		{
			worldBlocks[width][height].setPosition((width+1)*100, (height + 1) * 100);
			worldBlocks[width][height].setSize(sf::Vector2f(99,99));
			worldBlocks[width][height].setFillColor(sf::Color::Green);
		}
	}
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
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}

void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}


void Game::render()
{
	m_window.clear(sf::Color::White);
	for (int width = 0; width < WORLD_WIDTH; width++)
	{
		for (int height = 0; height < WORLD_HEIGHT; height++)
		{
			m_window.draw(worldBlocks[width][height]);
		}
	}
	m_window.display();
}


void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}
