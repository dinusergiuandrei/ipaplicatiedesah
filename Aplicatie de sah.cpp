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

int Table[10][10];

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect TableViewport;

void setSQPort();


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
	free();
}



bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;
	SDL_FreeSurface(loadedSurface);
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
LTexture BackgroundTexture;
SDL_Rect SQPort[64];
SDL_Rect WindowPort;

bool init();

void loadMedia();

void close();


enum SQ {

	A8, B8, C8, D8, E8, F8, G8, H8,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A1, B1, C1, D1, E1, F1, G1, H1

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
	cout<<BackgroundTexture.loadFromFile("Images/background3.png")<<"\n";
	cout << "background loaded successfully" << "\n";
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

void Show_Background()
{
	SDL_RenderClear(gRenderer);
	//SDL_RenderSetViewport(gRenderer, &WindowPort);
	TableViewport.x = SCREEN_HEIGHT / 10;
	TableViewport.y = SCREEN_HEIGHT / 10;
	TableViewport.w = SCREEN_HEIGHT * 8 / 10;
	TableViewport.h = SCREEN_HEIGHT * 8 / 10;
	SDL_RenderSetViewport(gRenderer, &TableViewport);
	BackgroundTexture.render(WindowPort);
}

void Show_Board()
{
	//SDL_RenderClear(gRenderer);
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

void setSQPort()
{
	int i, dif=0, l, c;
	for (i = 0; i < 64; i++)
	{
		SQPort[i].h = 66+dif;
		SQPort[i].w = 66+dif;
		dif = 1 - dif;
	}
	dif = 0;
	for (i = 0; i < 64; i++)
	{
		SQPort[i].x = 94 + (i%8) * (66);
		SQPort[i].y = 94 + (i/8) * (66);
		dif = 1 - dif;
	}
}

void put_piece(int p, int sq)
{
	Table[sq / 8][sq % 8] = p;
}

int move(int sq1, int sq2)
{
	int captured;
	captured = Table[sq2 / 8][sq2 % 8];
	put_piece(Table[sq1 / 8][sq1 % 8], sq2);
	return captured;
}

void Init_Table()
{
	int i;
	for (i = A8; i <= H1; i++)
		Table[i / 8][i % 8] = -1;
	for(i=A7; i<=H7; i++)
		put_piece(pionnegru, i);
	for (i = A2; i <= H2; i++)
		put_piece(pionalb, i);
	put_piece(calalb, B1);
	put_piece(calalb, G1);
	put_piece(calnegru, B8);
	put_piece(calnegru, G8);
	put_piece(nebunalb, C1);
	put_piece(nebunalb, F1);
	put_piece(nebunnegru, C8);
	put_piece(nebunnegru, F8);
	put_piece(turnalb, A1);
	put_piece(turnalb, H1);
	put_piece(turnnegru, A8);
	put_piece(turnnegru, H8);
	put_piece(damaalb, D1);
	put_piece(damanegru, D8);
	put_piece(regealb, E1);
	put_piece(regenegru, E8);
}

void Show_Table()
{
	int i;
	for (i = A8; i <= H1; i++)
		if (Table[i / 8][i % 8] >= 0)
			Show_Piece(Table[i / 8][i % 8], SQPort[i]);
}

int main(int argc, char* args[])
{
	int i, j;
	init();
	loadMedia();
	bool quit = false;
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	//Show_Background();
	Show_Board();
	setSQPort();
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	Init_Table();
	Show_Table();
	while (!quit)
		{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}
				
			}
	
	
	close();
	
	return 0;
}


// tabla incepe de la x=72p
//prima coloana incepe de la x=94p
//un camp e 66 x 66
//b7 SDL_Rect fillRect = { 169, 160, 50, 66 };