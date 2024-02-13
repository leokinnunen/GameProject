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
		Mix_Music* waltz = NULL;
		Mix_Chunk* vineBoom = NULL;
		loadMusic(&waltz);
		loadSoundEffect(&vineBoom);
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
		Uint32 frameTimer;
		SDL_Color textColor = { 0, 0, 0 };

		OpeningScreen startScreen(&renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
		if (startScreen.start == false)
		{
			quit = true;
		}
		Player player(&renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, UP);
		Projectile laser(&renderer);
		
		Thing asteroid(&renderer, 0, 0, DOWN, ASTEROID);
		std::vector<std::vector<SDL_Rect>> allAsteroidColliders(MAX_ASTEROIDS);
		std::vector<std::vector<SDL_Rect>> allColliders(MAX_ASTEROIDS + 1);
		//Create asteroid array
		std::vector<Thing> asteroids(MAX_ASTEROIDS + 2, Thing(&renderer, 0, 0, DOWN, ASTEROID));

		createAsteroidArray(&renderer, asteroids);
		Mix_VolumeMusic(64);
		Mix_PlayMusic(waltz, -1);

		Mix_Volume(-1, 3);


		//Event handler
		SDL_Event e;
		//Text variables
		InGameUI UI;

		int highScore = 0;
		int frameCount = 0;
		int asteroidCount = 0;
		int fps = 0;
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
			if (!player.getExploded())
			{
				asteroidCount = spawnAsteroid(asteroids, asteroidCount, frameCount);
			}



			moveAsteroids(asteroids, asteroidCount);
			player.move();
			shootingHandler(player, laser);

			detectCollision(asteroids, player, laser, asteroidCount);

			checkAsteroidExplosion(asteroids, asteroidCount, vineBoom);
			//Clear screen

			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			SDL_RenderClear(renderer);


			//Background render
			backgroundTexture.render(&renderer, 0, 0, NULL, &backgroundDimensions);

			//Asteroids render
			renderAsteroids(asteroids, asteroidCount, asteroidTexture, explosionTexture);

			//Spaceship render
			player.render(player.currentTexture);
			renderMoveLaser(player, laser);
			
			//Check if player exploded and restart the game
			CheckPlayer(player, asteroidCount, highScore, vineBoom);

			frameTimer = SDL_GetTicks() - timer;
			if (frameTimer < FRAME_TIME)
			{
				SDL_Delay(FRAME_TIME - frameTimer);
			}
			frameTimer = SDL_GetTicks() - timer;
			frameCount++;
			fps = (1000 / (frameTimer));
			UI.assignText(asteroidCount, highScore, fps);
			UI.render(&renderer);
			SDL_RenderPresent(renderer);

		}
		closeAll(&renderer, &window);
	}
	return 0;
}