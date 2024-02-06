#include "source.h"

bool checkCollision(std::vector<SDL_Rect>& a, std::vector<SDL_Rect>& b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Go through the A boxes
	for (int Abox = 0; Abox < a.size(); Abox++)
	{
		//Calculate the sides of rect A
		leftA = a[Abox].x;
		rightA = a[Abox].x + a[Abox].w;
		topA = a[Abox].y;
		bottomA = a[Abox].y + a[Abox].h;
		//Go through the B boxes
		for (int Bbox = 0; Bbox < b.size(); Bbox++)
		{
			//Calculate the sides of rect B
			leftB = b[Bbox].x;
			rightB = b[Bbox].x + b[Bbox].w;
			topB = b[Bbox].y;
			bottomB = b[Bbox].y + b[Bbox].h;

			//If no sides from A are outside of B
			if (((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
			{
				//A collision is detected
				return true;
			}
		}
	}

	//If neither set of collision boxes touched
	return false;
}

bool init(SDL_Renderer** renderer, SDL_Window** window, const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		*window = SDL_CreateWindow("Asteroid Storm", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
			if (*renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				//Initialize SDL_Mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMusic(Mix_Music** music)
{
	//Loading success flag
	bool success = true;
	//Load music
	*music = Mix_LoadMUS("Kuvat/Waltz.mp3");
	if (*music == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

bool loadSoundEffect(Mix_Chunk** chunk)
{
	//Loading success flag
	bool success = true;
	//Load music
	*chunk = Mix_LoadWAV("Kuvat/vine_boom.wav");
	if (*chunk == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	return success;
}

bool loadFont(TTF_Font** font, int fontSize)
{
	//Loading success flag
	bool success = true;

	//Open the font
	*font = TTF_OpenFont("Kuvat/SansFont.ttf", fontSize);
	if (*font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}
void closeAll(SDL_Renderer** renderer, SDL_Window** window)
{
	//Destroy window    
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	*window = NULL;
	*renderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
void createAsteroidArray(SDL_Renderer** renderer, std::vector<Thing> asteroids)
{
	enum
	{
		PLAYER,
		ASTEROID
	};
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	int coinAxis, coinSide;
	//Default asteroid
	Thing asteroid(renderer, 0, 0, DOWN, ASTEROID);

	int asteroidCount = 0;
	while (asteroidCount < asteroids.size())
	{
		asteroids[asteroidCount++] = asteroid;
	}
}

int spawnAsteroid(std::vector<Thing> &asteroids, int asteroidCount, int frameCount)
{
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	if(frameCount%60==0)
	{
		int coinAxis, coinSide;
		srand(time(NULL));
		//Spawns on x or y axis
		coinAxis = rand() % 2;
		//Spawns on bottom or up, left or right side
		coinSide = rand() % 2;
		if (coinAxis)
		{
			if (coinSide)
			{
				//random x, bottom of the screen

				asteroids[asteroidCount++].spawn((rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), (SCREEN_HEIGHT), UP);
			}
			else
			{
				//Random x, top of the screen

				asteroids[asteroidCount++].spawn(300, 0 - asteroids[0].THING_HEIGHT, DOWN);

			}
		}
		else
		{
			if (coinSide)
			{
				//Right of the screen, random y 

				asteroids[asteroidCount++].spawn((SCREEN_WIDTH), (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), LEFT);
			}
			else
			{
				//Left of the screen random y

				asteroids[asteroidCount++].spawn(0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT);
			}
		}
	}
	//Return incremented asteroidCount
	return asteroidCount;
}

void moveAsteroids(std::vector<Thing>& asteroids, int asteroidCount)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		asteroids[i].move();
	}
}

void detectCollision(std::vector<Thing> &asteroids, Thing &player, int asteroidCount)
{
	//Check if player hits asteroids
	for (int i = 0; i < asteroidCount; i++)
	{
		if (!asteroids[i].getExploded()&&checkCollision(asteroids[i].getColliders(), player.getColliders()))
		{
			asteroids[i].explode();
			player.explode();

		}
	}
	for (int i = 0; i < asteroidCount; i++)
	{
		for (int j = 0; j < asteroidCount; j++)
		{
			if (j != i && !asteroids[i].getExploded() && checkCollision(asteroids[i].getColliders(), asteroids[j].getColliders()))
			{
				asteroids[i].explode();
				asteroids[j].explode();

			}
		}
	}
}

void checkAsteroidExplosion(std::vector<Thing> &asteroids, int asteroidCount, Mix_Chunk* vineBoom)
{
	enum
	{
		PLAYER,
		ASTEROID
	};
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	for (int i = 0; i < asteroidCount; i++)
	{
		if (asteroids[i].getExploded() && !asteroids[i].getSoundEffectPlaying())
		{
			Mix_PlayChannel(-1, vineBoom, 0);
			asteroids[i].setSoundEffectPlaying(true);
		}
		if (asteroids[i].getExploded() && (asteroids[i].getClip().x == 300))
		{
			int coinAxis, coinSide;
			//Spawns on x or y axis
			coinAxis = rand() % 2;
			//Spawns on bottom or up, left or right side
			coinSide = rand() % 2;

			if (coinAxis)
			{
				if (coinSide)
				{
					//random x, bottom of the screen
					asteroids[i].spawn(rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH), (SCREEN_HEIGHT), UP);
				}
				else
				{
					//Random x, top of the screen

					asteroids[i].spawn((rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), 0 - asteroids[0].THING_HEIGHT, DOWN);

				}
			}
			else
			{
				if (coinSide)
				{
					//Right of the screen, random y 

					asteroids[i].spawn(SCREEN_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), LEFT);

				}
				else
				{
					//Left of the screen random y
					asteroids[i].spawn(0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT);

				}
			}
			asteroids[i].setSoundEffectPlaying(false);
		}
	}
}

//Explodes player and restarts the game
void CheckPlayer(Thing& player, int &asteroidCount, int &highScore, Mix_Chunk* vineBoom)
{
	enum
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (player.getExploded() && !player.getSoundEffectPlaying())
	{
		Mix_PlayChannel(-1, vineBoom, 0);
		player.setSoundEffectPlaying(true);
	}
	if (player.getExploded() && (player.getClip().x == 300) &&
		!currentKeyStates[SDL_SCANCODE_UP] && !currentKeyStates[SDL_SCANCODE_DOWN] && !currentKeyStates[SDL_SCANCODE_LEFT] && !currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		player.setSoundEffectPlaying(false);
		player.spawn((SCREEN_WIDTH - player.THING_WIDTH) / 2, (SCREEN_HEIGHT - player.THING_HEIGHT) / 2, UP);

		if (asteroidCount > highScore)
		{
			highScore = asteroidCount;
		}
		asteroidCount = 0;
	}
}

void renderAsteroids(std::vector<Thing>& asteroids, int asteroidCount, LTexture& asteroidTexture, LTexture& explosionTexture)
{
	for (int i = 0; i < asteroidCount; i++)
	{
		if (!asteroids[i].getExploded())
		{
			asteroids[i].render(asteroidTexture);
		}
		else
		{
			asteroids[i].render(explosionTexture);
		}
			
	}
}
