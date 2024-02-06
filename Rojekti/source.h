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

	

	enum
	{
		PLAYER,
		ASTEROID
	};


	//Maximum axis velocity of the Player
	int THING_VEL = 2;
	int MAX_SPEED = 10;

	//Initializes the variables
	Thing(SDL_Renderer** renderer, int x, int y, int direction, int type);


	~Thing();

	SDL_Renderer* thingRenderer;


	void handleEvent(SDL_Event& e);

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

void detectCollision(std::vector<Thing>& asteroids, Thing& player, int asteroidCount);

void moveAsteroids(std::vector<Thing>& asteroids, int asteroidCount);

void checkAsteroidExplosion(std::vector<Thing> &asteroids, int asteroidCount, Mix_Chunk *vineBoom);

void CheckPlayer(Thing& player, int &asteroidCount, int &highScore, Mix_Chunk* vineBoom);

void renderAsteroids(std::vector<Thing> &asteroids, int asteroidCount, LTexture &asteroidTexture, LTexture &explosionTexture);