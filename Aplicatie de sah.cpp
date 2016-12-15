/*
git init
git remote add origin *link*
git add --all
git commit -m "init"
git push
https://www.atlassian.com/git/tutorials/
*/


#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
//#include "LTexture.h"

using namespace std;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect TableViewport;



class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(SDL_Rect port);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
	int *vect;
	int size;
};



LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	
}

LTexture::~LTexture()
{
	//Deallocate memory
	free();
}



bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
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


int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
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

void LTexture::render(SDL_Rect port)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { port.x, port.y, port.w, port.h };
	SDL_RenderSetViewport(gRenderer, &port);
	SDL_RenderCopy(gRenderer, this->mTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}


LTexture TableTexture;
LTexture PieceTexture[12];
SDL_Rect SQPort[64];
SDL_Rect WindowPort;

bool init();

void loadMedia();

void close();


enum SQ {
	A1, A2, A3, A4, A5, A6 ,A7, A8,
	B1, B2, B3, B4, B5, B6, B7, B8,
	C1, C2, C3, C4, C5, C6, C7, C8,
	D1, D2, D3, D4, D5, D6, D7, D8,
	E1, E2, E3, E4, E5, E6, E7, E8,
	F1, F2, F3, F4, F5, F6, F7, F8,
	G1, G2, G3, G4, G5, G6, G7, G8,
	H1, H2, H3, H4, H5, H6, H7, H8
};

enum PIECE
{
	pionalb, calalb, nebunalb, turnalb, damaalb, regealb,
	pionnegru, calnegru, nebunnegru, turnnegru, damanegru, regenegru
};

void setWindowPort()
{
	WindowPort.h = SCREEN_HEIGHT;
	WindowPort.w = SCREEN_WIDTH;
	WindowPort.x = WindowPort.y = 0;
}

void setSQPort()
{
	//pentru fiecare camp completez SQPort[SQ].h, .w, .x, .y
}

bool init()
{
	bool success = true;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

		
	gWindow = SDL_CreateWindow("Aplicatie de sah", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);
	
	setWindowPort();
	setSQPort();

	return success;
}

void loadMedia()
{
	TableTexture.loadFromFile("Images/tabla.png");
	cout << "tabla loaded successfully" << "\n";
	PieceTexture[pionalb].loadFromFile("Images/pionalb.png");
	cout << "pionalb loaded successfully" << "\n";
	PieceTexture[calalb].loadFromFile("Images/calalb.png");
	cout << "calalb loaded successfully" << "\n";
	PieceTexture[nebunalb].loadFromFile("Images/nebunalb.png");
	cout << "nebunalb loaded successfully" << "\n";
	PieceTexture[turnalb].loadFromFile("Images/turnalb.png");
	cout << "turnalb loaded successfully" << "\n";
	PieceTexture[damaalb].loadFromFile("Images/damaalb.png");
	cout << "damaalb loaded successfully" << "\n";
	PieceTexture[regealb].loadFromFile("Images/regealb.png");
	cout << "regealb loaded successfully" << "\n";
	PieceTexture[pionnegru].loadFromFile("Images/pionnegru.png");
	cout << "pionnegru loaded successfully" << "\n";
	PieceTexture[calnegru].loadFromFile("Images/calnegru.png");
	cout << "calnegru loaded successfully" << "\n";
	PieceTexture[nebunnegru].loadFromFile("Images/nebunnegru.png");
	cout << "nebunnegru loaded successfully" << "\n";
	PieceTexture[turnnegru].loadFromFile("Images/turnnegru.png");
	cout << "turnnegru loaded successfully" << "\n";
	PieceTexture[damanegru].loadFromFile("Images/damanegru.png");
	cout << "damanegru loaded successfully" << "\n";
	PieceTexture[regenegru].loadFromFile("Images/regenegru.png");
	cout << "regenegru loaded successfully" << "\n";
}

void close()
{
	TableTexture.free();
	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}

void Show_Table()
{
	SDL_RenderClear(gRenderer);
	TableViewport.x = SCREEN_HEIGHT / 10;
	TableViewport.y = SCREEN_HEIGHT / 10;
	TableViewport.w = SCREEN_HEIGHT * 8 / 10;
	TableViewport.h = SCREEN_HEIGHT * 8 / 10;
	SDL_RenderSetViewport(gRenderer, &TableViewport);
	TableTexture.render(TableViewport);
}

void Show_Piece(int piece, SDL_Rect port)
{
	SDL_RenderSetViewport(gRenderer, &port);
	PieceTexture[piece].render(port);
}

int main(int argc, char* args[])
{
	init();
	loadMedia();
	bool quit = false;
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	Show_Table();
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	while (!quit)
		{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				
				
				
				
				/*SDL_Rect port;
				port.y = SCREEN_HEIGHT * 8 / 10;
				port.x = SCREEN_HEIGHT * 3 / 10;
				port.h = SCREEN_HEIGHT/ 10;
				port.w = SCREEN_HEIGHT / 10;
				Show_Piece(calalb, port);
				*/
				SDL_Rect fillRect = { 169, 160, 50, 66 };
				//SDL_SetRenderDrawColor(gRenderer, 0, 0xFF, 0xFF, 0xFF);
				//SDL_RenderFillRect(gRenderer, &fillRect);
				Show_Piece(regenegru, fillRect);
				SDL_RenderPresent(gRenderer);
				
			}
		
	close();
	
	return 0;
}


// tabla incepe de la x=72p
//prima coloana incepe de la x=94p
//un camp e 66,5 x 66,5
//b7 SDL_Rect fillRect = { 169, 160, 50, 66 };