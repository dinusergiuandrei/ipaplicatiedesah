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
int SQ_Value_White[64] = { 5,8,8,6,6,8,8,8, 6,7,7,6,6,7,7,6, 2,3,4,5,5,4,4,2, 1,2,4,4,4,4,3,2, 1,2,3,3,3,3,3,2, 1,1,2,2,2,3,3,3, 1,1,1,1,1,1,1,1 };
int Piece_Value[20], SQ_Value_Black[64];
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

int Eval();

void MoveGenerator(int T[8][8], int tomove, int &sq1, int &sq2, int &minscore, int &maxscore, int depth);


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
	Back_Image.loadFromFile("Images/back.png");
	Main_Menu_Background.loadFromFile("Images/mainmenu.png");
	Player_1_Image.loadFromFile("Images/player1.png");
	Player_2_Image.loadFromFile("Images/player2.png");
	Computer_Image.loadFromFile("Images/computer.png");
	cout<<BackgroundTexture.loadFromFile("Images/bg_water.png")<<"\n";
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
	if(Table[sq1/8][sq1%8]==regealb)
		if (sq2 % 8 - sq1 % 8 == 2)
		{
			Table[7][7] = -1;
			Table[7][5] = turnalb;
		}
		else
			if (sq1 % 8 - sq2 % 8 == 2)
			{
				Table[7][0] = -1;
				Table[7][3] = turnalb;
			}
	if (Table[sq1 / 8][sq1 % 8] == regenegru)
		if (sq2 % 8 - sq1 % 8 == 2)
		{
			Table[0][7] = -1;
			Table[0][5] = turnnegru;
		}
		else
			if (sq1 % 8 - sq2 % 8 == 2)
			{
				Table[0][0] = -1;
				Table[0][3] = turnnegru;
			}
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
			if (Table[l2][c2] == -1 && Table[l2 - 1][c2] != pionalb)
				return false;
			return true;
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
		if (!(difl == 0 || difc == 0 || difl == difc))
			return false;

		if (difl == 0)
		{

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
			return false;
		return true;
	}
	if (piece == damaalb || piece == damanegru)   // problema
	{
		if (!(difl == 0 || difc == 0 || difl == difc))
			return false;

		if (difl == 0)
		{
			
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
		if (difc == 2||difc==3)
		{
			if (difl > 0)
				return false;
			if (piece == regealb)
			{
				if (sq1 / 8 != 7 && sq1 % 8 != 4)
					return false;
				if (sq2 != 7 * 8 + 6 && sq2 != 7 * 8 + 2)
					return false;
				if (sq2 == 7 * 8 + 6)
				{
					if (Table[7][7] != turnalb)
						return false;
					if (Table[7][5] != -1 || Table[7][6] != -1)
						return false;
					return true;
				}
				if (sq2 == 7 * 8 + 2)
				{
					if (Table[7][0] != turnalb)
						return false;
					if (Table[7][2] != -1 || Table[7][3] != -1 || Table[7][1] != -1)
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
					if (Table[0][7] != turnnegru)
						return false;
					if (Table[0][5] != -1 || Table[0][6] != -1)
						return false;
					return true;
				}
				if (sq2 == 2)
				{
					if (Table[0][0] != turnnegru)
						return false;
					if (Table[0][1] != -1 || Table[0][2] != -1 || Table[0][3] != -1)
						return false;
					return true;
				}
			}
		}
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
					while (vl >= 0 && vl < 8 && vc >= 0 && vc < 8 )
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

void RenderToMouse(int p)
{
	SDL_Rect mouseport;
	int x, y;
	SDL_GetMouseState(&x, &y);
	mouseport.x = x;
	mouseport.y = y;
	mouseport.h = SQPort[0].h;
	mouseport.w = SQPort[0].w;
	if(p<regenegru)
	PieceTexture[p].render(mouseport);
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
	for(l=0; l<8; l++)
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
	Player1_Port.y = 72;
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
	Player2_Port.y = 575;
	Player2_Port.w = 280;
	Player2_Port.h = 70;
	SDL_RenderSetViewport(gRenderer, &Player2_Port);
	Player_2_Image.render(Player2_Port);
}

void vsPlayer()
{
	SDL_RenderClear(gRenderer);
	Show_Background();
	Show_Board();
	Init_Table();
	Show_Table();
	Show_Player1();
	Show_Player2();
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
								
								Show_Table();
								
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
												if (piece >= pionalb&&piece <= regealb && IsLegalMove(sq1, sq2))
												{
													Table[sq1 / 8][sq1 % 8] = piece;
													move(sq1, sq2);
													Show_Table();
													tomove = 1 - tomove;
												}
												else;

											else
												if (tomove == 0)
													if (IsLegalMove(sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
													{
														Table[sq1 / 8][sq1 % 8] = piece;
														move(sq1, sq2);
														Show_Table();
														tomove = 1 - tomove;
													}
													else;


													//
										}
									}
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
	Init_Table();
	Show_Table();
	char sir[5];
	int i, j, sq1, sq2, l1, c1, l2, c2, tomove = 1, piece;
	bool quit = false;
	SDL_Event e;
	i = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
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

			piece = Table[l1][c1];
			if (tomove == 1)
				if (piece >= pionalb&&piece <= regealb && IsLegalMove(sq1, sq2))
				{

					move(sq1, sq2);
					Show_Table();
					tomove = 1 - tomove;
				}
				else cout << "\nMutare invalida\n";

			else
				if (tomove == 0)
					if (IsLegalMove(sq1, sq2) && (piece >= pionnegru&&piece <= regenegru))
					{
						move(sq1, sq2);
						Show_Table();
						tomove = 1 - tomove;
					}
					else cout << "\nMutare invalida\n";


		}

	}
}


void MoveGenerator(int T[8][8], int tomove, int &sq1, int &sq2, int &minscore, int &maxscore, int depth)
{
	int piece, i, j, attacked = 0, dir, vl, vc, cmaxscore, cminscore, sq;
	//codul de la attacked by
	//pentru fiecare mutare posibila
	for (i = 0; i<8; i++)
		for (j = 0; j < 8; j++)
		{
			piece = T[i][j];
			sq = i * 8 + j;
			if (piece == pionnegru)
			{
				vl = i + 1;
				vc = j - 1;
				if (vl >= 0 && vl < 8 && vc >= 0 && vc < 8)
					if (vl * 8 + vc % 8 == sq)
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
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
						if (IsLegalMove(i * 8 + j % 8, vl * 8 + vc % 8))
							if (vl * 8 + vc % 8 == sq)
								attacked++;
					}
				}
			}
		}

}

int Eval(int T[8][8] )
{
	int score = 0, l, c, piece, sq;
	for(l=0; l<8; l++)
		for (c = 0; c < 8; c++)
		{
			piece = T[l][c];
			sq = l * 8 + c;
			if (piece != -1)
			{
				score += Piece_Value[piece];
			}
			score += IsAttackedByBlack(sq)*SQ_Value_Black[sq];
			score += IsAttackedByWhite(sq)*SQ_Value_White[sq];
		}
	return score;
}

void MainMenu()
{
	SDL_Event e;
	Show_Main_Menu();
	
	int option=-1;
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

	while ((!quit)&&(option==-1))
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