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

#include "ShaderProgram.h"
#include "SheetSprite.h"


class Bullet {
public:
	Bullet();
	Bullet(ShaderProgram* program, SheetSprite sprite);
	void Update(float elapsed);

	Matrix modelMatrix;
	ShaderProgram* program;
	SheetSprite sprite;

	float xposi;
	float yposi;
	float speed;
	float direction;

	bool alive;


};