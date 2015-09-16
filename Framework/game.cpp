// 717310 C++ SDL Framework

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "asteroid.h"
#include "playership.h"
#include "PlayerBullet.h"
#include "Vector2D.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_common.h"
#include "animatedsprite.h"
#include "Explosion.h"
#include "particle.h"
#include "particleemitter.h"
#include "asteroidparticle.h"
#include "hud.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cmath>
#include <ctime>
#include <string>
#include <thread>
#include <list>

// Static Members:
Game* Game::sm_pInstance = 0;

FMOD::System *systemFMOD = NULL;
FMOD::Sound *sound1, *sound2, *sound3, *sound4, *maintheme;
FMOD::Channel *channelSFX = 0, *channelMusic;
FMOD_RESULT result;

int backgroundCounter = 0;
Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{ 
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void 
Game::DestroyInstance()
{
	delete sm_pInstance; 
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
{
	
}

Game::~Game()
{
	result = maintheme->release();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = sound1->release();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = sound2->release();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	//insert sounds to close here ^^

	result = systemFMOD->close();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = systemFMOD->release();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	

	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool 
Game::Initialise()
{
	result = FMOD::System_Create(&systemFMOD);      // Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));

		exit(-1);
	}

	result = systemFMOD->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	//initialize individual sounds
	result = systemFMOD->createSound("assets\\gamemusic.wav", FMOD_DEFAULT, 0, &maintheme);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = systemFMOD->createSound("assets\\playershoot.wav", FMOD_DEFAULT, 0, &sound1);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = systemFMOD->createSound("assets\\enemyexplode.wav", FMOD_DEFAULT, 0, &sound2);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = channelSFX->setVolume(0.2);
	result = channelMusic->setVolumeRamp(1);
	result = channelMusic->setVolume(0.2);

	result = maintheme->setMode(FMOD_LOOP_NORMAL);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = systemFMOD->playSound(maintheme, 0, false, &channelMusic);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// Ex006.2: Load the player ship sprite.
	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	player = new PlayerShip();
	player->Initialise(pPlayerSprite);
	player->SetPositionX((float)width / 2);
	player->SetPositionY((float)height / 2);
	player->setDirection(1);
	player->setLives(3);
	player->setScore(1234);

	
	/*Sprite* scoretextSpr = m_pBackBuffer->CreateSprite("assets\\score.png");
	scoretext = new Hud();
	scoretext->Initialise(scoretextSpr);
	scoretext->SetPositionX(20);
	scoretext->SetPositionY(20);*/


	Sprite* livestextSpr = m_pBackBuffer->CreateSprite("assets\\lives.png");
	livestext = new Hud();
	livestext->Initialise(livestextSpr);
	livestext->SetPositionX(20);
	livestext->SetPositionY(20);

	
	// Ex006.3: Spawn four rows of 14 alien enemies.
	m_asteroidCount = 0;

	pGlobalEmitter = new ParticleEmitter();


	for (int i = 0; i < 100; i++){
		float randomVelocityX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		float randomVelocityY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		float randomAccelerationX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1))*directionRandomizer();
		float randomAccelerationY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1))*directionRandomizer();
		Vector2D* position = new Vector2D(400, 300);
		Vector2D* velocity = new Vector2D(randomVelocityX, randomVelocityY);
		Vector2D* acceleration = new Vector2D(0, 0);
		float age = 1000.0;

		Sprite* pParticleSprite = m_pBackBuffer->CreateSprite("assets\\smallstar.png");
		SpawnGlobalParticle(position, velocity, acceleration, age, pParticleSprite);
	}

	for (int i = 0; i < 20; i++){

		float randomVelocityX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		float randomVelocityY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		float randomAccelerationX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		float randomAccelerationY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
		Vector2D* position = new Vector2D(400, 300);
		Vector2D* velocity = new Vector2D(randomVelocityX, randomVelocityY);
		Vector2D* acceleration = new Vector2D(randomAccelerationX, randomAccelerationY);
		float age = 1000.0;

		Sprite* pParticleSprite = m_pBackBuffer->CreateSprite("assets\\biggerstar.png");
		SpawnGlobalParticle(position, velocity, acceleration, age, pParticleSprite);
	}

	SpawnAsteroid(100, 100);

	Sprite* goSprite = m_pBackBuffer->CreateSprite("assets\\gameover.png");

	gameover = new Hud;
	gameover->Initialise(goSprite);
	gameover->SetPositionX(230);
	gameover->SetPositionY(250);
	
	updateHud();
	//DrawScore(140, 5);
	// Ex006.3: Fill the container with these new enemies.

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}


bool 
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessButtons(*this);
	
	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}
		
		Draw(*m_pBackBuffer);
	}
	if (backgroundCounter == 500 || backgroundCounter == 250){

		for (int i = 0; i < 20; i++){
			float randomVelocityX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			float randomVelocityY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			float randomAccelerationX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1))*directionRandomizer();
			float randomAccelerationY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.1))*directionRandomizer();
			Vector2D* position = new Vector2D(400, 300);
			Vector2D* velocity = new Vector2D(randomVelocityX, randomVelocityY);
			Vector2D* acceleration = new Vector2D(0, 0);
			float age = 1000.0;

			Sprite* pParticleSprite = m_pBackBuffer->CreateSprite("assets\\smallstar.png");
			SpawnGlobalParticle(position, velocity, acceleration, age, pParticleSprite);
		}
	}
	if (backgroundCounter % 50 == 0){


		for (int i = 0; i < 5; i++){
			float randomVelocityX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			float randomVelocityY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			float randomAccelerationX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			float randomAccelerationY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*directionRandomizer();
			Vector2D* position = new Vector2D(400, 300);
			Vector2D* velocity = new Vector2D(randomVelocityX/10, randomVelocityY/10);
			Vector2D* acceleration = new Vector2D(randomAccelerationX/10, randomAccelerationY/10);
			float age = 1000.0;

			Sprite* pParticleSprite = m_pBackBuffer->CreateSprite("assets\\biggerstar.png");
			SpawnGlobalParticle(position, velocity, acceleration, age, pParticleSprite);
		}

	}

	if (backgroundCounter >= 500)
	{
		backgroundCounter = 0;
	}

	SDL_Delay(1);

	return (m_looping);
}

void 
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	//scoretext->Process(deltaTime);
	livestext->Process(deltaTime);
	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}

	result = systemFMOD->update();
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	// Update the game world simulation:

	// Ex003.5: Process each alien enemy in the container.
	for each(Asteroid* asteroid in asteroidContainer)
	{
		asteroid->Process(deltaTime);
	}
	for each(Hud* lives in livesContainer)
	{
		lives->Process(deltaTime);
	}
	/*for each(Hud* score in scoreContainer)
	{
		score->Process(deltaTime);
	}*/
	// Ex006.4: Process each bullet in the container.
	for each (PlayerBullet* playerbullet in PlayerbulletContainer)
	{
		playerbullet->Process(deltaTime);
	}

	for each (Explosion* explosion in explosionContainer)
	{
		explosion->Process(deltaTime);
	}
	// Ex006.2: Update player...
	player->Process(deltaTime);
	pGlobalEmitter->Process();
	//hud->Process(deltaTime);

	// Ex006.4: Check for bullet vs alien enemy collisions...
	// Ex006.4: For each bullet
	// Ex006.4: For each alien enemy
	// Ex006.4: Check collision between two entities.
	// Ex006.4: If collided, destory both and spawn explosion.

	// Ex006.4: Remove any dead bullets from the container...

	// Ex006.4: Remove any dead enemy aliens from the container...

	for (int j = 0; j < asteroidContainer.size(); j++)
	{
		Asteroid* asteroid = asteroidContainer.at(j);
		Entity &e = *asteroid;

		if (player->IsCollidingWith(e))
		{
			asteroid->SetDead(true);
			player->SetDead(true);

			if (asteroid->getSize() == 1){
				SpawnAsteroidSmaller(asteroid->GetPositionX(), asteroid->GetPositionY());
				SpawnAsteroidSmaller(asteroid->GetPositionX(), asteroid->GetPositionY());

			}
		}
		if (asteroid->IsDead())
		{
			result = systemFMOD->playSound(sound2, 0, false, &channelSFX);
			if (result != FMOD_OK)
			{
				printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
				exit(-1);
			}
			
			delete asteroid;
			asteroidContainer.erase(asteroidContainer.begin() + j);
		}
		if (player->IsDead())
		{
			SpawnExplosion((int)player->GetPositionX(), (int)player->GetPositionY());
			LifeUpdate(-1);
			
		}

	}

	

	// Ex006.4: Remove any dead explosions from the container...
	for (int i = 0; i < PlayerbulletContainer.size(); i++)
	{
		PlayerBullet* playerbullet = PlayerbulletContainer.at(i);

		for (int j = 0; j < asteroidContainer.size(); j++)
		{
			Asteroid* asteroid = asteroidContainer.at(j);
			Entity &e = *asteroid;

			if (playerbullet->IsCollidingWith(e))
			{
				asteroid->SetDead(true);
				playerbullet->SetDead(true);
				
				if (asteroid->getSize() == 1){
					SpawnAsteroidSmaller(asteroid->GetPositionX(), asteroid->GetPositionY());
					SpawnAsteroidSmaller(asteroid->GetPositionX(), asteroid->GetPositionY());
					
				}
			}
			if (asteroid->IsDead())
			{
				result = systemFMOD->playSound(sound2, 0, false, &channelSFX);
				if (result != FMOD_OK)
				{
					printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
					exit(-1);
				}
				SpawnExplosion((int)asteroid->GetPositionX(), (int)asteroid->GetPositionY());
				
				ScoreUpdate(100);
				delete asteroid;
				asteroidContainer.erase(asteroidContainer.begin() + j);
			}
			
		}
		float x, y;
		x = playerbullet->GetPositionX();
		y = playerbullet->GetPositionY();
		if (x == 20 || x == 790 || y == 20 || y == 590 || playerbullet->IsDead()){
			delete playerbullet;
			PlayerbulletContainer.erase(PlayerbulletContainer.begin() + i);
		}

	}

	for (int j = 0; j < asteroidContainer.size(); j++)
	{
		Asteroid* asteroid = asteroidContainer.at(j);
		Entity &e = *asteroid;
		float x, y;
		x = asteroid->GetPositionX();
		y = asteroid->GetPositionY();
		if (x == 20 || x > 730 || y == 20 || y > 540){
			float xspeedRan = asteroid->GetHorizontalVelocity();
			float yspeedRan = asteroid->GetVerticalVelocity();
			if (x == 20)
			{
				xspeedRan = -xspeedRan;
			}
			if (x > 720)
			{
				xspeedRan = -xspeedRan;
			}
			if (y == 20)
			{
				yspeedRan = -yspeedRan;
			}
			if (y > 520)
			{
				yspeedRan = -yspeedRan;
			}

			asteroid->SetHorizontalVelocity(xspeedRan);
			asteroid->SetVerticalVelocity(yspeedRan);
		}
	}

	//m_asteroidCount = m_asteroidCount + 1;
	if (asteroidContainer.empty())
	{
		m_asteroidCount = m_asteroidCount + 2;
		ScreenClear();
	}

	if (player->getLives() <= 0)
	{
		for each(Asteroid* asteroid in asteroidContainer)
		{
			asteroid->SetHorizontalVelocity(0);
			asteroid->SetVerticalVelocity(0);
		}
		setGameOver(true);
	}
	
	
}

void 
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	pGlobalEmitter->Draw(backBuffer);
	// Ex006.3: Draw all enemy aliens in container...
	for each(Asteroid* asteroid in asteroidContainer)
	{
		if (asteroid->getSize() == 1)
		{
			asteroid->Draw(backBuffer);
		}
		if (asteroid->getSize() == 2)
		{
			asteroid->DrawSmaller(backBuffer);
		}
	}
	// Ex006.4: Draw all bullets in container...
	for each (PlayerBullet* bullet in PlayerbulletContainer)
	{
		int m = 20;
		bullet->Draw(backBuffer);
		int compare = bullet->GetPositionY();
		if (compare%m == 0)
		{

			Vector2D* position = new Vector2D(bullet->GetPositionX() + 5, bullet->GetPositionY() + 5);
			Vector2D* velocity = new Vector2D(0, 0);
			Vector2D* acceleration = new Vector2D(0, 0);
			float age = 50.0;

		}

	}

	for each (Explosion* explosion in explosionContainer)
	{
		explosion->Draw(backBuffer);
	}
	// Ex006.2: Draw the player ship...
	/*for each(Hud* score in scoreContainer)
	{
		score->Draw(backBuffer);
	}*/

	//scoretext->Draw(backBuffer);
	livestext->Draw(backBuffer);
	for each(Hud* lives in livesContainer)
	{
		lives->DrawSmaller(backBuffer);
	}

	

	if (player->getDirection() == 1){
		player->Draw(backBuffer);
	}
	if (player->getDirection() == 2){
		player->Rotate(backBuffer);
	}
	if (player->getDirection() == 3){
		player->Rotate(backBuffer);
	}
	if (player->getDirection() == 4){
		player->Rotate(backBuffer);
	}

	if (GO == true){
		gameover->Draw(backBuffer);
	}

	backBuffer.Present();
}

void 
Game::Quit()
{
	m_looping = false;
}

void 
Game::MoveSpaceShipLeft()
{
	// Ex006.2: Tell the player ship to move left.
	if (player->getLives() != 0)
	{
		player->SetHorizontalVelocity(-130);
	}
}

// Ex006.2: Add the method to tell the player ship to move right...
void
Game::MoveSpaceShipRight()
{
	if (player->getLives() != 0)
	{
		player->SetHorizontalVelocity(+130);
	}
}

void
Game::MoveSpaceShipUp()
{
	if (player->getLives() != 0)
	{
		player->SetVerticalVelocity(-130);
	}
}

void
Game::MoveSpaceShipDown()
{
	if (player->getLives() != 0)
	{
		player->SetVerticalVelocity(+130);
	}
}

void
Game::StopMovement()
{
	player->SetHorizontalVelocity(0);
	player->SetVerticalVelocity(0);
}

// Ex006.4: Space a Bullet in game.
void 
Game::FireSpaceShipBullet(int but, double ang, SDL_RendererFlip flip)
{
	if (player->getLives() != 0)
	{
		if (PlayerbulletContainer.size() < 8){
			result = systemFMOD->playSound(sound1, 0, false, &channelSFX);
			if (result != FMOD_OK)
			{
				printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
				exit(-1);
			}
			// Ex006.4: Load the player bullet sprite.      
			Sprite* pBulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
			// Ex006.4: Create a new bullet object.
			PlayerBullet* playerbullet = new PlayerBullet();

			if (but == 1){
				playerbullet->SetPositionX(player->GetPositionX());
				playerbullet->SetPositionY(player->GetPositionY());

				playerbullet->Initialise(pBulletSprite);
				// Shoot upwards
				playerbullet->SetVerticalVelocity(-150);
				player->setDirection(1);
				player->setFlip(flip);
				player->setAngle(ang);
			}
			if (but == 2){
				playerbullet->SetPositionX(player->GetPositionX());
				playerbullet->SetPositionY(player->GetPositionY());

				playerbullet->Initialise(pBulletSprite);
				// Shoot downwards
				playerbullet->SetVerticalVelocity(+150);
				player->setDirection(2);
				player->setFlip(flip);
				player->setAngle(ang);
			}
			if (but == 3){
				playerbullet->SetPositionX(player->GetPositionX());
				playerbullet->SetPositionY(player->GetPositionY());

				playerbullet->Initialise(pBulletSprite);
				// 
				playerbullet->SetHorizontalVelocity(-150);
				player->setDirection(3);
				player->setFlip(flip);
				player->setAngle(ang);
			}
			if (but == 4){
				playerbullet->SetPositionX(player->GetPositionX());
				playerbullet->SetPositionY(player->GetPositionY());

				playerbullet->Initialise(pBulletSprite);
				// Ex006.4: Set the bullets vertical velocity.
				playerbullet->SetHorizontalVelocity(+150);
				player->setDirection(4);
				player->setFlip(flip);
				player->setAngle(ang);
			}

			// Ex006.4: Add the new bullet to the bullet container.
			PlayerbulletContainer.push_back(playerbullet);
		}
	}
}

// Ex006.3: Spawn a Enemy in game.
void 
Game::SpawnAsteroid(int x, int y)
{
	// Ex006.3: Load the alien enemy sprite file.
	Sprite* pAsteroidSprite = m_pBackBuffer->CreateSprite("assets\\asteroid.png");
	// Ex006.3: Create a new Enemy object. 
	Asteroid* asteroid = new Asteroid();
	asteroid->Initialise(pAsteroidSprite);
	asteroid->SetPositionX((float)x);
	asteroid->SetPositionY((float)y);
	asteroid->setType(1);

	int speedRan = ((rand()) % 50 + 15);
	float randomizer;
	int randomDirection = (rand()) % 2;
	if (randomDirection == 1)
	{
		randomizer = speedRan;
	}
	else
	{
		randomizer = -speedRan;
	}

	asteroid->SetHorizontalVelocity(randomizer);
	asteroid->SetVerticalVelocity(randomizer);
	asteroid->setSize(0);
	asteroid->setSize(asteroid->getSize() + 1);
	// Ex006.3: Add the new Enemy to the enemy container.
	asteroidContainer.push_back(asteroid);
	
}


void
Game::ScreenClear()
{
	asteroidContainer.clear();
	PlayerbulletContainer.clear();
	int counter = m_asteroidCount;
	counter = counter * 3;
	counter = counter - 2;
	if (asteroidContainer.empty())
	{
		while (counter != 0){
			int x = (rand()) % 740;
			int y = (rand()) % 490;

			SpawnAsteroid(x, y);

			counter--;
		}
	}
}

void
Game::LifeUpdate(int life)
{
	int p_life = player->getLives();
	if (life == -1)
	{
		p_life += life;
		player->setLives(p_life);
		ResetPlayer();
	}
	if (life == 1)
	{
		p_life += life;
		player->setLives(p_life);
	}
	updateHud();
}

void
Game::ResetPlayer()
{
	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	int lives = player->getLives();
	int score = player->getScore();
	player = new PlayerShip();

	player->Initialise(pPlayerSprite);
	player->SetPositionX(400);
	player->SetPositionY(300);
	player->setLives(lives);
	player->setDirection(1);

}

void
Game::SpawnExplosion(int x, int y)
{
	AnimatedSprite* pExplosionSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");

	Explosion* explosion = new Explosion();
	explosion->Initialise(pExplosionSprite);
	explosion->SetPositionX((float)x);
	explosion->SetPositionY((float)y);
	explosionContainer.push_back(explosion);
}

void
Game::SpawnLocalParticle(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, float age, Sprite* sprite)
{


	Particle* particle = new AsteroidParticle(*position, *velocity, *acceleration, age);
	particle->Initialise(sprite);

	pLocalEmitter->addParticle(particle);
}

float
Game::directionRandomizer()
{
	float randomizer;
	int randomDirection = (rand()) % 2;
	if (randomDirection == 1)
	{
		randomizer = 1.0;
	}
	else
	{
		randomizer = -1.0;
	}

	return randomizer;
}

void
Game::SpawnAsteroidSmaller(int x, int y){
	
	Sprite* pAsteroidSprite = m_pBackBuffer->CreateSprite("assets\\asteroid.png");
	// Ex006.3: Create a new Enemy object. 
	Asteroid* asteroid = new Asteroid();
	asteroid->Initialise(pAsteroidSprite);
	asteroid->SetPositionX((float)x);
	asteroid->SetPositionY((float)y);
	asteroid->setType(2);
	int speedRan = ((rand()) % 50 + 60);
	float randomizer;
	int randomDirection = (rand()) % 2;
	if (randomDirection == 1)
	{
		randomizer = speedRan;
	}
	else
	{
		randomizer = -speedRan;
	}

	asteroid->SetHorizontalVelocity(randomizer);
	asteroid->SetVerticalVelocity(randomizer);
	asteroid->setSize(2);
	// Ex006.3: Add the new Enemy to the enemy container.
	asteroidContainer.push_back(asteroid);
}

void
Game::ScoreUpdate(int score)
{
	int ss;
	ss = player->getScore();
	ss = ss + score;
	player->setScore(ss);

}

void
Game::SpawnGlobalParticle(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, float age, Sprite* sprite)
{


	AsteroidParticle* particle = new AsteroidParticle(*position, *velocity, *acceleration, age);
	particle->Initialise(sprite);

	pGlobalEmitter->addParticle(particle);
}

void
Game::SpawnLife(int x){
	Sprite* pLifeSprite = m_pBackBuffer->CreateSprite("assets\\playership.png");
	// Ex006.3: Create a new Enemy object. 
	Hud* life = new Hud();
	life->Initialise(pLifeSprite);
	life->SetPositionX((float)x);
	life->SetPositionY(30);
	// Ex006.3: Add the new Enemy to the enemy container.
	livesContainer.push_back(life);
}

//void
//Game::DrawScore(int x, int num){
//	Sprite* pScoreSprite = m_pBackBuffer->CreateSprite("assets\\zero.png");
//	
//	Hud* score = new Hud;
//	// Ex006.3: Create a new Enemy object. 
//	score->Initialise(pScoreSprite);
//	score->SetPositionX((float)x);
//	score->setScore(num);
//	score->SetPositionY(20);
//	// Ex006.3: Add the new Enemy to the enemy container.
//	scoreContainer.push_back(score);
//
//}

void
Game::updateHud(){

	livesContainer.clear();
	//scoreContainer.clear();
	int x = 140;
	int counter = player->getLives();
	while (counter > 0){
		SpawnLife(x);

		x = x + 20;
		counter--;
	}
	
	//int score = player->getScore();
	//if (0 == score) {
	//	Hud* digit = new Hud;
	//	digit->setScore(0);
	//	scoreContainer.push_back(digit);
	//}
	//else {
	//	while (score != 0) {
	//		int last = score % 10;
	//		Hud* digit = new Hud;
	//		digit->setScore(last);
	//		scoreContainer.push_back(digit);
	//		score = (score - last) / 10;
	//	}
	//}

	//x = 140;

	////DrawScore(150, 0);
	
}

bool
Game::getGameOver()
{
	return GO;
}

void
Game::setGameOver(bool b)
{
	GO = b;
}