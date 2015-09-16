// 717310 C++ SDL Framework

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"

// Library includes:
#include <cassert>

#include <SDL.h>

InputHandler::InputHandler()
: m_pGameController(0)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool 
InputHandler::Initialise()
{
	int numControllesr = SDL_NumJoysticks();

	m_pGameController = SDL_JoystickOpen(0);
	assert(m_pGameController);

	return (true);
}

void 
InputHandler::ProcessButtons(Game& game)
{
	// Ex006.2: Receive Input Events below...
	SDL_Event e;
	float angle = 0;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		else if (e.type == SDL_JOYBUTTONDOWN && game.getGameOver())
		{
			// shoot from A button, south
			if (e.jbutton.button == 10)
			{
				game.FireSpaceShipBullet(2, 0, SDL_FLIP_VERTICAL);
				
			}

			// shoot from B button, east
			if (e.jbutton.button == 11)
			{
				game.FireSpaceShipBullet(4, 90, SDL_FLIP_NONE);

			}

			// shoot from X button, west
			if (e.jbutton.button == 12)
			{
				game.FireSpaceShipBullet(3, -90, SDL_FLIP_NONE);

			}

			// shoot from Y button, north
			if (e.jbutton.button == 13)
			{
				game.FireSpaceShipBullet(1, 0, SDL_FLIP_NONE);

			}

			//move space ship down
			if (e.jbutton.button == 1)
			{
				game.MoveSpaceShipDown();

			}
			if (e.jbutton.button == 0)
			{
				game.MoveSpaceShipUp();

			}
			// Ex006.2: Tell the game to move the space ship left...
			if (e.jbutton.button == 3)
			{
				game.MoveSpaceShipRight();
			}
			// Ex006.2: Tell the game to move the space ship right...
			if (e.jbutton.button == 2)
			{
				game.MoveSpaceShipLeft();
			}

			if (e.jbutton.button == 6)
			{
				game.Quit();
			}
			
		}
		else if (e.type == SDL_JOYBUTTONUP && game.getGameOver())
		{
			if (e.jbutton.button == 3 || e.jbutton.button == 2 || e.jbutton.button == 1 || e.jbutton.button == 0)
			{
				game.StopMovement();
			}
		}
		else if (game.getGameOver() == false)
		{
			if (e.jbutton.button == 5)
			{
				game.setGameOver(true);
				game.ResetPlayer();
			}
		}
	}
}
