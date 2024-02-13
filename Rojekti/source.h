#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>


class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(SDL_Renderer** renderer, std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)
	//Creates image from font string
	bool loadFromRenderedText(SDL_Renderer** renderer, TTF_Font** font, std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer** renderer, int x, int y, SDL_Rect* clip = NULL, SDL_Rect* destQuad = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class Timer
{
public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

class Player
{
public:
	//The dimensions of the Player
	const int PLAYER_WIDTH = 100;
	const int PLAYER_HEIGHT = 100;
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//Maximum axis velocity of the Player
	int PLAYER_VEL = 20;

	//Initializes the variables
	Player(SDL_Renderer** renderer, int x, int y, int direction);

	//Deconstructor
	~Player();

	SDL_Renderer* playerRenderer;

	void handleEvent(SDL_Event& e);

	//Moves the Player and checks collision
	void move();

	void explode();

	//Shows the Player on the screen
	void render(LTexture& texture);

	int getPosX();

	int getPosY();

	int getDirection();

	double getRotation();

	bool getExploded();

	bool getHasShot();

	void setHasShot(bool a);

	SDL_Rect getClip();

	int getSpawnPointX();

	int getSpawnPointY();

	bool getSoundEffectPlaying();

	void setSoundEffectPlaying(bool a);

	void spawn(int x, int y, int direction);


	LTexture playerTexture;
	LTexture currentTexture;
	LTexture explosionTexture;

	//Gets the collision boxes
	std::vector<SDL_Rect>& getColliders();

	

private:

	bool mPlayingSoundEffect;

	//The X and Y offsets of the Player
	int mPosX, mPosY;

	//Initial position
	int mPreviousPosX, mPreviousPosY;

	int mSpawnPointX, mSpawnPointY;

	//The velocity of the Player
	int mVelX, mVelY;



	SDL_Rect mClip;

	double mRotation;

	int mDirection;

	int mFrameCounter = 0;

	bool hasExploded;

	bool hasShot;

	//Player's collision boxes
	std::vector<SDL_Rect> mColliders;

	//Moves the collision boxes relative to the Player's offset
	void shiftColliders();
};

class Projectile
{
public:
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const int PROJECTILE_WIDTH = 65;
	const int PROJECTILE_HEIGHT = 216;
	const int PLAYER_WIDTH = 100;
	const int PLAYER_HEIGHT = 100;
	int reloadTime = 500;
	Projectile(SDL_Renderer** renderer);

	//Start the projectile
	void shoot(int x, int y, int direction);

	Timer reloadTimer;

	const int PROJECTILE_VEL = 30;

	LTexture projectileTexture;
	//Move the projectile while it is alive
	void move();

	SDL_Rect getColliders();

	void shiftColliders();

	int getProjectileX();

	int getProjectileY();

	double getProjectileRotation();
	//Get isAlive status, which means that we will render Projectile on screen and enable collision
	bool getAlive();

	void setAlive(bool a);

private:
	
	int mProjectileX, mProjectileY, mPreviousProjectileX, mPreviousProjectileY;
	
	int mProjectileVelX, mProjectileVelY;
	
	int mProjectileDirection = UP;
	
	double mProjectileRotation;
	
	SDL_Rect mProjectileColliders;

	bool isAlive;

};

class Thing
{
public:
	//The dimensions of the Player
	static const int THING_WIDTH = 100;
	static const int THING_HEIGHT = 100;
	static const int SCREEN_WIDTH = 1280;
	static const int SCREEN_HEIGHT = 720;
	
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//Maximum axis velocity of the asteroid
	int THING_VEL = 2;
	int MAX_SPEED = 10;

	//Initializes the variables
	Thing(SDL_Renderer** renderer, int x, int y, int direction, int type);


	~Thing();

	SDL_Renderer* thingRenderer;

	//Moves the Player and checks collision
	void move();

	void explode();

	//Shows the Player on the screen
	void render( LTexture &texture);

	int getPosX();

	int getPosY();

	int getDirection();

	double getRotation();

	bool getExploded();

	SDL_Rect getClip();

	int getSpawnPointX();

	int getSpawnPointY();

	bool getSoundEffectPlaying();

	void setSoundEffectPlaying(bool a);

	void spawn(int x, int y, int direction);


	LTexture playerTexture;
	LTexture currentTexture;
	LTexture asteroidTexture;
	LTexture explosionTexture;

	//Gets the collision boxes
	std::vector<SDL_Rect>& getColliders();

private:

	bool mPlayingSoundEffect;

	//The X and Y offsets of the Player
	int mPosX, mPosY;

	//Initial position
	int mPreviousPosX, mPreviousPosY;

	int mSpawnPointX, mSpawnPointY;

	//The velocity of the Player
	int mVelX, mVelY;

	int mType;

	SDL_Rect mClip;

	double mRotation;

	int mDirection;

	int mFrameCounter = 0;

	bool hasExploded;

	//Player's collision boxes
	std::vector<SDL_Rect> mColliders;

	//Moves the collision boxes relative to the Player's offset
	void shiftColliders();
};

class OpeningScreen
{
public:
	OpeningScreen(SDL_Renderer** renderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

	LTexture gameNameTexture;
	LTexture infoTexture;
	LTexture instructionTexture;
	LTexture backgroundTexture;

	std::string gameNameText;
	std::string infoText;
	std::string	instructionText;

	SDL_Renderer* OpeningScreenRenderer;

	TTF_Font* sansFont;

	SDL_Color textColor{ 0x33, 0x33, 0x33 };

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	bool start = false;

	void render(SDL_Renderer** renderer);
};

class InGameUI
{
public:
	InGameUI();

	void assignText(int asteroidCount, int highScore, int fps);

	void render(SDL_Renderer **renderer);

	std::stringstream asteroidText;
	std::stringstream highScoreText;
	std::stringstream fpsText;

	LTexture asteroidTextTexture;
	LTexture highScoreTextTexture;
	LTexture fpsTextTexture;

	TTF_Font* sansFont;

	SDL_Color textColor{0, 0, 0};
};


bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b);

bool loadMusic(Mix_Music** music);

bool loadSoundEffect(Mix_Chunk** chunk);

bool loadFont(TTF_Font** font, int fontSize = 14);

bool init(SDL_Renderer** renderer, SDL_Window** window, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

void closeAll(SDL_Renderer** renderer, SDL_Window** window);

void createAsteroidArray(SDL_Renderer** renderer, std::vector<Thing> asteroids);

int spawnAsteroid(std::vector<Thing> &asteroids, int asteroidCount, int frameCount);

void shootingHandler(Player &player, Projectile &laser);

void renderMoveLaser(Player& player, Projectile &laser);

void detectCollision(std::vector<Thing>& asteroids, Player& player, Projectile& laser, int asteroidCount);

void moveAsteroids(std::vector<Thing>& asteroids, int asteroidCount);

void checkAsteroidExplosion(std::vector<Thing> &asteroids, int asteroidCount, Mix_Chunk *vineBoom);

void CheckPlayer(Player& player, int &asteroidCount, int &highScore, Mix_Chunk* vineBoom);

void renderAsteroids(std::vector<Thing> &asteroids, int asteroidCount, LTexture &asteroidTexture, LTexture &explosionTexture);