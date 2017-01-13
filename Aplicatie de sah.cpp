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
#include "SDL_mixer.h"
#include <stdio.h>
#include <string>

using namespace std;

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect TableViewport, Button1, Button2, Button3, Button4;

SDL_Rect BackButton, UndoButton;

SDL_Surface *screen;

class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void render(SDL_Rect port);
	int getWidth();
	int getHeight();
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

LTexture::LTexture()
{
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
	SDL_Rect renderQuad = { port.x, port.y, port.w, port.h };
	SDL_RenderSetViewport(gRenderer, &port);

	SDL_RenderCopy(gRenderer, this->mTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}

SDL_Texture* loadTexture(string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}


LTexture TableTexture;
LTexture PieceTexture[12];
LTexture BackgroundTexture;
LTexture Main_Menu_Background;
LTexture Player_1_Image;
LTexture Player_2_Image;
LTexture Computer_Image;
LTexture Undo_Image;
LTexture Albulacastigat_Image;
LTexture Negrulacastigat_Image;
LTexture Back_Image;
LTexture SelectColor_Image;
LTexture BackgroundComputerWhite;
LTexture BackgroundComputerBlack;
LTexture SelectDifficulty;
LTexture SelectDifficultyComputer1;
LTexture SelectDifficultyComputer2;
SDL_Rect SQPort[64];
SDL_Rect WindowPort;

Mix_Chunk *MoveSound = NULL;
Mix_Chunk *NEWGAME = NULL;
Mix_Chunk *CAPHIT1 = NULL;
Mix_Chunk *CAPHIT2 = NULL;
Mix_Chunk *CAPHIT3 = NULL;
Mix_Chunk *CAPTURE = NULL;
Mix_Chunk *CAPTURE2 = NULL;
Mix_Chunk *CAPTURE3 = NULL;
Mix_Chunk *MOVE2 = NULL;
Mix_Chunk *MOVE3 = NULL;
Mix_Chunk *MOVE4 = NULL;
Mix_Chunk *Tick = NULL;

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int Table[10][10], WhiteControl[10][10], BlackControl[10][10];
int SQ_Value_White[64] = { 1,1,1,1,1,1,1,1, 2,1,2,2,2,2,2,2, 1,1,3,3,3,3,1,1, 1,2,3,3,3,3,2,1, 1,1,2,2,2,2,1,1, 1,1,1,1,1,1,1,1, 0, 0, 0, 0, 0, 0, 0, 0 };
int Line_Value_White[10] = { 1, 3, 2, 1, 1, 1, 1, 0, 0, 0 };
int Line_Value_Black[10] = { 0, -1, -1, -1, -1, -2, -3, -1, 0, 0 };
int Piece_Value[20], SQ_Value_Black[64];
int dl[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
int dc[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };
int dcl[8] = { -2, -1, 1, 2, 2, 1, -1, -2 };
int dcc[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };


void setSQPort();

void PresentTableControl(int T[10][10]);

int IsAttackedByWhite(int T[10][10], int sq);

int IsAttackedByBlack(int T[10][10], int sq);

int Eval(int T[10][10]);

void MoveGenerator(int T[10][10], int tomove, int &sq1, int &sq2, int &minscore, int &maxscore, int depth);

void MainMenu();

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

struct Partida
{
	int CurrentTable[10][10];
	int sq1, sq2;
	int tomove;
	Partida *prec, *urm;
} *CurrentPosition, *InitialPosition, *LastPosition;

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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);

	setWindowPort();
	setSQPort();

	
	return success;
}

void loadMedia()
{
	MoveSound = Mix_LoadWAV("Sounds/MoveSound.wav");
	CAPHIT1= Mix_LoadWAV("Sounds/CAPHIT1.wav");
	CAPHIT2= Mix_LoadWAV("Sounds/CAPHIT2.wav");
	CAPHIT3= Mix_LoadWAV("Sounds/CAPHIT3.wav");
	CAPTURE= Mix_LoadWAV("Sounds/CAPTURE.wav");
	CAPTURE2= Mix_LoadWAV("Sounds/CAPTURE2.wav");
	CAPTURE3= Mix_LoadWAV("Sounds/CAPTURE3.wav");
	MOVE2= Mix_LoadWAV("Sounds/MOVE2.wav");
	MOVE3= Mix_LoadWAV("Sounds/MOVE3.wav");
	MOVE4= Mix_LoadWAV("Sounds/MOVE4.wav");
	NEWGAME= Mix_LoadWAV("Sounds/NEWGAME.wav");
	Tick= Mix_LoadWAV("Sounds/Tick.wav");

	BackgroundTexture.loadFromFile("Images/brownbg.png");
	Main_Menu_Background.loadFromFile("Images/brownmainmenu.png");
	TableTexture.loadFromFile("Images/tabla.png");

	Albulacastigat_Image.loadFromFile("Images/albulacastigat.png");
	Negrulacastigat_Image.loadFromFile("Images/negrulacastigat.png");
	SelectColor_Image.loadFromFile("Images/selectcolor.png");
	BackgroundComputerWhite.loadFromFile("Images/brownbgvscomp.png");
	BackgroundComputerBlack.loadFromFile("Images/brownbgvscomp2.png");
	SelectDifficulty.loadFromFile("Images/selectdiff.png");
	SelectDifficultyComputer1.loadFromFile("Images/selectdiff1.png");
	SelectDifficultyComputer2.loadFromFile("Images/selectdiff2.png");

	PieceTexture[pionalb].loadFromFile("Images/pionalb.png");
	PieceTexture[calalb].loadFromFile("Images/calalb.png");
	PieceTexture[nebunalb].loadFromFile("Images/nebunalb.png");
	PieceTexture[turnalb].loadFromFile("Images/turnalb.png");
	PieceTexture[damaalb].loadFromFile("Images/damaalb.png");
	PieceTexture[regealb].loadFromFile("Images/regealb.png");
	PieceTexture[pionnegru].loadFromFile("Images/pionnegru.png");
	PieceTexture[calnegru].loadFromFile("Images/calnegru.png");
	PieceTexture[nebunnegru].loadFromFile("Images/nebunnegru.png");
	PieceTexture[turnnegru].loadFromFile("Images/turnnegru.png");
	PieceTexture[damanegru].loadFromFile("Images/damanegru.png");
	PieceTexture[regenegru].loadFromFile("Images/regenegru.png");

}

void close()
{
	TableTexture.free();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	Mix_FreeChunk(MoveSound);
	MoveSound = NULL;
	IMG_Quit();
	SDL_Quit();
}


void Show_Background()
{
	TableViewport.x = SCREEN_HEIGHT / 10;
	TableViewport.y = SCREEN_HEIGHT / 10;
	TableViewport.w = SCREEN_HEIGHT * 8 / 10;
	TableViewport.h = SCREEN_HEIGHT * 8 / 10;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	BackgroundTexture.render(WindowPort);
}

void Show_Board()
{
	TableViewport.x = SCREEN_HEIGHT / 10;
	TableViewport.y = SCREEN_HEIGHT / 10;
	TableViewport.w = SCREEN_HEIGHT * 8 / 10;
	TableViewport.h = SCREEN_HEIGHT * 8 / 10;
	SDL_RenderSetViewport(gRenderer, &TableViewport);
	TableTexture.render(TableViewport);
}

void Show_White_Win()
{
	SDL_Rect CurrentPort;
	CurrentPort.x = SQPort[25].x+SQPort[25].w/2;
	CurrentPort.y = SQPort[25].y+SQPort[25].h/2 ;
	CurrentPort.w = 5*SQPort[0].w;
	CurrentPort.h = SQPort[0].h;
	SDL_RenderSetViewport(gRenderer, &CurrentPort);
	Albulacastigat_Image.render(CurrentPort);
}

void Show_Black_Win()
{
	SDL_Rect CurrentPort;
	CurrentPort.x = SQPort[25].x + SQPort[25].w / 2;
	CurrentPort.y = SQPort[25].y + SQPort[25].h / 2;
	CurrentPort.w = 5 * SQPort[0].w;
	CurrentPort.h = SQPort[0].h;
	SDL_RenderSetViewport(gRenderer, &CurrentPort);
	Negrulacastigat_Image.render(CurrentPort);
}

void Show_Black_To_Move()
{
	SDL_Rect current_rect;
	current_rect.x = SQPort[63].x + 300;
	current_rect.y = SQPort[63].y + SQPort[63].h / 2;
	current_rect.h = 20;
	current_rect.w = 50;
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &current_rect);
	SDL_RenderPresent(gRenderer);
}

void Show_White_To_Move()
{
	
}

void Show_Piece(int piece, int SQ)
{
	SDL_Rect port;
	port = SQPort[SQ];
	SDL_RenderSetViewport(gRenderer, &port);
	PieceTexture[piece].render(port);
}

void setSQPort()
{
	int i, dif = 0, l, c;
	for (i = 0; i < 64; i++)
	{
		SQPort[i].h = 66 + dif;
		SQPort[i].w = 66 + dif;
		dif = 1 - dif;
	}
	dif = 0;
	for (i = 0; i < 64; i++)
	{
		SQPort[i].x = 94 + (i % 8) * (66);
		SQPort[i].y = 94 + (i / 8) * (66);
		dif = 1 - dif;
	}
}

void put_piece(int T[10][10], int p, int sq)
{
	T[sq / 8][sq % 8] = p;
}

int move(int T[10][10], int sq1, int sq2)
{
	int captured, piece;
	int l1, l2, c1, c2;
	l1 = sq1 / 8;
	c1 = sq1 % 8;
	l2 = sq2 / 8;
	c2 = sq2 % 8;
	if (l1 >= 0 && l1 < 8 && c1 >= 0 && c1 < 8 && l2 >= 0 && l2 < 8 && c2 >= 0 && c2 < 8)
		if(sq1!=sq2)
	{
		

		piece = T[l1][c1];
		captured = T[l2][c2];
		
		if (T[l1][c1] == regealb && l1 == 7)
			if (c2 - c1 == 2)
			{
				T[7][7] = -1;
				T[7][5] = turnalb;
			}
			else
				if (sq1 % 8 - sq2 % 8 == 2)
				{
					T[7][0] = -1;
					T[7][3] = turnalb;
				}
		if (T[sq1 / 8][sq1 % 8] == regenegru&&l1 == 0)
			if (sq2 % 8 - sq1 % 8 == 2)
			{
				T[0][7] = -1;
				T[0][5] = turnnegru;
			}
			else
				if (sq1 % 8 - sq2 % 8 == 2)
				{
					T[0][0] = -1;
					T[0][3] = turnnegru;
				}
		captured = T[sq2 / 8][sq2 % 8];

		int enpassant = 0;
		if (piece == pionalb)
		{
			if (c2 != c1)
				if (T[l2][c2] == -1)
				{
					T[l2 + 1][c2] = -1;
					enpassant = 1;
				}
			if (l2 == 0)
				T[l2][c2] = damaalb;
		}

		if (piece == pionnegru)
		{
			if (c2 != c1)
				if (T[l2][c2] == -1)
				{
					enpassant = 1;
					T[l2 - 1][c2] = -1;
				}
			if (l2 == 7)
				T[l2][c2] = damanegru;
		}
		put_piece(T, T[sq1 / 8][sq1 % 8], sq2);
		T[sq1 / 8][sq1 % 8] = -1;
		if (piece == pionalb)
		{
			if (l2 == 0)
				T[l2][c2] = damaalb;
		}
		if (piece == pionnegru)
		{
			if (l2 == 7)
				T[l2][c2] = damanegru;
		}
		return captured;
	}
	return 0;
}

void makemove(int T[10][10], int sq1, int sq2)
{
	int captured, piece;
	captured = T[sq2 / 8][sq2 % 8];
	piece = T[sq1 / 8][sq1 % 8];
	move(T, sq1, sq2);

	if (captured == -1)
	{
		if (piece % 4 == 0)
			Mix_PlayChannel(-1, MoveSound, 0);
		if (piece % 4 == 1)
			Mix_PlayChannel(-1, MOVE2, 0);
		if (piece % 4 == 2)
			Mix_PlayChannel(-1, MOVE3, 0);
		if (piece % 4 == 3)
			Mix_PlayChannel(-1, MOVE4, 0);
	}
	else
	{
		if (piece % 3 == 0)
			Mix_PlayChannel(-1, CAPTURE, 0);
		if (piece % 3 == 1)
			Mix_PlayChannel(-1, CAPTURE2, 0);
		if (piece % 3 == 2)
			Mix_PlayChannel(-1, CAPTURE3, 0);
	}
}

void Init_Table(int T[10][10])
{
	Mix_PlayChannel(-1, NEWGAME, 0);
	int i;
	for (i = A8; i <= H1; i++)
		T[i / 8][i % 8] = -1;
	for (i = A7; i <= H7; i++)
		put_piece(T, pionnegru, i);
	for (i = A2; i <= H2; i++)
		put_piece(T, pionalb, i);
	put_piece(T, calalb, B1);
	put_piece(T, calalb, G1);
	put_piece(T, calnegru, B8);
	put_piece(T, calnegru, G8);
	put_piece(T, nebunalb, C1);
	put_piece(T, nebunalb, F1);
	put_piece(T, nebunnegru, C8);
	put_piece(T, nebunnegru, F8);
	put_piece(T, turnalb, A1);
	put_piece(T, turnalb, H1);
	put_piece(T, turnnegru, A8);
	put_piece(T, turnnegru, H8);
	put_piece(T, damaalb, D1);
	put_piece(T, damanegru, D8);
	put_piece(T, regealb, E1);
	put_piece(T, regenegru, E8);
}

void Show_Table(int T[10][10])
{
	int i;
	Show_Board();
	for (i = A8; i <= H1; i++)
		if (T[i / 8][i % 8] >= 0)
			Show_Piece(T[i / 8][i % 8], i);
}

bool IsValidMove(int T[10][10], int sq1, int sq2)
{
	int valid = 1, culoare1 = 0, culoare2 = -1, aux;
	int piece = T[sq1 / 8][sq1 % 8], piece2 = T[sq2 / 8][sq2 % 8];
	int StartLine, StartColumn, EndLine, EndColumn, LineDifference, ColumnDifference;
	int LineDirection, ColumnDirection, ColumnAux, LineAux;
	if (!(sq1 >= 0 && sq1<64 && sq2 >= 0 && sq2<64))
		return false;
	if (piece < 0)
		return false;
	if (sq1 == sq2)
		return false;

	StartLine = sq1 / 8;
	StartColumn = sq1 % 8;
	EndLine = sq2 / 8;
	EndColumn = sq2 % 8;


	LineDifference = EndLine - StartLine;
	if (LineDifference < 0)
		LineDifference = LineDifference*(-1);
	ColumnDifference = EndColumn - StartColumn;
	if (ColumnDifference < 0)
		ColumnDifference = ColumnDifference*(-1);
	if (piece <= regealb)
		culoare1 = 1;
	else culoare1 = 0;
	if (piece2 != -1)
		if (piece2 <= regealb)
			culoare2 = 1;
		else culoare2 = 0;
	else culoare2 = -1;
	if (culoare1 == culoare2)
		return false;

	if (piece == pionalb)
	{

		if (ColumnDifference > 1)
			return false;

		if (LineDifference > 2)
			return false;

		if (StartLine <= EndLine)
			return false;
		if (LineDifference == 2)
		{
			if (EndColumn != StartColumn)
				return false;

			if (StartLine != 6 || EndLine != 4)
				return false;

			if (T[StartLine - 1][StartColumn] != -1 || T[EndLine][EndColumn] != -1)
				return false;

			return true;
		}
		if (LineDifference == 1 && ColumnDifference == 0)
		{
			if (EndColumn != StartColumn)
				return false;

			if (T[EndLine][EndColumn] != -1)
				return false;

			return true;
		}
		if (LineDifference == 1 && ColumnDifference == 1)
		{
			if (T[EndLine][EndColumn] != -1)
				return true;
			if (T[EndLine + 1][EndColumn] == pionnegru&&EndLine == 2)
				return true;

			return false;
		}
	}

	if (piece == pionnegru)
	{
		if (ColumnDifference > 1)
			return false;

		if (LineDifference > 2)
			return false;

		if (StartLine >= EndLine)
			return false;

		if (LineDifference == 2)
		{
			if (EndColumn != StartColumn)
				return false;
			if (StartLine != 1 || EndLine != 3)
				return false;
			if (T[StartLine + 1][StartColumn] != -1 || T[EndLine][EndColumn] != -1)
				return false;
			return true;
		}
		if (LineDifference == 1 && ColumnDifference == 0)
		{
			if (EndColumn != StartColumn)
				return false;
			if (T[EndLine][EndColumn] != -1)
				return false;
			return true;
		}
		if (LineDifference == 1 && ColumnDifference == 1)
		{
			if (T[EndLine][EndColumn] == -1 && T[EndLine - 1][EndColumn] != pionalb)
				return false;
			if (T[EndLine][EndColumn] != -1)
				return true;
			if (T[EndLine - 1][EndColumn] == pionalb&&EndLine == 5)
				return true;
			return false;
		}
	}

	if (piece == calalb || piece == calnegru)
	{
		if ((!(ColumnDifference == 2 && LineDifference == 1)) && (!(ColumnDifference == 1 && LineDifference == 2)))
		{
			valid = 0;
		}
	}
	if (piece == nebunalb || piece == nebunnegru)
	{
		if (ColumnDifference != LineDifference)
			return false;
		if (ColumnDifference == 0 || LineDifference == 0)
			return false;
		LineDirection = (EndLine - StartLine) / LineDifference;
		ColumnDirection = (EndColumn - StartColumn) / ColumnDifference;
		LineAux = StartLine + LineDirection;
		ColumnAux = StartColumn + ColumnDirection;
		while (LineAux != EndLine)
		{
			if (T[LineAux][ColumnAux] != -1)
				return false;
			LineAux += LineDirection;
			ColumnAux += ColumnDirection;
		}
	}

	if (piece == turnalb || piece == turnnegru)
	{
		if (!(LineDifference == 0 || ColumnDifference == 0 || LineDifference == ColumnDifference))
			return false;

		if (LineDifference == 0)
		{

			ColumnAux = StartColumn;
			LineAux = StartLine;
			ColumnDirection = (EndColumn - StartColumn) / ColumnDifference;
			ColumnAux = StartColumn + ColumnDirection;
			while (ColumnAux != EndColumn)
			{
				if (T[LineAux][ColumnAux] != -1)
					return false;
				ColumnAux += ColumnDirection;
			}

			return true;
		}
		if (ColumnDifference == 0)
		{
			ColumnAux = StartColumn;
			LineAux = StartLine;
			LineDirection = (EndLine - StartLine) / LineDifference;
			LineAux = StartLine + LineDirection;
			while (LineAux != EndLine)
			{
				if (T[LineAux][ColumnAux] != -1)
				{
					return false;
				}
				LineAux += LineDirection;
			}
			return true;
		}
		if (ColumnDifference == LineDifference)
			return false;
		return true;
	}
	if (piece == damaalb || piece == damanegru)   // problema
	{
		if (!(LineDifference == 0 || ColumnDifference == 0 || LineDifference == ColumnDifference))
			return false;

		if (LineDifference == 0)
		{

			ColumnAux = StartColumn;
			LineAux = StartLine;
			ColumnDirection = (EndColumn - StartColumn) / ColumnDifference;
			ColumnAux = StartColumn + ColumnDirection;
			while (ColumnAux != EndColumn)
			{
				if (T[LineAux][ColumnAux] != -1)
					return false;
				ColumnAux += ColumnDirection;
			}

			return true;
		}
		if (ColumnDifference == 0)
		{
			ColumnAux = StartColumn;
			LineAux = StartLine;
			LineDirection = (EndLine - StartLine) / LineDifference;
			LineAux = StartLine + LineDirection;
			while (LineAux != EndLine)
			{
				if (T[LineAux][ColumnAux] != -1)
				{
					return false;
				}
				LineAux += LineDirection;
			}
			return true;
		}
		if (ColumnDifference == LineDifference)
		{

			ColumnDirection = (EndColumn - StartColumn) / ColumnDifference;
			LineDirection = (EndLine - StartLine) / LineDifference;
			LineAux = StartLine + LineDirection;
			ColumnAux = StartColumn + ColumnDirection;
			while (LineAux != EndLine&&ColumnAux != EndColumn)
			{
				if (T[LineAux][ColumnAux] != -1)
					return false;
				LineAux += LineDirection;
				ColumnAux += ColumnDirection;
			}

			return true;
		}
	}

	if (piece == regealb || piece == regenegru)
	{
		if (!(LineDifference == 0 || ColumnDifference == 0 || LineDifference == ColumnDifference))
			return false;
		if (LineDifference > 1)
			return false;
		if (ColumnDifference == 2 || ColumnDifference == 3)
		{
			if (LineDifference > 0)
				return false;
			if (piece == regealb)
			{
				if (sq1 / 8 != 7 && sq1 % 8 != 4)
					return false;
				if (sq2 != 7 * 8 + 6 && sq2 != 7 * 8 + 2)
					return false;
				if (sq2 == 7 * 8 + 6)
				{
					if (T[7][7] != turnalb)
						return false;
					if (T[7][5] != -1 || T[7][6] != -1)
						return false;
					return true;
				}
				if (sq2 == 7 * 8 + 2)
				{
					if (T[7][0] != turnalb)
						return false;
					if (T[7][2] != -1 || T[7][3] != -1 || T[7][1] != -1)
						return false;
					return true;
				}
			}
			if (piece == regenegru)
			{
				if (sq1 / 8 != 0 && sq1 % 8 != 4)
					return false;
				if (sq2 != 0 * 8 + 6 && sq2 != 0 * 8 + 2)
					return false;
				if (sq2 == 6)
				{
					if (T[0][7] != turnnegru)
						return false;
					if (T[0][5] != -1 || T[0][6] != -1)
						return false;
					return true;
				}
				if (sq2 == 2)
				{
					if (T[0][0] != turnnegru)
						return false;
					if (T[0][1] != -1 || T[0][2] != -1 || T[0][3] != -1)
						return false;
					return true;
				}
			}
		}
	}

	return valid;
}

bool IsLegalMove(int T[10][10], int sq1, int sq2)
{
	if (!IsValidMove(T, sq1, sq2))
		return false;
	int p1, p2;
	p1 = T[sq1 / 8][sq1 % 8];
	p2 = T[sq2 / 8][sq2 % 8];
	T[sq2 / 8][sq2 % 8] = T[sq1 / 8][sq1 % 8];
	T[sq1 / 8][sq1 % 8] = -1;
	int BlackKingLine, BlackKingColumn, WhiteKingLine, WhiteKingColumn, i, j, piece = T[sq2 / 8][sq2 % 8];
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (T[i][j] == regealb)
			{
				WhiteKingLine = i;
				WhiteKingColumn = j;
			}
			if (T[i][j] == regenegru)
			{
				BlackKingColumn = j;
				BlackKingLine = i;
			}
		}
	}

	if (piece <= regealb)   
		if (IsAttackedByBlack(T, WhiteKingLine * 8 + WhiteKingColumn) > 0)
		{
			T[sq1 / 8][sq1 % 8] = p1;
			T[sq2 / 8][sq2 % 8] = p2;
			return false;
		}
	if (piece > regealb)
		if (IsAttackedByWhite(T, BlackKingLine * 8 + BlackKingColumn) > 0)
		{
			T[sq1 / 8][sq1 % 8] = p1;
			T[sq2 / 8][sq2 % 8] = p2;
			return false;
		}
	T[sq1 / 8][sq1 % 8] = p1;
	T[sq2 / 8][sq2 % 8] = p2;
	return true;
}

int IsAttackedByWhite(int T[10][10], int sq)
{
	int piece, i, j, attacked = 0, vl, vc, dir;

	for (i = 0; i<8; i++)
		for (j = 0; j < 8; j++)
		{
			piece = T[i][j];
			if (piece == pionalb)
			{
				vl = i - 1;
				vc = j - 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							attacked++;
				vc = j + 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							attacked++;
			}
			if (piece == calalb)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dcl[dir];
					vc = j + dcc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
				}
			}

			if (piece == nebunalb)
			{
				for (dir = 4; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}

			if (piece == turnalb)
			{
				for (dir = 0; dir < 4; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}

			if (piece == damaalb)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}

			if (piece == regealb)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
					}
				}
			}

		}

	return attacked;
}

int IsAttackedByBlack(int T[10][10], int sq)
{
	int piece, i, j, attacked = 0, dir, vl, vc;

	for (i = 0; i<8; i++)
		for (j = 0; j < 8; j++)
		{
			piece = T[i][j];
			if (piece == pionnegru)
			{
				vl = i + 1;
				vc = j - 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							attacked++;
				vc = j + 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							attacked++;
			}

			if (piece == calnegru)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dcl[dir];
					vc = j + dcc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
				}
			}


			if (piece == nebunnegru)
			{
				for (dir = 4; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}

			if (piece == turnnegru)
			{
				for (dir = 0; dir < 4; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}

			if (piece == damanegru)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
						vl += dl[dir];
						vc += dc[dir];
					}
				}
			}
			if (piece == regenegru)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dl[dir];
					vc = j + dc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					{
						if (IsValidMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
					}
				}
			}
		}

	return attacked;
}

void PresentTableControl(int T[10][10])
{
	int i, j, col, piece, vl, vc, dir;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			WhiteControl[i][j] = 0;
			BlackControl[i][j] = 0;
		}
	}
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			WhiteControl[i][j] = IsAttackedByWhite(T, i * 8 + i % 8);
			BlackControl[i][j] = IsAttackedByBlack(T, i * 8 + i % 8);
		}
	}
}

void Copie_Tabla(int T1[10][10], int T2[10][10])
{
	int i, j;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			T1[i][j] = T2[i][j];
}

bool KingNotAbleToMove(int T[10][10], int tomove)
{
	int king, i, j, kingl, kingc, attacked = 0, dir, vl, vc;
	if (tomove == 1)
		king = regealb;
	else king = regenegru;
	for (i = 0; i<8; i++)
		for (j = 0; j<8; j++)
			if (T[i][j] == king)
			{
				kingl = i;
				kingc = j;
			}
	if (king == regealb)
		if (IsAttackedByBlack(T, kingl * 8 + kingc % 8) > 0)
			attacked = 1;
	if (king == regenegru)
		if (IsAttackedByWhite(T, kingl * 8 + kingc % 8) > 0)
			attacked = 1;
	if (!attacked)
		return false;
	for (dir = 0; dir < 8; dir++)
	{
		vl = kingl + dl[dir];
		vc = kingc + dc[dir];
		if (IsLegalMove(T, kingl * 8 + kingc % 8, vl * 8 + vc % 8) == 1)
			return false;
	}

	


	return true;

}

bool ismate(int T[10][10], int tomove)
{
	if (!KingNotAbleToMove(T, tomove))
		return false;
	int best_move_sq1, best_move_sq2;
	int piece, attacked = 0, dir, vl, vc, cscore;
	int T2[10][10], i2, j2, csq1, csq2, cmaxscore2, cminscore2, KingSq, i, j;
	
	if (tomove == -1)
	{
		for (i = 0; i < 8; i++)
			for (j = 0; j < 8; j++)
				if (T[i][j] == regenegru)
					KingSq = i * 8 + j;
		if (!IsAttackedByWhite(T, KingSq))
			return false;
		for (i = 0; i < 8; i++)
			for (j = 0; j < 8; j++)
			{
				piece = T[i][j];
				if (piece == pionnegru)
				{
					vl = i + 1;
					vc = j - 1;
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
						{
							for (i2 = 0; i2 < 8; i2++)
								for (j2 = 0; j2 < 8; j2++)
									T2[i2][j2] = T[i2][j2];
							move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
							
							if (!IsAttackedByWhite(T2, KingSq))
								return false;
						}
					vc = j + 1;
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
						{
							for (i2 = 0; i2 < 8; i2++)
								for (j2 = 0; j2 < 8; j2++)
									T2[i2][j2] = T[i2][j2];

							move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

							if (!IsAttackedByWhite(T2, KingSq))
								return false;
						}
					vl = i + 1;
					vc = j;
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
						{
							for (i2 = 0; i2 < 8; i2++)
								for (j2 = 0; j2 < 8; j2++)
									T2[i2][j2] = T[i2][j2];

							move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

							if(!IsAttackedByWhite(T2, KingSq))
								return false;
						}
					vl = i + 2;
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
						{
							for (i2 = 0; i2 < 8; i2++)
								for (j2 = 0; j2 < 8; j2++)
									T2[i2][j2] = T[i2][j2];
							move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

							if (!IsAttackedByWhite(T2, KingSq))
								return false;
						}
				}

				if (piece == calnegru)
				{
					for (dir = 0; dir < 8; dir++)
					{
						vl = i + dcl[dir];
						vc = j + dcc[dir];
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByWhite(T2, KingSq))
									return false;
							}
					}
				}

				if (piece == nebunnegru)
				{
					for (dir = 4; dir < 8; dir++)
					{
						vl = i + dl[dir];
						vc = j + dc[dir];
						while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						{
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByWhite(T2, KingSq))
									return false;
							}
							vl += dl[dir];
							vc += dc[dir];
						}
					}
				}

				if (piece == turnnegru)
				{
					for (dir = 0; dir < 4; dir++)
					{
						vl = i + dl[dir];
						vc = j + dc[dir];
						while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						{
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByWhite(T2, KingSq))
									return false;
							}
							vl += dl[dir];
							vc += dc[dir];
						}
					}
				}

				if (piece == damanegru)
				{
					for (dir = 0; dir < 8; dir++)
					{
						vl = i + dl[dir];
						vc = j + dc[dir];
						while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						{
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByWhite(T2, KingSq))
									return false;
							}
							vl += dl[dir];
							vc += dc[dir];
						}
					}
				}
			}
	}
		if (tomove == 1)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
					if (T[i][j] == regealb)
						KingSq = i * 8 + j;
			if (!IsAttackedByBlack(T, KingSq))
				return false;
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
				{
					piece = T[i][j];
					if (piece == pionalb)
					{
						vl = i - 1;
						vc = j - 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);


								if (!IsAttackedByBlack(T2, KingSq))
									return false;
							}
						vc = j + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByBlack(T2, KingSq))
									return false;
							}
						vl = i - 1;
						vc = j;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByBlack(T2, KingSq))
									return false;
							}
						vl = i - 2;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (!IsAttackedByBlack(T2, KingSq))
									return false;
							}
					}

					if (piece == calalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dcl[dir];
							vc = j + dcc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (!IsAttackedByBlack(T2, KingSq))
										return false;
								}
						}
					}

					if (piece == nebunalb)
					{
						for (dir = 4; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (!IsAttackedByBlack(T2, KingSq))
										return false;
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == turnalb)
					{
						for (dir = 0; dir < 4; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (!IsAttackedByBlack(T2, KingSq))
										return false;
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == damaalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (!IsAttackedByBlack(T2, KingSq))
										return false;
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}
				}
		}
		
		return true;
}

void RenderToMouse(int p)
{
	/*SDL_Rect mouseport;
	SDL_Surface *temp, *sprite;
	int x, y;
	SDL_GetMouseState(&x, &y);
	mouseport.x = x;
	mouseport.y = y;
	mouseport.h = SQPort[0].h;
	mouseport.w = SQPort[0].w;
	//temp = PieceTexture[p].mTexture;
	sprite = SDL_DisplayFormat(temp);
	SDL_BlitSurface(sprite, NULL, screen, mouseport);
	*/
}

void Set_Piece_Value()
{
	Piece_Value[pionalb] = 10;
	Piece_Value[calalb] = 30;
	Piece_Value[nebunalb] = 35;
	Piece_Value[turnalb] = 50;
	Piece_Value[damaalb] = 90;
	Piece_Value[regealb] = 40;

	Piece_Value[pionnegru] = -10;
	Piece_Value[calnegru] = -30;
	Piece_Value[nebunnegru] = -35;
	Piece_Value[turnnegru] = -50;
	Piece_Value[damanegru] = -90;
	Piece_Value[regenegru] = -40;


}


void Set_SQ_Value_Black()
{
	int l, c;
	for (l = 0; l<8; l++)
		for (c = 0; c < 8; c++)
		{
			SQ_Value_Black[l * 8 + c] = (-1)*SQ_Value_White[(7 - l) * 8 + c];
		}
}

void HighlightSQ(int sq)
{
	SDL_Rect port = SQPort[sq];
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &port);
	SDL_RenderPresent(gRenderer);
}

void vsPlayer()
{
	SDL_RenderClear(gRenderer);
	Show_Background();
	Show_Board();
	Partida *InitialPosition = new Partida;
	Partida *LastPosition = new Partida ;
	Init_Table(InitialPosition->CurrentTable);
	InitialPosition->prec = NULL;
	InitialPosition->tomove = 1;
	Show_Table(InitialPosition->CurrentTable);
	Partida *CurrentPosition = new Partida;
	CurrentPosition = InitialPosition;
	int i, j, sq1 = -1, sq2 = -1, l1, c1, l2, c2, tomove = 1, piece, movecount = 0, k1, k2, undolastmove = 0;
	int exitomainmenu = 0;
	int T[300][10][10];
	bool quit = false;
	SDL_Event e;
	LastPosition = InitialPosition;
	Copie_Tabla(LastPosition->CurrentTable, InitialPosition->CurrentTable);
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				sq1 = -1;
				sq2 = -1;
				int x, y;
				SDL_GetMouseState(&x, &y);
				int inside;
				if (x > SQPort[0].x&&x < SQPort[63].x + SQPort[63].w&&y > SQPort[0].y&&y < SQPort[63].y + SQPort[63].h)
					inside = 1;
				else inside = 0;
				if (inside)
					for (i = 0; i < 64; i++)
						if (x >= SQPort[i].x&&x < SQPort[i].x + SQPort[i].w&&y > SQPort[i].y&&y < SQPort[i].y + SQPort[i].h)
						{
							sq1 = i;

							if (CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] != -1)
							{
								piece = CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8];

								Show_Table(CurrentPosition->CurrentTable);

								SDL_PollEvent(&e);
								while (e.type != SDL_MOUSEBUTTONUP)
								{
									SDL_PollEvent(&e);
									RenderToMouse(CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8]);
								}
								SDL_GetMouseState(&x, &y);
								if (x >= SQPort[0].x&&x <= SQPort[63].x + SQPort[63].w&&y >= SQPort[0].y&&y <= SQPort[63].y + SQPort[63].h)
									inside = 1;
								else inside = 0;
								if (inside)
									for (j = 0; j < 64; j++)
									{
										if (x > SQPort[j].x&&x < SQPort[j].x + SQPort[j].w&&y > SQPort[j].y&&y < SQPort[j].y + SQPort[j].h)
										{
											sq2 = j;
											if (sq2 != sq1)
											{
												CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] = piece;
												if (tomove == 1)
													if (piece >= pionalb&&piece <= regealb && IsLegalMove(CurrentPosition->CurrentTable, sq1, sq2))
													{
														LastPosition = CurrentPosition;
														Copie_Tabla(LastPosition->CurrentTable, CurrentPosition->CurrentTable);

														CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] = piece;
														CurrentPosition->sq1 = sq1;
														CurrentPosition->sq2 = sq2;
														CurrentPosition->tomove = tomove;
														Partida *AuxPosition = new Partida;
														AuxPosition = CurrentPosition; //pozitia dupa de mutare
														AuxPosition->prec = CurrentPosition;
														CurrentPosition->urm = AuxPosition;

														for (k1 = 0; k1 < 64; k1++)
															for (k2 = 0; k2 < 64; k2++)
																T[movecount][k1][k2] = AuxPosition->CurrentTable[k1][k2];
														movecount++;
														undolastmove = 0;
														makemove(AuxPosition->CurrentTable, sq1, sq2);
														CurrentPosition = AuxPosition;

														
														Show_Table(CurrentPosition->CurrentTable);
														
														tomove = 0 - tomove;
													}
													else;

												else
													if (tomove == -1)
														if (IsLegalMove(CurrentPosition->CurrentTable, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
														{
															LastPosition = CurrentPosition;
															Copie_Tabla(LastPosition->CurrentTable, CurrentPosition->CurrentTable);

															CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] = piece;
															CurrentPosition->sq1 = sq1;
															CurrentPosition->sq2 = sq2;
															CurrentPosition->tomove = tomove;
															Partida *AuxPosition = new Partida;
															AuxPosition = CurrentPosition; //pozitia dupa de mutare
															AuxPosition->prec = CurrentPosition;
															CurrentPosition->urm = AuxPosition;
															for (k1 = 0; k1 < 64; k1++)
																for (k2 = 0; k2 < 64; k2++)
																	T[movecount][k1][k2] = AuxPosition->CurrentTable[k1][k2];
															movecount++;
															undolastmove = 0;

															makemove(AuxPosition->CurrentTable, sq1, sq2);
															CurrentPosition = AuxPosition;
															Show_Table(CurrentPosition->CurrentTable);
															tomove = 0 - tomove;
														}
														else;

											}
											else
											{

												SDL_PollEvent(&e);
												while (e.type != SDL_MOUSEBUTTONDOWN)
												{
													SDL_PollEvent(&e);
												}
												SDL_GetMouseState(&x, &y);
												if (x > SQPort[0].x&&x < SQPort[63].x + SQPort[63].w&&y > SQPort[0].y&&y < SQPort[63].y + SQPort[63].h)
													inside = 1;
												else inside = 0;
												if (inside)
													for (j = 0; j < 64; j++)
													{
														if (x > SQPort[j].x&&x < SQPort[j].x + SQPort[j].w&&y > SQPort[j].y&&y < SQPort[j].y + SQPort[j].h)
														{
															sq2 = j;
															if (tomove == 1)
																if (piece >= pionalb&&piece <= regealb && IsLegalMove(CurrentPosition->CurrentTable, sq1, sq2))
																{
																	LastPosition = CurrentPosition;
																	Copie_Tabla(LastPosition->CurrentTable, CurrentPosition->CurrentTable);

																	CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] = piece;
																	CurrentPosition->sq1 = sq1;
																	CurrentPosition->sq2 = sq2;
																	CurrentPosition->tomove = tomove;
																	Partida *AuxPosition = new Partida;
																	AuxPosition = CurrentPosition; //pozitia dupa de mutare
																	AuxPosition->prec = CurrentPosition;
																	CurrentPosition->urm = AuxPosition;
																	for (k1 = 0; k1 < 64; k1++)
																		for (k2 = 0; k2 < 64; k2++)
																			T[movecount][k1][k2] = AuxPosition->CurrentTable[k1][k2];
																	movecount++;
																	undolastmove = 0;

																	makemove(AuxPosition->CurrentTable, sq1, sq2);
																	CurrentPosition = AuxPosition;

																	Show_Table(CurrentPosition->CurrentTable);
																	tomove = 0 - tomove;
																}
																else;

															else
																if (tomove == -1)
																	if (IsLegalMove(CurrentPosition->CurrentTable, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
																	{
																		LastPosition = CurrentPosition;
																		Copie_Tabla(LastPosition->CurrentTable, CurrentPosition->CurrentTable);

																		CurrentPosition->CurrentTable[sq1 / 8][sq1 % 8] = piece;
																		CurrentPosition->sq1 = sq1;
																		CurrentPosition->sq2 = sq2;
																		CurrentPosition->tomove = tomove;
																		Partida *AuxPosition = new Partida;
																		AuxPosition = CurrentPosition; 
																		AuxPosition->prec = CurrentPosition;
																		CurrentPosition->urm = AuxPosition; for (k1 = 0; k1 < 64; k1++)
																			for (k2 = 0; k2 < 64; k2++)
																				T[movecount][k1][k2] = AuxPosition->CurrentTable[k1][k2];
																		movecount++;
																		undolastmove = 0;

																		makemove(AuxPosition->CurrentTable, sq1, sq2);
																		CurrentPosition = AuxPosition;
																		Show_Table(CurrentPosition->CurrentTable);
																		tomove = 0 - tomove;
																	}
														}
													}
												else
												{

													if (x >= BackButton.x && x <= BackButton.x + BackButton.w && y >= BackButton.y && y <= BackButton.y + BackButton.h)
													{
														quit = 1;
														exitomainmenu = 1;
														quit = 1;
														break;
													}

													if (x >= UndoButton.x && x <= UndoButton.x + UndoButton.w && y >= UndoButton.y && y <= UndoButton.y + UndoButton.h)
													{
														movecount--;
														if (movecount < 0)
															movecount = 0;
														else
															if(!undolastmove)
														{
															for (k1 = 0; k1 < 64; k1++)
																for (k2 = 0; k2 < 64; k2++)
																	CurrentPosition->CurrentTable[k1][k2] = T[movecount][k1][k2];
															tomove = 0 - tomove;
															undolastmove = 1;

														}
														


														Show_Table(CurrentPosition->CurrentTable);
													}
													// undo


												}
											}



											if (ismate(CurrentPosition->CurrentTable, tomove))
													{
														quit = 1;
														cout << 0 - tomove << " wins\n";
														//Show window "-tomove wins" 
														if (tomove == 1)
															Show_Black_Win();
														if (tomove == -1)
															Show_White_Win();
														SDL_Delay(5000);
														exitomainmenu = 1;
														break;

													}

													//
										}
									}
								else
								{

									if (x >= BackButton.x && x <= BackButton.x + BackButton.w && y >= BackButton.y && y <= BackButton.y + BackButton.h)
									{
										quit = 1;
										exitomainmenu = 1;

										quit = 1;
										break;
									}

									if (x >= UndoButton.x && x <= UndoButton.x + UndoButton.w && y >= UndoButton.y && y <= UndoButton.y + UndoButton.h)
									{
										movecount--;
										if (movecount < 0)
											movecount = 0;
										else
											if(!undolastmove)
										{
											tomove = 0 - tomove;
											undolastmove = 1;

											for (k1 = 0; k1 < 64; k1++)
												for (k2 = 0; k2 < 64; k2++)
													CurrentPosition->CurrentTable[k1][k2] = T[movecount][k1][k2];
										}


										Show_Table(CurrentPosition->CurrentTable);
									}
									// undo


								}
							}
						}
						else;
				else
				{
					
					if (x >=BackButton.x && x <= BackButton.x + BackButton.w && y >= BackButton.y && y <= BackButton.y + BackButton.h)
					{
						quit = 1;
						exitomainmenu = 1;

						break;
						quit = 1;
					}

					if (x >= UndoButton.x && x <= UndoButton.x + UndoButton.w && y >= UndoButton.y && y <= UndoButton.y + UndoButton.h)
					{
						movecount--;
						if (movecount < 0)
							movecount = 0;
						else
							if(!undolastmove)
						{
							for (k1 = 0; k1 < 64; k1++)
								for (k2 = 0; k2 < 64; k2++)
									CurrentPosition->CurrentTable[k1][k2] = T[movecount][k1][k2];
							tomove = 0 - tomove;
							undolastmove = 1;

						}


						Show_Table(CurrentPosition->CurrentTable);
					}
					// undo


				}

			}

		}

	}
	exitomainmenu = 1;
	if (exitomainmenu == 1)
		MainMenu();
}


void Show_Main_Menu()
{
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	Main_Menu_Background.render(WindowPort);

}

void SetButtonPort()
{
	Button1.x = 388;
	Button1.y = 252;
	Button1.h = 70;
	Button1.w = 305;
	
	Button2.x = 388;
	Button2.y = 365;
	Button2.h = 70;
	Button2.w = 305;
	
	Button3.x = 388;
	Button3.y = 480;
	Button3.h = 70;
	Button3.w = 305;
	
	Button4.x = 480;
	Button4.y = 605;
	Button4.h = 25;
	Button4.w = 120;

	BackButton.x = 72;
	BackButton.y = 645; //
	BackButton.h = 32; //
	BackButton.w = 90;

	UndoButton.x = 294;
	UndoButton.y = 645;
	UndoButton.h = 30;
	UndoButton.w = 134;
}

int IsInButton()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x > Button1.x&&x<Button1.x + Button1.w&&y>Button1.y&&y < Button1.y + Button1.h)
		return 1;
	if (x > Button2.x&&x<Button2.x + Button2.w&&y>Button2.y&&y < Button2.y + Button2.h)
		return 2;
	if (x > Button3.x&&x<Button3.x + Button3.w&&y>Button3.y&&y < Button3.y + Button3.h)
		return 3;
	if (x > Button4.x&&x<Button4.x + Button4.w&&y>Button4.y&&y < Button4.y + Button4.h)
		return 4;
	return 0;
}


int SelectDifficultyF()
{
	int option=-1, quit=0;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	SelectDifficulty.render(WindowPort);
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				option = IsInButton();
				if (option > 0)
					quit = 1;
			}
		}
	}
	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
				quit = 1;

		}
	}

		return option;
	
}

int SelectColor()
{
	int option = -1, quit = 0;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	SelectColor_Image.render(WindowPort);
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				option = IsInButton();  
				if (option == 1 || option == 2 || option == 4)
				{
					quit = 1;
				}
			}
		}
	}
	quit = 0;
	while(!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
				quit = 1;
				
		}
	}


	if (option == 1)
		return 1;
	if (option == 2)
		return -1;
	if (option == 4)
		return 4;
	return 0;

}

void Show_BackgroundComputerBlack()
{
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	BackgroundComputerBlack.render(WindowPort);
}

void Show_BackgroundComputerWhite()
{
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	BackgroundComputerWhite.render(WindowPort);
}


void vsComputer()
{
	int exittomainmenu = 0;
	int depth = SelectDifficultyF();
	int player_color;

	if (depth == 4)
		exittomainmenu = 1;
	if (exittomainmenu == 0)
	{
		player_color = SelectColor();
		if (player_color == 4)
			exittomainmenu = 1;
	}
	if (exittomainmenu == 0)
	{
		exittomainmenu = 0;
		SDL_RenderClear(gRenderer);
		Show_Background();
		if (player_color == 1)
			Show_BackgroundComputerBlack();
		if (player_color == -1)
			Show_BackgroundComputerWhite();
		Show_Board();
		Init_Table(Table);
		Show_Table(Table);

		char sir[5];
		int i, j, sq1 = -1, sq2 = -1, l1, c1, l2, c2, tomove = 1, piece, minscore, maxscore, movecount = 0, k1, k2, undolastmove = 0;
		int T[300][10][10];
		bool quit = false;
		SDL_Event e;
		while (!quit)
		{
			if (tomove == player_color)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						sq1 = -1;
						sq2 = -1;
						int x, y;
						SDL_GetMouseState(&x, &y);
						int inside;
						if (x >= SQPort[0].x&&x <= SQPort[63].x + SQPort[63].w&&y >= SQPort[0].y&&y <= SQPort[63].y + SQPort[63].h)
							inside = 1;
						else inside = 0;
						if (inside)
							for (i = 0; i < 64; i++)
								if (x >= SQPort[i].x&&x <= SQPort[i].x + SQPort[i].w&&y >= SQPort[i].y&&y <= SQPort[i].y + SQPort[i].h)
								{
									sq1 = i;
									if (Table[sq1 / 8][sq1 % 8] != -1)
									{
										piece = Table[sq1 / 8][sq1 % 8];

										Show_Table(Table);

										SDL_PollEvent(&e);
										while (e.type != SDL_MOUSEBUTTONUP)
										{
											SDL_PollEvent(&e);
										}
										SDL_GetMouseState(&x, &y);
										if (x >= SQPort[0].x&&x <= SQPort[63].x + SQPort[63].w&&y >= SQPort[0].y&&y <= SQPort[63].y + SQPort[63].h)
											inside = 1;
										else inside = 0;
										if (inside)
											for (j = 0; j < 64; j++)
											{
												if (x >= SQPort[j].x&&x <= SQPort[j].x + SQPort[j].w&&y >= SQPort[j].y&&y <= SQPort[j].y + SQPort[j].h)
												{

													sq2 = j;
													if (sq2 != sq1)
													{
														
														if (tomove == 1)
															if (piece >= pionalb&&piece <= regealb && IsLegalMove(Table, sq1, sq2))
															{
																Table[sq1 / 8][sq1 % 8] = piece;
																for (k1 = 0; k1 < 64; k1++)
																	for (k2 = 0; k2 < 64; k2++)
																		T[movecount][k1][k2] = Table[k1][k2];
																movecount++;
																undolastmove = 0;
																makemove(Table, sq1, sq2);
																Show_Table(Table);
																tomove = 0 - tomove;
															}
															else;

														else
															if (tomove == -1)
																if (IsLegalMove(Table, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
																{
																	Table[sq1 / 8][sq1 % 8] = piece;
																	for (k1 = 0; k1 < 64; k1++)
																		for (k2 = 0; k2 < 64; k2++)
																			T[movecount][k1][k2] = Table[k1][k2];
																	movecount++;
																	undolastmove = 0;

																	makemove(Table, sq1, sq2);
																	Show_Table(Table);
																	tomove = 0 - tomove;
																}
																else;
													}
													else
													{
														SDL_PollEvent(&e);
														while (e.type != SDL_MOUSEBUTTONDOWN)
														{
															SDL_PollEvent(&e);
														}
														SDL_GetMouseState(&x, &y);
														if (x >= SQPort[0].x&&x <= SQPort[63].x + SQPort[63].w&&y >= SQPort[0].y&&y <= SQPort[63].y + SQPort[63].h)
															inside = 1;
														else inside = 0;
														if (inside)
														{
															for (int k = 0; k < 64; k++)
																if (x >= SQPort[k].x&&x <= SQPort[k].x + SQPort[k].w&&y >= SQPort[k].y&&y <= SQPort[k].y + SQPort[k].h)
																{

																	sq2 = k;
																	if (sq2 != sq1)
																	{
																		Table[sq1 / 8][sq1 % 8] = piece;
																		if (tomove == 1)
																			if (piece >= pionalb&&piece <= regealb && IsLegalMove(Table, sq1, sq2))
																			{
																				Table[sq1 / 8][sq1 % 8] = piece;
																				for (k1 = 0; k1 < 64; k1++)
																					for (k2 = 0; k2 < 64; k2++)
																						T[movecount][k1][k2] = Table[k1][k2];
																				movecount++;
																				undolastmove = 0;

																				makemove(Table, sq1, sq2);
																				Show_Table(Table);
																				tomove = 0 - tomove;
																			}
																			else;

																		else
																			if (tomove == -1)
																				if (IsLegalMove(Table, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
																				{
																					Table[sq1 / 8][sq1 % 8] = piece;
																					for (k1 = 0; k1 < 64; k1++)
																						for (k2 = 0; k2 < 64; k2++)
																							T[movecount][k1][k2] = Table[k1][k2];
																					movecount++;
																					undolastmove = 0;

																					makemove(Table, sq1, sq2);
																					Show_Table(Table);
																					tomove = 0 - tomove;
																				}
																				else;
																	}
																}
														}
														else
														{
															if (x >= BackButton.x && x <= BackButton.x + BackButton.w && y >= BackButton.y && y <= BackButton.y + BackButton.h)
															{
																quit = 1;
																exittomainmenu = 1;
																quit = 1;
																break;
															}

															if (x >= UndoButton.x && x <= UndoButton.x + UndoButton.w && y >= UndoButton.y && y <= UndoButton.y + UndoButton.h)
															{
																movecount--;
																movecount--;

																if (movecount < 0)
																	movecount = 0;
																else
																	if (undolastmove == 0)
																	{
																		undolastmove = 1;

																		for (k1 = 0; k1 < 64; k1++)
																			for (k2 = 0; k2 < 64; k2++)
																				Table[k1][k2] = T[movecount][k1][k2];
																	}


																Show_Table(Table);
															}
														}
													}
												}
											}
									}
								}
								else;
						else
						{

							if (x >= BackButton.x && x <= BackButton.x + BackButton.w && y >= BackButton.y && y <= BackButton.y + BackButton.h)
							{
								quit = 1;
								exittomainmenu = 1;

								quit = 1;
								break;
							}

							if (x >= UndoButton.x && x <= UndoButton.x + UndoButton.w && y >= UndoButton.y && y <= UndoButton.y + UndoButton.h)
							{
								movecount--;
								movecount--;

								if (movecount < 0)
								{
									movecount = 0;
									tomove = 1;
								}
								else
									if (undolastmove == 0)

									{
										undolastmove = 1;
										for (k1 = 0; k1 < 64; k1++)
											for (k2 = 0; k2 < 64; k2++)
												Table[k1][k2] = T[movecount][k1][k2];
									}


								Show_Table(Table);
							}
						}


					}

				}
				if (ismate(Table, tomove))
				{
					quit = 1;
					if (tomove == 1)
						Show_Black_Win();
					if (tomove == -1)
						Show_White_Win();
					SDL_Delay(3000);


					MainMenu();

				}

			}
			if (tomove == 0 - player_color)
			{
				//make computer move

				MoveGenerator(Table, tomove, sq1, sq2, minscore, maxscore, depth);
				makemove(Table, sq1, sq2);
				movecount++;
				Show_Table(Table);
				tomove = 0 - tomove;
				if (ismate(Table, tomove))
				{
					quit = 1;
					if (tomove == 1)
						Show_Black_Win();
					if (tomove == -1)
						Show_White_Win();
					SDL_Delay(5000);
					exittomainmenu = 1;


				}
			}
		}
	}
		MainMenu();
}


void MoveGenerator(int T[10][10], int tomove, int &sq1, int &sq2, int &minscore, int &maxscore, int depth)
{

	int best_move_for_white_sq1 = 2, best_move_for_white_sq2 = 3;
	int best_move_for_black_sq1 = 2, best_move_for_black_sq2 = 3;
	int best_move_sq1, best_move_sq2;

	int piece, i, j, attacked = 0, dir, vl, vc, cmaxscore = -1000000, cminscore = 1000000, cscore;
	int T2[10][10], i2, j2, csq1, csq2, cmaxscore2, cminscore2;
	// doar pentru test
	if (depth == 1)
	{
		//pentru fiecare mutare posibila
		//cmaxscore = cminscore = Eval(T);
		if (tomove == -1)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
				{
					piece = T[i][j];
					if (piece == pionnegru)
					{
						vl = i + 1;
						vc = j - 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];
								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);


								if(ismate(T2, 1))
								{
									cscore = -200000;

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
							}
						vc = j + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
								if (ismate(T2, 1))
								{
									cscore = -200000;

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
							}
						vl = i + 1;
						vc = j;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
								if (ismate(T2, 1))
								{
									cscore = -200000;

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
							}
						vl = i + 2;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];
								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
								if (ismate(T2, 1))
								{
									cscore = -200000;

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
							}
					}

					if (piece == calnegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dcl[dir];
							vc = j + dcc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
									if (ismate(T2, 1))
									{
										cscore = -200000;

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
								}
						}
					}

					if (piece == nebunnegru)
					{
						for (dir = 4; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
									if (ismate(T2, 1))
									{
										cscore = -200000;

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == turnnegru)
					{
						for (dir = 0; dir < 4; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
									if (ismate(T2, 1))
									{
										cscore = -200000;

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == damanegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, 1))
									{
										cscore = -200000;

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore < cminscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}
					if (piece == regenegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
									if(ismate(T2, 1))
								{
									cscore = -200000;

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
									}
								}
								}
							}
						}
					}
				}
		}
		if (tomove == 1)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
				{
					piece = T[i][j];
					if (piece == pionalb)
					{
						vl = i - 1;
						vc = j - 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (ismate(T2, -1))
								{
									cscore = 200000;

									if (cscore > cmaxscore )
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
							



							}
						vc = j + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (ismate(T2, -1))
								{
									cscore = 200000;

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
							}
						vl = i - 1;
						vc = j;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (ismate(T2, -1))
								{
									cscore = 200000;

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
							}
						vl = i - 2;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								if (ismate(T2, -1))
								{
									cscore = 200000;

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
								else
								{
									cscore = Eval(T2);

									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
									}
								}
							}
					}

					if (piece == calalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dcl[dir];
							vc = j + dcc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, -1))
									{
										cscore = 200000;

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
								}
						}
					}

					if (piece == nebunalb)
					{
						for (dir = 4; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, -1))
									{
										cscore = 200000;

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == turnalb)
					{
						for (dir = 0; dir < 4; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, -1))
									{
										cscore = 200000;

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == damaalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, -1))
									{
										cscore = 200000;

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}
					if (piece == regealb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									if (ismate(T2, -1))
									{
										cscore = 200000;

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}
									else
									{
										cscore = Eval(T2);

										if (cscore > cmaxscore)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cmaxscore = cscore;
										}
									}

								}
							}
						}
					}
				}
		}
	}
	if (depth > 1)
	{
		if (tomove == -1)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
				{
					piece = T[i][j];
					if (piece == pionnegru)
					{
						vl = i + 1;
						vc = j - 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cmaxscore2 < cminscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cmaxscore2;
								}
							}
						vc = j + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cmaxscore2 < cminscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cmaxscore2;
								}
							}

						vc = j;
						vl = i + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cmaxscore2 < cminscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cmaxscore2;
								}
							}
						vl = i + 2;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cmaxscore2 < cminscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cmaxscore2;
								}
							}
					}

					if (piece == calnegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dcl[dir];
							vc = j + dcc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cmaxscore2 < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cmaxscore2;
									}
								}
						}
					}

					if (piece == nebunnegru)
					{
						for (dir = 4; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cmaxscore2 < cminscore)
										if (tomove == -1)
										{
											best_move_sq1 = i * 8 + j % 8;
											best_move_sq2 = vl * 8 + vc % 8;
											cminscore = cmaxscore2;
										}

								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == turnnegru)
					{
						for (dir = 0; dir < 4; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);

									if (cmaxscore2 < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cmaxscore2;
									}

								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == damanegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);

									if (cmaxscore2 < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cmaxscore2;
									}

								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}
					if (piece == regenegru)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);


									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);

									if (cmaxscore2 < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cmaxscore2;
									}

								}
							}
						}
					}
				}
		}
		if (tomove == 1)
		{
			for (i = 0; i < 8; i++)
				for (j = 0; j < 8; j++)
				{
					piece = T[i][j];
					if (piece == pionalb)
					{
						vl = i - 1;
						vc = j - 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);
								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cminscore2 > cmaxscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cminscore2;
								}
							}
						vc = j + 1;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cminscore2 > cmaxscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cminscore2;
								}
							}
						vl = i - 1;
						vc = j;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cminscore2 > cmaxscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cminscore2;
								}
							}
						vl = i - 2;
						if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
							{
								for (i2 = 0; i2 < 8; i2++)
									for (j2 = 0; j2 < 8; j2++)
										T2[i2][j2] = T[i2][j2];

								move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

								MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
								if (cminscore2 > cmaxscore)

								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cminscore2;
								}
							}
					}

					if (piece == calalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dcl[dir];
							vc = j + dcc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))

								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cminscore2 > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cminscore2;
									}

								}

						}
					}

					if (piece == nebunalb)
					{
						for (dir = 4; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cminscore2 > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cminscore2;
									}


								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == turnalb)
					{
						for (dir = 0; dir < 4; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cminscore2 > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cminscore2;
									}


								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}

					if (piece == damaalb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cminscore2 > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cminscore2;
									}


								}
								vl += dl[dir];
								vc += dc[dir];
							}
						}
					}
					if (piece == regealb)
					{
						for (dir = 0; dir < 8; dir++)
						{
							vl = i + dl[dir];
							vc = j + dc[dir];
							if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
							{
								if (IsLegalMove(T, i * 8 + j % 8, vl * 8 + vc % 8))
								{
									for (i2 = 0; i2 < 8; i2++)
										for (j2 = 0; j2 < 8; j2++)
											T2[i2][j2] = T[i2][j2];

									move(T2, i * 8 + j % 8, vl * 8 + vc % 8);

									MoveGenerator(T2, 0 - tomove, csq1, csq2, cminscore2, cmaxscore2, depth - 1);
									if (cminscore2 > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cminscore2;
									}
								}
							}
						}
					}
				}
		}
	}
	sq1 = best_move_sq1;
	sq2 = best_move_sq2;
	minscore = cminscore;
	maxscore = cmaxscore;
}

int Eval(int T[10][10])
{
	int score = 0, l, c, piece, sq;
	for (l = 0; l<8; l++)
		for (c = 0; c < 8; c++)
		{
			piece = T[l][c];
			sq = l * 8 + c;
			if (piece != -1)
			{
				if (piece == pionalb || piece == pionnegru)
				{
					if (piece == pionalb)
						score += Piece_Value[piece]*Line_Value_White[l];
					if (piece == pionnegru)
						score += (-1)*Piece_Value[piece]*Line_Value_Black[l];
				}
				else
					score += Piece_Value[piece];
			}
			score += IsAttackedByBlack(T, sq)*SQ_Value_Black[sq];
			score += IsAttackedByWhite(T, sq)*SQ_Value_White[sq];
		}
	return score;
}

int ChooseDifficultyComputer1()
{
	int option = -1, quit = 0;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	SelectDifficultyComputer1.render(WindowPort);
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				option = IsInButton();
				if (option > 0)
					quit = 1;
			}
		}
	}
	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
				quit = 1;

		}
	}
	return option;
}

int ChooseDifficultyComputer2()
{
	int option = -1, quit = 0;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	SelectDifficultyComputer2.render(WindowPort);
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				option = IsInButton();
				if (option > 0)
					quit = 1;
			}
		}
	}
	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
				quit = 1;

		}
	}
	return option;
}

void DemoMode()
{

	int depth1 = ChooseDifficultyComputer1();
	int depth2 = ChooseDifficultyComputer2();
	if (depth1 != 4 && depth2 != 4)
	{
		SDL_RenderClear(gRenderer);
		Show_Background();
		Show_Board();
		Init_Table(Table);
		Show_Table(Table);

		char sir[5];
		int i, j, sq1 = -1, sq2 = -1, l1, c1, l2, c2, tomove = 1, piece, depth = 2, minscore, maxscore;
		bool quit = false;
		SDL_Event e;
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (x >= BackButton.x&&x <= BackButton.x + BackButton.w&&y >= BackButton.y&&y <= BackButton.y + BackButton.h)
						quit = 1;
				}
			}
			MoveGenerator(Table, tomove, sq1, sq2, minscore, maxscore, depth1);
			move(Table, sq1, sq2);
			if (depth1 == 1 || depth1 == 2)
				SDL_Delay(500);
			Show_Table(Table);
			tomove = 0 - tomove;
			if (ismate(Table, tomove))
			{
				quit = 1;
				if (tomove == 1)
					Show_Black_Win();
				if (tomove == -1)
					Show_White_Win();
				SDL_Delay(5000);
			}

			MoveGenerator(Table, tomove, sq1, sq2, minscore, maxscore, depth2);
			move(Table, sq1, sq2);
			if (depth2 == 1 || depth2 == 2)
				SDL_Delay(500);
			Show_Table(Table);
			tomove = 0 - tomove;
			if (ismate(Table, tomove))
			{
				quit = 1;
				if (tomove == 1)
					Show_Black_Win();
				if (tomove == -1)
					Show_White_Win();
				SDL_Delay(5000);
			}
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (x >= BackButton.x&&x <= BackButton.x + BackButton.w&&y >= BackButton.y&&y <= BackButton.y + BackButton.h)
						quit = 1;
				}
			}
		}
	}
	MainMenu();
}

void MainMenu()
{
	SDL_Event e;
	Show_Main_Menu();


	int option = -1;
	bool quit = 0;
	
	
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				option = IsInButton();
				if (option > 0)
					quit = 1;
			}
		}
	}
	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
				quit = 1;

		}
	}
	if (option == 1)
		vsPlayer();
	if (option == 2)
		vsComputer();
	if (option == 3)
		DemoMode();
}

int main(int argc, char* args[])
{
	init();
	SetButtonPort();
	Set_Piece_Value();
	Set_SQ_Value_Black();
	setSQPort();
	setWindowPort();
	loadMedia();
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	
	MainMenu();
	
	close();

	return 0;
}


// tabla incepe de la x=72p
//prima coloana incepe de la x=94p
//un camp e 66 x 66
//b7 SDL_Rect fillRect = { 169, 160, 50, 66 };