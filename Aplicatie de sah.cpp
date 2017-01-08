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

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect TableViewport;


const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int Table[10][10], WhiteControl[10][10], BlackControl[10][10];
int SQ_Value_White[64] = { 1,1,1,1,1,1,1,1, 2,1,2,2,2,2,2,2, 1,1,3,3,3,3,1,1, 1,2,3,3,3,3,2,1, 1,1,2,2,2,2,1,1, 1,1,1,1,1,1,1,1, 0, 0, 0, 0, 0, 0, 0, 0 };
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

LTexture TableTexture;
LTexture PieceTexture[12];
LTexture BackgroundTexture;
LTexture Main_Menu_Background;
LTexture Player_1_Image;
LTexture Player_2_Image;
LTexture Computer_Image;
LTexture Back_Image;
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
	BackgroundTexture.loadFromFile("Images/bg_water.png");
	Back_Image.loadFromFile("Images/back.png");
	Main_Menu_Background.loadFromFile("Images/mainmenu.png");
	Player_1_Image.loadFromFile("Images/player1.png");
	Player_2_Image.loadFromFile("Images/player2.png");
	Computer_Image.loadFromFile("Images/computer.png");
	TableTexture.loadFromFile("Images/tabla.png");
	
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
	piece = T[l1][c1];
	if (T[l1][c1] == regealb && l1==7)
		if (c2-c1 == 2)
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
	if (T[sq1 / 8][sq1 % 8] == regenegru&&l1==0)
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
	


	return captured;
}

void Init_Table(int T[10][10])
{
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



bool ismate(int T[10][10], int tomove)
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

void RenderToMouse(int p)
{
	SDL_Rect mouseport;
	int x, y;
	SDL_GetMouseState(&x, &y);
	mouseport.x = x;
	mouseport.y = y;
	mouseport.h = SQPort[0].h;
	mouseport.w = SQPort[0].w;
	if (p <= regenegru)
		PieceTexture[p].render(mouseport);
}

void Set_Piece_Value()
{
	Piece_Value[pionalb] = 15;
	Piece_Value[calalb] = 30;
	Piece_Value[nebunalb] = 35;
	Piece_Value[turnalb] = 50;
	Piece_Value[damaalb] = 90;
	Piece_Value[regealb] = 40;

	Piece_Value[pionnegru] = -15;
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

void Show_Player1()
{
	int i;
	SDL_Rect Player1_Port;
	Player1_Port.x = 720;
	Player1_Port.y = 575;
	Player1_Port.w = 280;
	Player1_Port.h = 70;
	SDL_RenderSetViewport(gRenderer, &Player1_Port);
	Player_1_Image.render(Player1_Port);
}

void Show_Player2()
{
	int i;
	SDL_Rect Player2_Port;
	Player2_Port.x = 720;
	Player2_Port.y = 72;
	Player2_Port.w = 280;
	Player2_Port.h = 70;
	SDL_RenderSetViewport(gRenderer, &Player2_Port);
	Player_2_Image.render(Player2_Port);
}

void Show_Computer()
{
	int i;
	SDL_Rect Computer_Port;
	Computer_Port.x = 720;
	Computer_Port.y = 72;
	Computer_Port.w = 280;
	Computer_Port.h = 70;
	SDL_RenderSetViewport(gRenderer, &Computer_Port);
	Computer_Image.render(Computer_Port);
}

void Show_Back()
{
	int i;
	SDL_Rect Back_Port;
	Back_Port.x = 72;
	Back_Port.y = 660;
	Back_Port.w = 140;
	Back_Port.h = 50;
	SDL_RenderSetViewport(gRenderer, &Back_Port);
	Back_Image.render(Back_Port);
}

void vsPlayer()
{
	SDL_RenderClear(gRenderer);
	Show_Background();
	Show_Board();
	Init_Table(Table);
	Show_Table(Table);
	Show_Player1();
	Show_Player2();
	Show_Back();
	//Show_Computer();


	char sir[5];
	int i, j, sq1 = -1, sq2 = -1, l1, c1, l2, c2, tomove = 1, piece;
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

											Table[sq1 / 8][sq1 % 8] = piece;
											if (tomove == 1)
												if (piece >= pionalb&&piece <= regealb && IsLegalMove(Table, sq1, sq2))
												{
													Table[sq1 / 8][sq1 % 8] = piece;
													move(Table, sq1, sq2);
													Show_Table(Table);
													tomove = 1 - tomove;
												}
												else;

											else
												if (tomove == 0)
													if (IsLegalMove(Table, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
													{
														Table[sq1 / 8][sq1 % 8] = piece;
														move(Table, sq1, sq2);
														Show_Table(Table);
														tomove = 1 - tomove;
													}
													else;
													if (ismate(Table, tomove))
													{
														quit = 1;
														cout << 0 - tomove << " wins\n";
														//Show window "-tomove wins" 
														SDL_Delay(3000);
														MainMenu();

													}

													//
										}
									}
							}
						}
						else;
				else
				{
					/*Back_Port.x = 72;
					Back_Port.y = 660;
					Back_Port.w = 140;
					Back_Port.h = 50;
					*/
					if (x >= 72 && x <= 72 + 140 && y >= 660 && y <= 660 + 50)
					{
						quit = 1;
						MainMenu();
						quit = 1;
					}
				}

			}

		}

	}
}


void Show_Main_Menu()
{
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	Main_Menu_Background.render(WindowPort);

}

void vsComputer()
{
	SDL_RenderClear(gRenderer);
	Show_Background();
	Show_Board();
	Init_Table(Table);
	Show_Table(Table);
	Show_Player1();
	Show_Computer();
	Show_Back();

	char sir[5];
	int i, j, sq1 = -1, sq2 = -1, l1, c1, l2, c2, tomove = 1, piece, depth = 2, player_colour = 1, minscore, maxscore;
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		if (tomove == player_colour)
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

												Table[sq1 / 8][sq1 % 8] = piece;
												if (tomove == 1)
													if (piece >= pionalb&&piece <= regealb && IsLegalMove(Table, sq1, sq2))
													{
														Table[sq1 / 8][sq1 % 8] = piece;
														move(Table, sq1, sq2);
														Show_Table(Table);
														tomove = 0 - tomove;
													}
													else;

												else
													if (tomove == 0)
														if (IsLegalMove(Table, sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
														{
															Table[sq1 / 8][sq1 % 8] = piece;
															move(Table, sq1, sq2);
															Show_Table(Table);
															tomove = 0 - tomove;
														}
														else;


														//
											}
										}
								}
							}
							else;
					else
					{
						/*Back_Port.x = 72;
						Back_Port.y = 660;
						Back_Port.w = 140;
						Back_Port.h = 50;
						*/
						if (x >= 72 && x <= 72 + 140 && y >= 660 && y <= 660 + 50)
						{
							quit = 1;
							MainMenu();
							quit = 1;
						}
					}


				}

			}
			if (ismate(Table, tomove))
			{
				quit = 1;
				cout << 0 - tomove << " wins\n";
				SDL_Delay(3000);

				//Show window "-tomove wins" 
				MainMenu();

			}

		}
		else
		{
			//make computer move
			sq1 = 2;
			sq2 = 5;
			MoveGenerator(Table, tomove, sq1, sq2, minscore, maxscore, 2);
			cout << sq1 << " to " << sq2 << "\n";
			move(Table, sq1, sq2);
			Show_Table(Table);
			tomove = 0 - tomove;
			if (ismate(Table, tomove))
			{
				quit = 1;
				cout << 0 - tomove << " wins\n";
				SDL_Delay(3000);

				//Show window "-tomove wins" 
				MainMenu();

			}
		}
	}
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

								cscore = Eval(T2);

								if (cscore < cminscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cscore;
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

								cscore = Eval(T2);

								if (cscore < cminscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cscore;
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

								cscore = Eval(T2);

								if (cscore < cminscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cscore;
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

								cscore = Eval(T2);

								if (cscore < cminscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cminscore = cscore;
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

									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
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

									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
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

									cscore = Eval(T2);

									if (cscore < cminscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cminscore = cscore;
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

								cscore = Eval(T2);
								if (cscore > cmaxscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cscore;
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

								cscore = Eval(T2);
								if (cscore > cmaxscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cscore;
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

								cscore = Eval(T2);
								if (cscore > cmaxscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cscore;
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

								cscore = Eval(T2);
								if (cscore > cmaxscore)
								{
									best_move_sq1 = i * 8 + j % 8;
									best_move_sq2 = vl * 8 + vc % 8;
									cmaxscore = cscore;
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

									cscore = Eval(T2);
									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
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

									cscore = Eval(T2);
									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
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

									cscore = Eval(T2);
									if (cscore > cmaxscore)
									{
										best_move_sq1 = i * 8 + j % 8;
										best_move_sq2 = vl * 8 + vc % 8;
										cmaxscore = cscore;
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
				score += Piece_Value[piece];
			}
			score += IsAttackedByBlack(T, sq)*SQ_Value_Black[sq];
			score += IsAttackedByWhite(T, sq)*SQ_Value_White[sq];
		}
	return score;
}

void MainMenu()
{
	SDL_Event e;
	Show_Main_Menu();

	int option = -1;
	bool quit = false;
	SDL_Rect zona1, zona2, zona3;

	zona1.x = 380;
	zona1.y = 254;
	zona1.h = 89;
	zona1.w = 306;

	zona2.x = 380;
	zona2.y = 377;
	zona2.h = 89;
	zona2.w = 306;

	zona3.x = 380;
	zona3.y = 501;
	zona3.h = 89;
	zona3.w = 306;

	while ((!quit) && (option == -1))
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);

				if ((x >= zona3.x&&x <= zona3.x + zona3.w) && (y >= zona3.y&&y <= zona3.y + zona3.h))
					quit = true;
				if ((x >= zona1.x&&x <= zona1.x + zona1.w) && (y >= zona1.y&&y <= zona1.y + zona1.h))
					option = 1;
				if ((x >= zona2.x&&x <= zona2.x + zona2.w) && (y >= zona2.y&&y <= zona2.y + zona2.h))
					option = 2;
			}
		}
	}
	if (option == 1)
		vsPlayer();
	if (option == 2)
		vsComputer();

}

int main(int argc, char* args[])
{
	init();
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