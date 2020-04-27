/// <summary>
/// author Quintin Furlong
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "WorldBlock.h"
#include "Multi-agentHandler.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents(); 
	void processKeysRelease(sf::Event t_event);
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupWorldAndAgents();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_timeText; // text used for message on screen
	int oldTime;
	float newTime;
	MultiAgentHandler aHandler;
	bool m_exitGame; // control exiting game
	static const int BLOCK_SIZE = 50;
};

#endif // !GAME_HPP

