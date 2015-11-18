#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


#include "Bullet.h"
#include "ShaderProgram.h"
#include "SheetSprite.h"



class Enemy {
public:
	Enemy();
	Enemy(ShaderProgram* program, SheetSprite sprite, SheetSprite bulletSprite, float xposi, float yposi);
	void Update(const float &pxposi, const SheetSprite &psprite, Bullet &pbullet, bool &palive, int &ealive);
	const Uint8 *keys;

	ShaderProgram* program;
	Matrix modelMatrix;
	SheetSprite sprite;
	SheetSprite bulletSprite;

	float eTicks;
	float xposi;
	float yposi;
	float speed;
	bool alive;
	int bulletNum;

	Bullet bullet;

};