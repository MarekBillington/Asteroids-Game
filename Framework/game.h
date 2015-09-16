// 717310 C++ SDL Framework
#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <SDL.h>
// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class PlayerShip;
class Vector2D;
class PlayerBullet;
class Asteroid;
class Explosion;
class Particle;
class ParticleEmitter;
class Hud;

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void StopMovement();
	void MoveSpaceShipLeft();
	void MoveSpaceShipRight();
	void MoveSpaceShipUp();
	void MoveSpaceShipDown();
	void FireSpaceShipBullet(int but, double ang, SDL_RendererFlip flip);

	void ScoreUpdate(int score);
	void LifeUpdate(int life);
	void ResetPlayer();

	void RotateSpaceShipCW(float angle);
	void RotateSpaceShipAntiCW(float angle);

	void SpawnAsteroidSmaller(int x, int y);

	void MovePlayer(int x, int y);
	
	void ScreenClear();

	float directionRandomizer();

	void SpawnAsteroid(int x, int y);
	void SpawnLocalParticle(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, float age, Sprite* sprite);
	void SpawnGlobalParticle(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, float age, Sprite* sprite);
	void SpawnEnemy(int x, int y);
	void SpawnExplosion(int x, int y);
	void DrawScore(int x, int num);
	void SpawnGameOver();
	bool getGameOver();
	void setGameOver(bool g);

	void updateHud();
	void SpawnLife(int x);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;
	bool GO;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;

	// Game Entities:
	// Ex006.2: Add a PlayerShip field.
	PlayerShip* player;
	Hud* scoretext;
	
	Hud* livestext;
	
	Hud* gameover;

	std::vector<Hud*> livesContainer;
	std::vector<Hud*> scoreContainer;

	// Ex006.3: Add an asteroid enemy container field.
	std::vector<Asteroid*> asteroidContainer;
	int m_asteroidCount;
	// Ex006.4: Add a bullet container field.
	std::vector<PlayerBullet*> PlayerbulletContainer;

	std::vector<Explosion*> explosionContainer;

	ParticleEmitter* pLocalEmitter;
	ParticleEmitter* pGlobalEmitter;

private:

};

#endif // __GAME_H__
