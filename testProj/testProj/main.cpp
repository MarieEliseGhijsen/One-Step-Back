/// <summary>
/// simple game loop for SFML[3.1]
/// 
/// author Pete Lowe May 2026
/// you need to change the above line or lose marks
/// </summary>



#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <iostream>

#include "Game.h"

int main()
{
	srand(time(0));

	Game game;
	game.run();
		
	return EXIT_SUCCESS; //0
}