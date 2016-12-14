#pragma once


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
	void render(int x, int y);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	int opmat(int *v, int n)
	{
		this->vect = v;
		this->size = n;
	}

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
	int *vect;
	int size;
};