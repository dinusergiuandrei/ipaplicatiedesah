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

int Table[10][10], WhiteControl[10][10], BlackControl[10][10];
int dl[8] = {-1, 0, 1, 0, -1, 1, 1, -1};
int dc[8] = {0, 1, 0, -1, 1, 1, -1, -1};
int dcl[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
int dcc[8] = {1, 2, 2, 1, -1, -2, -2, -1};

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

SDL_Rect TableViewport;

void setSQPort();

void PresentTableControl();

int IsAttackedByWhite(int sq);

int IsAttackedByBlack(int sq);

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

enum MENU
{
	Main_Menu, vsPlayer, vsComputer, Analysis
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
	//SDL_RenderClear(gRenderer);
	//SDL_RenderSetViewport(gRenderer, &WindowPort);
	TableViewport.x = SCREEN_HEIGHT / 10;
	TableViewport.y = SCREEN_HEIGHT / 10;
	TableViewport.w = SCREEN_HEIGHT * 8 / 10;
	TableViewport.h = SCREEN_HEIGHT * 8 / 10;
	SDL_RenderSetViewport(gRenderer, &WindowPort);
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

void Show_Piece(int piece, int SQ)
{
	SDL_Rect port;
	port = SQPort[SQ];
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
	Table[sq1 / 8][sq1 % 8] = -1;
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
	//SDL_RenderClear(gRenderer);
	Show_Board();
	for (i = A8; i <= H1; i++)
		if (Table[i / 8][i % 8] >= 0)
			Show_Piece(Table[i / 8][i % 8], i);
}

bool IsValidMove(int sq1, int sq2)
{
	int valid = 1, culoare1=0, culoare2=-1, aux;
	int piece = Table[sq1 / 8][sq1 % 8], piece2=Table[sq2/8][sq2%8];
	int l1, c1, l2, c2, difl, difc, dl, dc, caux, laux;
	if(!(sq1>=0&&sq1<64&&sq2>=0&&sq2<64))
		return false;
	if (piece < 0)
		return false;
	if (sq1 == sq2)
		return false;
	
	l1 = sq1 / 8;
	c1 = sq1 % 8;
	l2 = sq2 / 8;
	c2 = sq2 % 8;


	difl = l2 - l1;
	if (difl < 0) 
		difl = difl*(-1);
	difc = c2 - c1;
	if (difc < 0)
		difc = difc*(-1);
	if (piece <= regealb)
		culoare1 = 1;
	else culoare1 = 0;
	if(piece2!=-1)
		if (piece2 <= regealb)
			culoare2 = 1;
		else culoare2 = 0;
	else culoare2 = -1;
	if (culoare1 == culoare2)
		return false;

	if (piece == pionalb)
	{
		
		if (difc > 1)
			return false;
		
		if (difl > 2)
			return false;
		
		if (l1 <= l2)
			return false;
		if (difl == 2)
		{
			if (c2 != c1)
				return false;

			if (l1 != 6 || l2 != 4)
				return false;

			if (Table[l1 - 1][c1] != -1 || Table[l2][c2] != -1)
				return false;

			return true;
		}
		if (difl == 1 && difc == 0)
		{
			if (c2 != c1)
				return false;

			if (Table[l2][c2] != -1)
				return false;
			
			return true;
		}
		if (difl == 1 && difc == 1)
		{
			

			if (Table[l2][c2] == -1 && Table[l2+1][c2]!=pionnegru )
				return false;
		

			return true;
		}
	}

	if (piece == pionnegru)
	{
		if (difc > 1)
			return false;

		if (difl > 2)
			return false;

		if (l1 >= l2)
			return false;

		if (difl == 2)
		{
			if (c2 != c1)
				return false;
			if (l1 != 1 || l2 != 3)
				return false;
			if (Table[l1 + 1][c1] != -1 || Table[l2][c2] != -1)
				return false;
			return true;
		}
		if (difl == 1 && difc == 0)
		{
			if (c2 != c1)
				return false;
			if (Table[l2][c2] != -1)
				return false;
			return true;
		}
		if (difl == 1 && difc == 1)
		{
			if (Table[l2][c2] == -1 && Table[l2 - 1][c2] != pionnegru)
				return false;
			return false;
		}
	}

	if (piece == calalb || piece == calnegru)
	{
		if (  (! (difc == 2 && difl == 1) ) && ( !(difc == 1 && difl == 2)  )   )
		{
			valid = 0;
		}
	}
	if (piece == nebunalb || piece == nebunnegru)
	{
		if (difc != difl)
			return false;
		if (difc == 0 || difl == 0)
			return false;
		dl = (l2 - l1) / difl;
		dc = (c2 - c1) / difc;
		laux = l1 + dl;
		caux = c1 + dc;
		while (laux != l2)
		{
			if (Table[laux][caux] != -1)
				return false;
			laux += dl;
			caux += dc;
		}
	}

	if (piece == turnalb || piece == turnnegru)
	{
		if (l2 > l1)
			dl = 1;
		else 
			if (l2 < l1)
				dl = -1;
			else dl = 0;
		if (c2 > c1)
			dc = 1;
		else
			if (c2 < c1)
				dc = -1;
			else dc = 0;
		laux = l1 + dl;
		caux = c1 + dc;
		while (laux != l2&&caux!=c2)
		{
			if (Table[laux][caux] != -1)
				return false;
			laux += dl;
			caux += dc;
		}
	}
	if (piece == damaalb || piece == damanegru)   // problema
	{
		if (!(difl == 0 || difc == 0 || difl == difc))
			return false;

		if (difl == 0)
		{
			cout << 2;
			caux = c1;
			laux = l1;
			dc = (c2 - c1) / difc;
			caux = c1 + dc;
			while (caux != c2)
			{
				if (Table[laux][caux] != -1)
					return false;
				caux += dc;
			}

			return true;
		}
		if (difc == 0)
		{
			caux = c1;
			laux = l1;
			dl = (l2 - l1) / difl;
			laux = l1 + dl;
			while (laux != l2)
			{
				if (Table[laux][caux] != -1)
				{
					return false;
				}
				laux += dl;
			}
			return true;
		}
		if (difc == difl)
		{

			dc = (c2 - c1) / difc;
			dl = (l2 - l1) / difl;
			laux = l1 + dl;
			caux = c1 + dc;
			while (laux != l2&&caux != c2)
			{
				if (Table[laux][caux] != -1)
					return false;
				laux += dl;
				caux += dc;
			}

			return true;
		}
	}
	
	if (piece == regealb || piece==regenegru)
	{
		if (!(difl == 0 || difc == 0 || difl == difc))
			return false;
		if (difl == 0)
		{
			caux = c1;
			laux = l1;
			dc = (c2 - c1) / difc;
			while (caux != c2)
			{
				if (Table[laux][caux] != -1)
					return false;
				caux += dc;
			}
		}
		if (difc == 0)
		{
			caux = c1;
			laux = l1;
			dl = (c2 - c1) / difl;
			while (laux != l2)
			{
				if (Table[laux][caux] != -1)
					return false;
				laux += dl;
			}
		}
		if (difc == difl)
		{
			dc = (c2 - c1) / difc;
			dl = (c2 - c1) / difl;
			laux = l1 + dl;
			caux = c1 + dc;
			while (laux != l2&&caux != c2)
			{
				if (Table[laux][caux] != -1)
					return false;
				laux += dl;
				caux += dc;
			}
		}
	}

	return valid;
}

bool IsLegalMove(int sq1, int sq2)
{
	if (!IsValidMove(sq1, sq2))
		return false;
	int p1, p2;
	p1 = Table[sq1 / 8][sq1 % 8];
	p2 = Table[sq2 / 8][sq2 % 8];
	Table[sq2 / 8][sq2 % 8] = Table[sq1 / 8][sq1 % 8];
	int BlackKingLine, BlackKingColumn, WhiteKingLine, WhiteKingColumn, i, j, piece=Table[sq1/8][sq1%8] ;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (Table[i][j] == regealb)
			{
				WhiteKingLine = i;
				WhiteKingColumn = j;
			}
			if (Table[i][j] == regenegru)
			{
				BlackKingColumn = j;
				BlackKingLine = i;
			}
		}
	}

	if (piece <= regealb)    // se acceseaza ba asta ba attackedby
		if (IsAttackedByBlack(WhiteKingLine * 8 + WhiteKingColumn % 8) > 0)
		{
			Table[sq1 / 8][sq1 % 8]=p1;
			Table[sq2 / 8][sq2 % 8]=p2;
			return false;
		}
	if (piece > regealb)
		if (IsAttackedByWhite(BlackKingLine * 8 + BlackKingColumn % 8) > 0)
		{
			Table[sq1 / 8][sq1 % 8] = p1;
			Table[sq2 / 8][sq2 % 8] = p2;
			return false;
		}
	Table[sq1 / 8][sq1 % 8] = p1;
	Table[sq2 / 8][sq2 % 8] = p2;
	return true;
}

int IsAttackedByWhite(int sq)
{
	int piece, i, j, attacked = 0, vl, vc, dir;

	for (i = 0; i<8; i++)
		for (j = 0; j < 8; j++)
		{
			piece = Table[i][j];
			if (piece == pionalb)
			{
				vl = i - 1;
				vc = j - 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
						attacked++;
				vc = j + 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
						attacked++;
			}
			if (piece == calalb)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dcl[dir];
					vc = j + dcc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
					}
				}
			}

		}

return attacked;
}

int IsAttackedByBlack(int sq)
{
	int piece, i, j, attacked = 0, dir, vl ,vc;

	for (i = 0; i<8; i++)
		for (j = 0; j < 8; j++)
		{
			piece = Table[i][j];
			if (piece == pionnegru)
			{
				vl = i + 1;
				vc = j - 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
						attacked++;
				vc = j + 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
						attacked++;
			}

			if (piece == calnegru)
			{
				for (dir = 0; dir < 8; dir++)
				{
					vl = i + dcl[dir];
					vc = j + dcc[dir];
					if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsValidMove(i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
					}
				}
			}
		}

return attacked;
}

void PresentTableControl()
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
			WhiteControl[i][j] = IsAttackedByWhite(i * 8 + i % 8);  
			BlackControl[i][j] = IsAttackedByBlack(i * 8 + i % 8);	
		}
	}
}



bool ismate(int tomove)
{
	int king, i, j, kingl, kingc, attacked=0, dir, vl, vc;
	if (tomove == 0)
		king = regealb;
	else king = regenegru;
	for(i=0; i<8; i++)
		for(j=0; j<8; j++)
			if (Table[i][j] == king)
			{
				kingl = i;
				kingc = j;
			}
	if (king == regealb)
		if (IsAttackedByBlack(kingl * 8 + kingc % 8) > 0)
			attacked = 1;
	if(king==regenegru)
		if (IsAttackedByWhite(kingl * 8 + kingc % 8) > 0)
			attacked = 1;
	if (!attacked)
		return false;
	for (dir = 0; dir < 8; dir++)
	{
		vl = kingl + dl[dir];
		vc = kingc + dc[dir];
		if (IsLegalMove(kingl * 8 + kingc % 8, vl * 8 + vc % 8) == 1)
			return false;
	}
	return true;

}

int PvsP()
{
	int tomove, mate, quit, sq1, sq2;
	SDL_Event e;
	tomove = 0;
	mate = 0;
	quit = 0;
	while (!quit&&!mate)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//input
			
			cin >> sq1 >> sq2;


			if (sq1 != -1 && IsLegalMove(sq1, sq2))
			{
				move(sq1, sq2);
				mate = ismate(tomove);
				PresentTableControl();
				tomove = -1 - tomove;
				Show_Table();
			}
			if (mate)
				break;
		}

	}

	return 1 - tomove;   
	//returneaza castigatorul    
	//   alb=0
	//  negru=-1;

}


void MainMenu()
{
	
}

int main(int argc, char* args[])
{
	char sir[5];
	int i, j, sq1, sq2, l1, c1, l2, c2, tomove=1, piece;
	init();
	loadMedia();
	bool quit = false;
	SDL_Event e;
	setSQPort();
	setWindowPort();
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);
	Show_Background();
	Show_Board();
	
	SDL_RenderSetViewport(gRenderer, &WindowPort);
	Init_Table();
	Show_Table();
	i = 0;
	while (!quit)
		{
				while (SDL_PollEvent(&e) != 0)
				{
					Show_Background();
					Show_Table();
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					cout << "\nStarting SQ : ";
					cin >> sir;
					c1 = sir[0] - 'a';
					l1 = sir[1] - '0';
					l1 = 8 - l1;
					sq1 = l1 * 8 + c1 % 8;
					cout << "\nFinal SQ : ";
					cin >> sir;
					c2 = sir[0] - 'a';
					l2 = sir[1] - '0';
					l2 = 8 - l2;
					sq2 = l2 * 8 + c2 % 8;
					//cout << l1 << " " << c1 << " " << l2 << " " << c2 << "\n";
					piece = Table[l1][c1];
					if(tomove==1)
						if (piece >= pionalb&&piece <= regealb && IsLegalMove(sq1, sq2))
					{
						//cout << l1 << " " << c1 << " " << l2 << " " << c2 << "\n";
						move(sq1, sq2);
						Show_Table();
						tomove = 1 - tomove;
					}
						else cout << "\nMutare invalida\n";
	
					else
						if(tomove==0)
							if (IsLegalMove(sq1, sq2)&&(piece >= pionnegru&&piece <= regenegru))
							{
								move(sq1, sq2);
								Show_Table();
								tomove = 1 - tomove;
							}
							else cout << "\nMutare invalida\n";

					
				}
				
			}
	
	close();
	
	return 0;
}


// tabla incepe de la x=72p
//prima coloana incepe de la x=94p
//un camp e 66 x 66
//b7 SDL_Rect fillRect = { 169, 160, 50, 66 };