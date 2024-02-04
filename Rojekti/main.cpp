#include "source.h"
//This game was made by Leo Kinnunen in 29.1.2024-3.2.2024
//The code is not perfect, especially considering the amount of stuff in the main function, but this game was finished in a hurry.



int main(int argc, char* args[])
{
	SDL_Renderer* renderer;
	SDL_Window* window;
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
	const int FRAME_RATE = 60;
	const int FRAME_TIME = 1000 / FRAME_RATE + 1;
	const int MAX_ASTEROIDS = 100;


	//Init SDL and window
	if (!init(&renderer, &window, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load Media

		LTexture backgroundTexture;
		LTexture playerTexture;
		LTexture explosionTexture;
		LTexture asteroidTexture;
		LTexture fpsTextTexture;
		LTexture asteroidTextTexture;
		LTexture highScoreTextTexture;
		TTF_Font* sansFont;
		Mix_Music* waltz = NULL;
		Mix_Chunk* vineBoom = NULL;
		loadMusic(&waltz);
		loadSoundEffect(&vineBoom);
		loadFont(&sansFont);
		backgroundTexture.loadFromFile(&renderer, "Kuvat/SpaceBackground.png");
		playerTexture.loadFromFile(&renderer, "Kuvat/Spaceship.png");
		explosionTexture.loadFromFile(&renderer, "Kuvat/ExplosionAnimation.png");
		asteroidTexture.loadFromFile(&renderer, "Kuvat/Asteroid.png");
		//Main loop flag
		bool quit = false;
		SDL_Rect backgroundDimensions;
		backgroundDimensions.w = SCREEN_WIDTH;
		backgroundDimensions.h = SCREEN_HEIGHT;
		Uint32 timer;
		SDL_Color textColor = {0, 0, 0};

		OpeningScreen startScreen(&renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
		if (startScreen.start == false)
		{
			quit = true;
		}
		Thing player(&renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, UP, PLAYER);
		
		std::vector<std::vector<SDL_Rect>> allAsteroidColliders(MAX_ASTEROIDS);
		std::vector<std::vector<SDL_Rect>> allColliders(MAX_ASTEROIDS+1);
		std::vector<Thing> asteroids(MAX_ASTEROIDS + 2, Thing(&renderer, (rand() % (SCREEN_WIDTH - 100)), 0, DOWN, ASTEROID));
		int asteroidCount = 1;
		asteroids[0] = Thing(&renderer, (rand() % (SCREEN_WIDTH - 100)), 0, DOWN, ASTEROID);;
		allAsteroidColliders.resize(asteroidCount);

		//put the asteroids colliders into the next free slots of allAsteroidColliders
		allAsteroidColliders[0] = asteroids[0].getColliders();
		Mix_PlayMusic(waltz, -1);

		Mix_Volume(-1, 5);

		int frameCount = 0;
		//Event handler
		SDL_Event e;

		std::stringstream fpsText;
		std::stringstream asteroidText;
		std::stringstream highScoreText;
		int highScore = 0;

		//While application is running
		while (!quit)
		{
			timer = SDL_GetTicks();

			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}

				if (!player.getExploded())
				{
					player.handleEvent(e);
				}


			}
			//Spawn asteroids every 60 frames
			if (frameCount % 60 == 0 && frameCount != 0&&asteroidCount < MAX_ASTEROIDS)
			{
				if (asteroidCount < MAX_ASTEROIDS)
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
							asteroids[asteroidCount] = Thing(&renderer, (rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), (SCREEN_HEIGHT), UP, ASTEROID);
							//Add size for this asteroids colliders
							allAsteroidColliders.resize(allAsteroidColliders.size()+1);														
							//put the asteroids colliders into the next free slots of allAsteroidColliders
							allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
							asteroidCount++;


						}
						else
						{
							//Random x, top of the screen
							asteroids[asteroidCount] = Thing(&renderer, (rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), 0-asteroids[0].THING_HEIGHT, DOWN, ASTEROID);
							//Add size for this asteroids colliders
							allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
							//put the asteroids colliders into the next free slots of allAsteroidColliders
							allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
							asteroidCount++;

						}
					}
					else
					{
						if (coinSide)
						{
							//Right of the screen, random y 
							asteroids[asteroidCount] = Thing(&renderer, (SCREEN_WIDTH), (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), LEFT, ASTEROID);
							//Add size for this asteroids colliders
							allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
							//put the asteroids colliders into the next free slots of allAsteroidColliders
							allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
							asteroidCount++;

						}
						else
						{
							//Left of the screen random y
							asteroids[asteroidCount] = Thing(&renderer, 0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT, ASTEROID);
							//Add size for this asteroids colliders
							allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
							//put the asteroids colliders into the next free slots of allAsteroidColliders
							allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
							asteroidCount++;

						}
					}
				}
			}
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
							asteroids[i].respawn(rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH), (SCREEN_HEIGHT), UP);
						}
						else
						{
							//Random x, top of the screen
							
							asteroids[i].respawn(rand() % (rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), 0 - asteroids[0].THING_HEIGHT, DOWN);

						}
					}
					else
					{
						if (coinSide)
						{
							//Right of the screen, random y 

							asteroids[i].respawn(SCREEN_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), LEFT);

						}
						else
						{
							//Left of the screen random y
							asteroids[asteroidCount] = Thing(&renderer, 0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT, ASTEROID);
							asteroids[i].respawn(0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT);

						}
					}
					asteroids[i].setSoundEffectPlaying(false);
				}
			}
			

			for (int i = 0; i < asteroidCount; i++)
			{
				allAsteroidColliders[i] = asteroids[i].getColliders();
			}
			


			for (int set = 0; set < asteroidCount; ++set)
			{
				//Puts all the other colliders to allColliders
				allColliders.resize(0);
				allColliders.resize(asteroidCount+size_t(1));
				allColliders[set] = player.getColliders();
				
				for (int i = 0; i < asteroidCount;i++)
				{
					if (i != set)
					{
						allColliders[i + size_t(1)] = asteroids[i].getColliders();
					}
				}
				

				asteroids[set].move(allColliders);
			}
			//Update allAsteroidColliders
			
			



			player.move(allAsteroidColliders);



			//Clear screen

			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderClear(renderer);


			//Background render
			backgroundTexture.render(&renderer, 0, 0, NULL, &backgroundDimensions);

			//Asteroids render
			//Mikä ihme tässä on ettei voi varastoida järkevästi noita textuureita
			for (int i = 0; i < asteroidCount; i++)
			{
				if (!asteroids[i].getExploded())
				{
					asteroids[i].render(&asteroidTexture);
				}
				else
				{
					asteroids[i].render(&explosionTexture);
				}
					
			}

			//Spaceship render
			player.render(&player.currentTexture);


			/*SDL_Rect currentHitboxPlayer, currentHitboxAsteroid;
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			for (int set = 0; set < player.getColliders().size(); ++set)
			{
				currentHitboxPlayer = player.getColliders()[set];
				SDL_RenderFillRect(renderer, &currentHitboxPlayer);
			}

			for (int set = 0; set < allAsteroidColliders.size(); ++set)
			{
				for (int i = 0; i < allAsteroidColliders[set].size(); i++)
				{
					currentHitboxAsteroid = allAsteroidColliders[set][i];
					SDL_RenderFillRect(renderer, &currentHitboxAsteroid);
				}		
			}*/
			
			//Update screen

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
				player.respawn((SCREEN_WIDTH-player.THING_WIDTH) / 2, (SCREEN_HEIGHT - player.THING_HEIGHT)/ 2, UP);
				allAsteroidColliders.resize(0);
				if (asteroidCount > highScore)
				{
					highScore = asteroidCount;
				}
				asteroidCount = 0;
				if (frameCount % 60 == 0 && frameCount != 0)
				{
					if (asteroidCount < MAX_ASTEROIDS)
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
								asteroids[asteroidCount].respawn((rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), (SCREEN_HEIGHT), UP);
								//Add size for this asteroids colliders
								allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
								//put the asteroids colliders into the next free slots of allAsteroidColliders
								allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
								asteroidCount++;


							}
							else
							{
								//Random x, top of the screen
								asteroids[asteroidCount].respawn((rand() % (SCREEN_WIDTH - asteroids[0].THING_WIDTH)), 0 - asteroids[0].THING_HEIGHT, DOWN);
								//Add size for this asteroids colliders
								allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
								//put the asteroids colliders into the next free slots of allAsteroidColliders
								allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
								asteroidCount++;

							}
						}
						else
						{
							if (coinSide)
							{
								//Right of the screen, random y 
								asteroids[asteroidCount].respawn((SCREEN_WIDTH), (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), LEFT);
								//Add size for this asteroids colliders
								allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
								//put the asteroids colliders into the next free slots of allAsteroidColliders
								allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
								asteroidCount++;

							}
							else
							{
								//Left of the screen random y
								asteroids[asteroidCount].respawn(0 - asteroids[0].THING_WIDTH, (rand() % (SCREEN_HEIGHT - asteroids[0].THING_HEIGHT)), RIGHT);
								//Add size for this asteroids colliders
								allAsteroidColliders.resize(allAsteroidColliders.size() + 1);
								//put the asteroids colliders into the next free slots of allAsteroidColliders
								allAsteroidColliders[asteroidCount] = asteroids[asteroidCount].getColliders();
								asteroidCount++;

							}
						}
					}
				}

			}
			asteroidText.str("");
			asteroidText << "Asteroids:" << asteroidCount;
			highScoreText.str("");
			highScoreText << "High Score:" << highScore;
			asteroidTextTexture.loadFromRenderedText(&renderer, &sansFont, asteroidText.str().c_str(), textColor);
			asteroidTextTexture.render(&renderer, 0, 14);
			highScoreTextTexture.loadFromRenderedText(&renderer, &sansFont, highScoreText.str().c_str(), textColor);
			highScoreTextTexture.render(&renderer, 0, 28);
			Uint32 frameTimer = SDL_GetTicks() - timer;
			if (frameTimer < FRAME_TIME)
			{
				SDL_Delay(FRAME_TIME - frameTimer);
			}
			frameTimer = SDL_GetTicks() - timer;
			frameCount++;
			fpsText.str("");
			fpsText << "FPS" << (1000/(frameTimer));
			fpsTextTexture.loadFromRenderedText(&renderer, &sansFont, fpsText.str().c_str(), textColor);
			fpsTextTexture.render(&renderer, 0, 0);
			SDL_RenderPresent(renderer);

		}
		closeAll(&renderer, &window);
	}


	return 0;
}