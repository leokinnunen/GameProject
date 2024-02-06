#include "source.h"



LTexture::LTexture()
{
	//Init
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(SDL_Renderer** renderer, std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(*renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(SDL_Renderer** renderer, TTF_Font** font, std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(*font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(*renderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer** renderer, int x, int y, SDL_Rect* clip, SDL_Rect* destQuad, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (destQuad != NULL)
	{
		renderQuad.w = destQuad->w;
		renderQuad.h = destQuad->h;
	}

	//Render to screen
	SDL_RenderCopyEx(*renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}




Thing::Thing(SDL_Renderer** renderer, int x, int y, int direction, int type)
{


	mPosX = x;
	mPosY = y;

	mSpawnPointX = x;
	mSpawnPointY = y;

	mType = type;

	mVelX = 0;
	mVelY = 0;

	thingRenderer = *renderer;

	mPreviousPosX = x;
	mPreviousPosY = y;

	mRotation = 0.0;
	mDirection = direction;

	mPlayingSoundEffect = false;

	mClip.x = 0;
	mClip.y = 0;
	mClip.w = THING_WIDTH;
	mClip.h = THING_HEIGHT;

	hasExploded = false;
	if (type == ASTEROID)
	{
		THING_VEL = (2 + rand() % MAX_SPEED - 1);
		//Create the necessary SDL_Rects
		mColliders.resize(3);

		switch (direction)
		{
		case(LEFT):
			mVelX = -THING_VEL;
			mRotation = -90.0;
			break;
		case(RIGHT):
			mVelX = THING_VEL;
			mRotation = 90.0;
			break;
		case(UP):
			mVelY = -THING_VEL;
			mRotation = 0.0;
			break;
		case(DOWN):
			mVelY = THING_VEL;
			mRotation = 180.0;
			break;
		}
		mDirection = direction;


		asteroidTexture.loadFromFile(&thingRenderer, "Kuvat/Asteroid.png");
		explosionTexture.loadFromFile(&thingRenderer, "Kuvat/ExplosionAnimation.png");
		currentTexture = asteroidTexture;

		mColliders[0].w = 48;
		mColliders[0].h = 25;
		mColliders[0].x = x + 23;
		mColliders[0].y = y + 11;

		mColliders[1].w = 51;
		mColliders[1].h = 37;
		mColliders[1].x = x + 33;
		mColliders[1].y = y + 36;

		mColliders[2].w = 38;
		mColliders[2].h = 9;
		mColliders[2].x = x + 44;
		mColliders[2].y = y + 73;


		mRotation = 0.0;
	}
	else if (type == PLAYER)
	{
		THING_VEL = 10;
		//Initialize the offsets
		mPosX = x;
		mPosY = y;

		mPreviousPosX = x;
		mPreviousPosY = y;
		//Create the necessary SDL_Rects
		mColliders.resize(5);

		playerTexture.loadFromFile(&thingRenderer, "Kuvat/Spaceship.png");
		explosionTexture.loadFromFile(&thingRenderer, "Kuvat/ExplosionAnimation.png");
		currentTexture = playerTexture;

		mVelX = 0;
		mVelY = 0;

		mDirection = UP;
		mRotation = 0.0;

		//Set collision box dimension

		mColliders[0].w = 10;
		mColliders[0].h = 11;
		mColliders[0].x = x + 25;
		mColliders[0].y = y + 23;

		mColliders[1].w = 7;
		mColliders[1].h = 5;
		mColliders[1].x = x + 64;
		mColliders[1].y = y + 29;

		mColliders[2].w = 46;
		mColliders[2].h = 6;
		mColliders[2].x = x + 25;
		mColliders[2].y = y + 34;

		mColliders[3].w = 67;
		mColliders[3].h = 19;
		mColliders[3].x = x + 17;
		mColliders[3].y = y + 40;

		mColliders[4].w = 37;
		mColliders[4].h = 6;
		mColliders[4].x = x + 30;
		mColliders[4].y = y + 59;

		mVelX = 0;
		mVelY = 0;
	}
}

Thing::~Thing()
{
	mPosX = 0;
	mPosY = 0;
	mPreviousPosX = 0;
	mPreviousPosY = 0;
	mColliders.resize(0);
	mVelX = 0;
	mVelY = 0;
	mDirection = 0;
	mFrameCounter = 0;
	playerTexture.free();
	asteroidTexture.free();
	explosionTexture.free();

}

void Thing::shiftColliders()
{
	if (mType == ASTEROID)
	{
		//First put the Colliders in the up position
		mColliders[0].w = 48;
		mColliders[0].h = 25;
		mColliders[0].x = mPreviousPosX + 23;
		mColliders[0].y = mPreviousPosY + 11;

		mColliders[1].w = 51;
		mColliders[1].h = 37;
		mColliders[1].x = mPreviousPosX + 33;
		mColliders[1].y = mPreviousPosY + 36;

		mColliders[2].w = 38;
		mColliders[2].h = 9;
		mColliders[2].x = mPreviousPosX + 44;
		mColliders[2].y = mPreviousPosY + 73;
	}
	if (mType == PLAYER)
	{
		mColliders[0].w = 10;
		mColliders[0].h = 11;
		mColliders[0].x = mPreviousPosX + 25;
		mColliders[0].y = mPreviousPosY + 23;

		mColliders[1].w = 7;
		mColliders[1].h = 5;
		mColliders[1].x = mPreviousPosX + 64;
		mColliders[1].y = mPreviousPosY + 29;

		mColliders[2].w = 46;
		mColliders[2].h = 6;
		mColliders[2].x = mPreviousPosX + 25;
		mColliders[2].y = mPreviousPosY + 34;

		mColliders[3].w = 67;
		mColliders[3].h = 19;
		mColliders[3].x = mPreviousPosX + 17;
		mColliders[3].y = mPreviousPosY + 40;

		mColliders[4].w = 37;
		mColliders[4].h = 6;
		mColliders[4].x = mPreviousPosX + 30;
		mColliders[4].y = mPreviousPosY + 59;
	}
	SDL_Rect newHitbox;
	//K‰‰nn‰ Hitboxit oikein
	if (mDirection == LEFT)
	{
		mRotation = -90.0;
		for (int set = 0; set < mColliders.size(); ++set)
		{
			newHitbox.x = mPosX + mColliders[set].y - mPreviousPosY;
			newHitbox.y = mPosY + THING_WIDTH - (mColliders[set].x - mPreviousPosX) - mColliders[set].w;
			newHitbox.w = mColliders[set].h;
			newHitbox.h = mColliders[set].w;
			mColliders[set].x = newHitbox.x;
			mColliders[set].y = newHitbox.y;
			mColliders[set].w = newHitbox.w;
			mColliders[set].h = newHitbox.h;
		}
	}

	else if (mDirection == RIGHT)
	{
		mRotation = 90.0;
		for (int set = 0; set < mColliders.size(); ++set)
		{
			newHitbox.x = mPosX + THING_HEIGHT - (mColliders[set].y - mPreviousPosY) - mColliders[set].h;
			newHitbox.y = mPosY + mColliders[set].x - mPreviousPosX;
			newHitbox.w = mColliders[set].h;
			newHitbox.h = mColliders[set].w;
			mColliders[set].x = newHitbox.x;
			mColliders[set].y = newHitbox.y;
			mColliders[set].w = newHitbox.w;
			mColliders[set].h = newHitbox.h;
		}
	}

	else if (mDirection == DOWN)
	{
		mRotation = 180.0;
		for (int set = 0; set < mColliders.size(); ++set)
		{
			newHitbox.x = mPosX + THING_WIDTH - (mColliders[set].x - mPreviousPosX) - mColliders[set].w;
			newHitbox.y = mPosY + THING_HEIGHT - (mColliders[set].y - mPreviousPosY) - mColliders[set].h;
			newHitbox.w = mColliders[set].w;
			newHitbox.h = mColliders[set].h;
			mColliders[set].x = newHitbox.x;
			mColliders[set].y = newHitbox.y;
			mColliders[set].w = newHitbox.w;
			mColliders[set].h = newHitbox.h;
		}
	}

	else
	{
		mRotation = 0.0;
		for (int set = 0; set < mColliders.size(); ++set)
		{
			mColliders[set].x = mPosX + mColliders[set].x - mPreviousPosX;
			mColliders[set].y = mPosY + mColliders[set].y - mPreviousPosY;
		}
	}
	mPreviousPosX = mPosX;
	mPreviousPosY = mPosY;
}

void Thing::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mVelX -= THING_VEL;
			mDirection = LEFT;
			mRotation = -90.0;
			break;
		case SDLK_RIGHT:
			mVelX += THING_VEL;
			mDirection = RIGHT;
			mRotation = 90.0;
			break;
		case SDLK_UP:
			mVelY -= THING_VEL;
			mDirection = UP;
			mRotation = 0.0;
			break;
		case SDLK_DOWN:
			mVelY += THING_VEL;
			mDirection = DOWN;
			mRotation = 180.0;
			break;

		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mVelX += THING_VEL;
			if (mVelX > 0)
			{
				mDirection = RIGHT;
				mRotation = 90.0;
			}
			else if (mVelY > 0)
			{
				mDirection = DOWN;
				mRotation = 180.0;
			}
			else if (mVelY < 0)
			{
				mDirection = UP;
				mRotation = 0;
			}
			break;
		case SDLK_RIGHT:
			mVelX -= THING_VEL;
			if (mVelX < 0)
			{
				mDirection = LEFT;
				mRotation = -90.0;
			}
			else if (mVelY > 0)
			{
				mDirection = DOWN;
				mRotation = 180.0;
			}
			else if (mVelY < 0)
			{
				mDirection = UP;
				mRotation = 0;
			}
			break;
		case SDLK_UP:
			mVelY += THING_VEL;
			if (mVelY > 0)
			{
				mDirection = DOWN;
				mRotation = 180.0;
			}
			else if (mVelX > 0)
			{
				mDirection = RIGHT;
				mRotation = 90.0;
			}
			else if (mVelX < 0)
			{
				mDirection = LEFT;
				mRotation = -90.0;
			}
			break;
		case SDLK_DOWN:
			mVelY -= THING_VEL;
			if (mVelY < 0)
			{
				mDirection = UP;
				mRotation = 0.0;
			}
			else if (mVelX > 0)
			{
				mDirection = RIGHT;
				mRotation = 90.0;
			}
			else if (mVelX < 0)
			{
				mDirection = LEFT;
				mRotation = -90.0;
			}
			break;

		}
	}
}

std::vector<SDL_Rect>& Thing::getColliders()
{
	return mColliders;
}

void Thing::move()
{
	if (hasExploded && mClip.x < 200)
	{
		if (mFrameCounter == 10)
		{
			explode();
			mFrameCounter = 0;
		}
		else
		{
			mFrameCounter++;
		}
	}
	else if (hasExploded && mClip.x == 200)
	{
		if (mFrameCounter++ == 30)
		{
			mFrameCounter = 0;
			mClip.x += 100;
		}
	}
	if (!hasExploded)
	{
		//Move the dot left or right
		mPosX += mVelX;
		shiftColliders();
		//If the dot went too far to the left or right
		if ((mPosX + THING_WIDTH < 0) && !hasExploded)
		{
			mPosX = SCREEN_WIDTH;
			if (mType == ASTEROID)
			{
				srand(time(NULL));
				mVelX = -2 + rand() % MAX_SPEED - 1;
			}
		}
		else if ((mPosX > SCREEN_WIDTH) && !hasExploded)
		{
			mPosX = -THING_WIDTH;
			if (mType == ASTEROID)
			{
				srand(time(NULL));
				mVelX = 2 + rand() % MAX_SPEED - 1;
			}
		}
		

		//Move the dot up or down
		mPosY += mVelY;
		shiftColliders();

		if ((mPosY + THING_HEIGHT < 0) && !hasExploded)
		{
			mPosY = SCREEN_HEIGHT;
			if (mType == ASTEROID)
			{
				srand(time(NULL));
				mVelY = -2 + rand() % MAX_SPEED - 1;
			}
		}
		else if ((mPosY > SCREEN_HEIGHT) && !hasExploded)
		{
			mPosY = -THING_HEIGHT;
			if (mType == ASTEROID)
			{
				srand(time(NULL));
				mVelY = 2 + rand() % MAX_SPEED - 1;
			}
		}
		

	}
}

void Thing::render(LTexture &texture)
{
	//Show the dot
	SDL_Rect destQuad;
	destQuad.x = mPosX;
	destQuad.y = mPosY;
	destQuad.w = THING_WIDTH;
	destQuad.h = THING_HEIGHT;
	texture.render(&thingRenderer, mPosX, mPosY, &mClip, &destQuad, getRotation());
}

void Thing::explode()
{
	if (hasExploded == false)
	{
		mDirection = UP;
		mRotation = 0.0;
		mVelX = 0;
		mVelY = 0;
		mClip.w = 100;
		mClip.h = 100;
		mClip.x = 0;
		mClip.y = 0;
		mFrameCounter = 0;
		hasExploded = true;
		currentTexture = explosionTexture;
	}
	else
	{
		mClip.x += 100;
	}
	if (mClip.x >= 200)
	{
		for (int i = 0; i < mColliders.size(); i++)
		{
			mColliders[i] = { 0, 0, 0, 0 };
		}
		
	}
}

int Thing::getPosX()
{
	return mPosX;
}

int Thing::getPosY()
{
	return mPosY;
}

int Thing::getDirection()
{
	return mDirection;
}

double Thing::getRotation()
{
	return mRotation;
}

bool Thing::getExploded()
{
	return hasExploded;
}

SDL_Rect Thing::getClip()
{
	return mClip;
}

int Thing::getSpawnPointX()
{
	return mSpawnPointX;
}

int Thing::getSpawnPointY()
{
	return mSpawnPointY;
}

bool Thing::getSoundEffectPlaying()
{
	return mPlayingSoundEffect;
}

void Thing::setSoundEffectPlaying(bool a)
{
	mPlayingSoundEffect = a;
}

void Thing::spawn(int x, int y, int direction)
{
	mPosX = x;
	mPosY = y;

	mVelX = 0;
	mVelY = 0;

	if (mType == PLAYER)
	{
		currentTexture = playerTexture;
	}
	else if (mType == ASTEROID)
	{
		mDirection = direction;
		currentTexture = asteroidTexture;
		switch (direction)
		{
		case(LEFT):
			mVelX = -THING_VEL;
			mRotation = -90.0;
			break;
		case(RIGHT):
			mVelX = THING_VEL;
			mRotation = 90.0;
			break;
		case(UP):
			mVelY = -THING_VEL;
			mRotation = 0.0;
			break;
		case(DOWN):
			mVelY = THING_VEL;
			mRotation = 180.0;
			break;
		}
	}
	mClip.x = 0;
	hasExploded = false;
}

void Thing::respawn(int x, int y, int direction)
{
	mPosX = x;
	mPosY = y;

	mVelX = 0;
	mVelY = 0;

	if (mType == PLAYER)
	{
		currentTexture = playerTexture;
	}
	else if (mType == ASTEROID)
	{
		mDirection = direction;
		currentTexture = asteroidTexture;
		switch (direction)
		{
		case(LEFT):
			mVelX = -THING_VEL;
			mRotation = -90.0;
			break;
		case(RIGHT):
			mVelX = THING_VEL;
			mRotation = 90.0;
			break;
		case(UP):
			mVelY = -THING_VEL;
			mRotation = 0.0;
			break;
		case(DOWN):
			mVelY = THING_VEL;
			mRotation = 180.0;
			break;
		}
	}
	mClip.x = 0;
	hasExploded = false;

}



OpeningScreen::OpeningScreen(SDL_Renderer** renderer, const int screenWidth, const int screenHeight)
{

	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;
	gameNameText = "Asteroid Storm";
	infoText = "Game, music and pixel art by Leo Kinnunen";
	instructionText = "Dodge the asteroids. Press space to start!";
	backgroundTexture.loadFromFile(renderer, "Kuvat/SpaceBackground.png");
	backgroundTexture.setAlpha(0xaa);
	loadFont(&sansFont, 40);
	gameNameTexture.loadFromRenderedText(renderer, &sansFont, gameNameText.c_str(), textColor);
	infoTexture.loadFromRenderedText(renderer, &sansFont, infoText.c_str(), textColor);
	instructionTexture.loadFromRenderedText(renderer, &sansFont, instructionText.c_str(), textColor);
	//Quit and start flag
	bool quit = false;
	bool startFlag = false;
	//Event handler
	SDL_Event e;
	while (!quit && !startFlag)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
			{
				startFlag = true;
			}
		}
		render(renderer);
	}
	start = startFlag;

}

void OpeningScreen::render(SDL_Renderer** renderer)
{
	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(*renderer);
	SDL_Rect backgroundDimensions{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	backgroundTexture.render(renderer, 0, 0, NULL, &backgroundDimensions);
	gameNameTexture.render(renderer, (SCREEN_WIDTH - gameNameTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - gameNameTexture.getHeight() - infoTexture.getHeight() - instructionTexture.getHeight());
	infoTexture.render(renderer, (SCREEN_WIDTH - infoTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - infoTexture.getHeight() - instructionTexture.getHeight());
	instructionTexture.render(renderer, (SCREEN_WIDTH - instructionTexture.getWidth()) / 2, SCREEN_HEIGHT / 2 - instructionTexture.getHeight());
	SDL_RenderPresent(*renderer);
}

InGameUI::InGameUI()
{
	asteroidText.str("");
	asteroidText << "Asteroids: "<<0;
	highScoreText.str("");
	highScoreText << "High Score: " << 0;
	fpsText.str("");
	fpsText << "FPS: "<<0;
	loadFont(&sansFont, 14);
}

void InGameUI::assignText(int asteroidCount, int highScore, int fps)
{
	asteroidText.str("");
	asteroidText << "Asteroids: " << asteroidCount;
	highScoreText.str("");
	highScoreText << "High Score: " << highScore;
	fpsText.str("");
	fpsText << "FPS: " << fps;
}

void InGameUI::render(SDL_Renderer ** renderer)
{
	asteroidTextTexture.loadFromRenderedText(renderer, &sansFont, asteroidText.str().c_str(), textColor);
	highScoreTextTexture.loadFromRenderedText(renderer, &sansFont, highScoreText.str().c_str(), textColor);
	fpsTextTexture.loadFromRenderedText(renderer, &sansFont, fpsText.str().c_str(), textColor);

	fpsTextTexture.render(renderer, 0, 0);
	asteroidTextTexture.render(renderer, 0, 14);
	highScoreTextTexture.render(renderer, 0, 28);
}