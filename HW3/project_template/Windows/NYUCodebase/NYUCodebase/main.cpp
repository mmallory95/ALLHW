/*

Name: Malcolm Mallory
ID: N15265745
Class: CS 3113


*/
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "ShaderProgram.h"
#include "Matrix.h"

//HW3
#include "SheetSprite.h"
#include "Player.h"
#include "Enemy.h"



#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
//Textures and Loading Images
GLuint LoadTexture(const char *image_path){

	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// IF the below line generates an error, re make the image using photoshop

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Do not use both Clamp and Repeat at once!

	/*
	//For repeating textures that exceed 0-1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Clamping: Extends the last pixel of the texture onward
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	*/


	SDL_FreeSurface(surface);

	return textureID;
}

GLuint LoadBackground() {
	SDL_Surface * surface = IMG_Load(RESOURCE_FOLDER"red.png");
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void DrawBackground(ShaderProgram* program, GLuint texID) {
	float vertices[] = { -1.2f, -1.2f, 1.2f, 1.2f, -1.2f, 1.2f, 1.2f, 1.2f, -1.2f, -1.2f, 1.2f, -1.2f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void CreateText(ShaderProgram* program, int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0f / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void DrawSprite(GLint texture, float x, float y, float rotation, float scale = 1.0) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);
	glScalef(scale, 1.0, 1.0);

	GLfloat tex[] = { -0.2f, 0.2f, -0.2f, -0.2f, 0.2f, -0.2f, 0.2f, 0.2f };
	GLfloat texUVs[] = { 0.0, 0.0, 0.0, 0.8, 0.8, 0.8, 0.8, 0.0 };
	glVertexPointer(2, GL_FLOAT, 0, tex);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

//HW2
/*
float ballStart(){
	int remainder = rand() % 2 + 1;
	if (remainder == 1) return 1.0f;
	return -1.0f;
}
//End HW2
*/

//HW3
enum GameState { MAIN_MENU, IN_LEVEL, GAME_OVER };
int thestate;

//End HW3

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 640, 360);


	//For No Textures used
	/*
	//ShaderProgram program = ShaderProgram("vertex.glsl", "fragment.glsl");
	*/

	//For Textures used, HW1, HW2
	/*
	//ShaderProgram program = ShaderProgram("vertex_textured.glsl", "fragment_textured.glsl");
	*/

	//For HW3
	ShaderProgram* program = new ShaderProgram("vertex_textured.glsl", "fragment_textured.glsl");

	//Sets clear color of screen. (0.0f, 0.0f, 0.0f, 1.0f) is black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Black Color

	//Base Matrices
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	//HW2
	/*
	Matrix modelMatrixleftpaddle;
	Matrix modelMatrixrightpaddle;
	Matrix modelMatrixBall;
	Matrix modelMatrixBorder;
	*/


	//HW3
	Matrix modelMatrixMain;
	Matrix modelMatrixGameOver;



	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	//Declare variables outisde of event loop
	float lastFrameTicks = 0.0f;

	float angle = 0.0f;

	GLuint redTextureID = LoadTexture(RESOURCE_FOLDER"red.png");
	GLuint blueTextureID = LoadTexture(RESOURCE_FOLDER"blue.png");
	GLuint yellowTextureID = LoadTexture(RESOURCE_FOLDER"yellow.png");
	GLuint purpleTextureID = LoadTexture(RESOURCE_FOLDER"purple.png");
	GLuint greenTextureID = LoadTexture(RESOURCE_FOLDER"green.png");
	GLuint orangeTextureID = LoadTexture(RESOURCE_FOLDER"orange.png");

	//HW2 Paddle/Ball Attributes
	/*
	float paddleLength = 0.6f;
	float paddleWidth = 0.075f;
	float leftpaddleY = 0.0f;
	float leftpaddleX = 0.0f;
	float rightpaddleY = 0.0f;
	float rightpaddlex = 0.0f;

	float ballPosiX = 0.0f;
	float ballPosiY = 0.0f;
	float ballDirectX = 0.0f;
	float ballDirectY = 0.0f;
	float ballSpeed = 1.0f;
	float ballSize = 0.1f;
	*/

	//HW3
	std::vector<Enemy> enemies;
	int ealive;

	//Reading Given Sprite Sheet
	GLuint spriteSheet = LoadTexture(RESOURCE_FOLDER"sheet.png");
	GLuint fontSheet = LoadTexture(RESOURCE_FOLDER"font1.png");
	//Player
	SheetSprite playerSprite(program, spriteSheet, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.15, GL_CLAMP);
	SheetSprite playerBullet(program, spriteSheet, 856.0f / 1024.0f, 421.0f / 1024.0f, 9.0f / 1024.0f, 54.0f / 1024.0f, 0.1, GL_CLAMP);

	//Enemies
	SheetSprite enemySprite1(program, spriteSheet, 423.0f / 1024.0f, 728.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.15, GL_CLAMP);
	SheetSprite enemySprite2(program, spriteSheet, 143.0f / 1024.0f, 293.0f / 1024.0f, 104.0f / 1024.0f, 84.0f / 1024.0f, 0.15, GL_CLAMP);
	SheetSprite enemySprite3(program, spriteSheet, 224.0f / 1024.0f, 496.0f / 1024.0f, 103.0f / 1024.0f, 84.0f / 1024.0f, 0.15, GL_CLAMP);
	SheetSprite enemySprite4(program, spriteSheet, 520.0f / 1024.0f, 577.0f / 1024.0f, 82.0f / 1024.0f, 84.0f / 1024.0f, 0.15, GL_CLAMP);
	SheetSprite enemyBullet(program, spriteSheet, 856.0f / 1024.0f, 983.0f / 1024.0f, 9.0f / 1024.0f, 37.0f / 1024.0f, 0.1, GL_CLAMP);
	
	GLuint backgroundTexture = LoadBackground();

	// Unit Generation
	Player player(program, playerSprite, playerBullet, 0.0f, -0.95f, 1.0f);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 6; j++) {
			if (i == 0){
				Enemy enemy(program, enemySprite1, enemyBullet, -0.66f + (j * 0.28f) - 0.04f, 1.0f);
				enemies.push_back(enemy);
			}
			else if (i == 1){
				Enemy enemy(program, enemySprite2, enemyBullet, -0.66f + (j * 0.28f) - 0.04f, 0.65f);
				enemies.push_back(enemy);
			}
			else if (i == 2){
				Enemy enemy(program, enemySprite3, enemyBullet, -0.66f + (j * 0.28f) - 0.04f, 0.3f);
				enemies.push_back(enemy);
			}
			else if (i = 3){
				Enemy enemy(program, enemySprite4, enemyBullet, -0.66f + (j * 0.28f) - 0.04f, -0.05f);
				enemies.push_back(enemy);
			}
		}
	}
	ealive = enemies.size();

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			/*
			else if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					//make character jump
					//character should be affected by gravity
				}

			}

			else if (event.type == SDL_MOUSEMOTION){
				//event.motion.x is our new x
				//float unitX = (((float)event.motion.x / 640.0f) * 3.554f) - 1.777f;

				//event.motion.y is our new y
				//float unitY = (((float)(360-event.motion.y) / 360.0f) * 2.0f) - 1.0f;
			}

			else if (event.type == SDL_MOUSEBUTTONDOWN){
				//event.button.x = click x position
				//event.button.y = click y position
				//event.button.button is the mouse button clicked (1,2,3,etc.)
			}
			*/
		}

		const Uint8 *keys = SDL_GetKeyboardState(NULL); //returns an array of keyboard keys, 1 if key is pressed

		//Polling Input
		/*
		if (keys[SDL_SCANCODE_LEFT]) {
			//MOVE CHARACTER LEFT/ROTATE LEFT
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			//MOVE CHARACTER RIGHT/ROTATE RIGHT
		}

		else if (keys[SDL_SCANCODE_UP]){
			//MOVE CHARACTER UP
		}

		else if (keys[SDL_SCANCODE_DOWN]){
			//MOVE CHARACTER DOWN
		}
		*/

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		//Base, HW1, HW2
		/*
		program.setModelMatrix(modelMatrix);
		program.setProjectionMatrix(projectionMatrix);
		program.setViewMatrix(viewMatrix);
		*/

		//HW3
		program->setModelMatrix(modelMatrix);
		program->setProjectionMatrix(projectionMatrix);
		program->setViewMatrix(viewMatrix);
		//End HW3


		glClear(GL_COLOR_BUFFER_BIT);
		//HW3
		glUseProgram(program->programID);
		//End HW3


		//Base, HW1, HW2
		/*
		glUseProgram(program.programID);
		*/

		//HW3 Space Invaders
		switch (thestate)
		{
		case MAIN_MENU:
			program->setModelMatrix(modelMatrixMain);
			modelMatrixMain.identity();
			modelMatrixMain.Translate(-0.9f, 0.0f, 0.0f); //What does this do?

			CreateText(program, fontSheet, "Press Enter!", 0.25f, 0.005f);
			if (keys[SDL_SCANCODE_RETURN]){
				thestate = IN_LEVEL;
			}
			break;

		case IN_LEVEL:
			if (player.alive && ealive > 0){
				player.Update();
				for (int i = 0; i < 24; i++) 
					enemies[i].Update(player.xposi, player.sprite, player.bullet, player.alive, ealive);
			}
			else { 
				thestate = GAME_OVER;
			}
			break;

		case GAME_OVER:
			program->setModelMatrix(modelMatrixGameOver);
			modelMatrixGameOver.identity();
			modelMatrixGameOver.Translate(-0.9f, 0.0f, 0.0f);

			CreateText(program, fontSheet, "Press Enter To Quit!", 0.25f, 0.0005f);
			if (keys[SDL_SCANCODE_RETURN]){
				done = true;
			}
			break;
		}

		//HW2 Pong
		/*

		//HW2 Pong

		//Left Paddle
		program.setModelMatrix(modelMatrixleftpaddle);

		if (keys[SDL_SCANCODE_W] && leftpaddleY < 0.8f) { leftpaddleY += elapsed; } // Up
		else if (keys[SDL_SCANCODE_S] && leftpaddleY > -0.8f){ leftpaddleY -= elapsed; } // Down

		float leftvertices[] = { 
		    -paddleWidth / 2, -paddleLength / 2,
			-paddleWidth / 2, paddleLength / 2,
			paddleWidth / 2, paddleLength / 2,
			-paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, paddleLength / 2 
			};

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftvertices);
		glEnableVertexAttribArray(program.positionAttribute);


		modelMatrixleftpaddle.identity();
		modelMatrixleftpaddle.Translate(-1.652f, leftpaddleY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Right Paddle
		program.setModelMatrix(modelMatrixrightpaddle);

		if (keys[SDL_SCANCODE_UP] && rightpaddleY < 0.8f) { rightpaddleY += elapsed; } // Up
		else if (keys[SDL_SCANCODE_DOWN] && rightpaddleY > -0.8f){ rightpaddleY -= elapsed; } // Down

		float rightvertices[] = { 
		    -paddleWidth / 2, -paddleLength / 2, 
			-paddleWidth / 2, paddleLength / 2,
			paddleWidth / 2, paddleLength / 2,
			-paddleWidth / 2, -paddleLength / 2,
			paddleWidth / 2, -paddleLength / 2, 
			paddleWidth / 2, paddleLength / 2 
			};

		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		modelMatrixrightpaddle.identity();
		modelMatrixrightpaddle.Translate(1.652f, rightpaddleY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Ball
		program.setModelMatrix(modelMatrixBall);

		if (keys[SDL_SCANCODE_SPACE] && ballDirectX == 0 && ballDirectY == 0)
		{
			ballDirectX = ballStart();
			ballDirectY = ballStart();
		}

		ballPosiX += elapsed * ballSpeed * ballDirectX;
		ballPosiY += elapsed * ballSpeed * ballDirectY;
		
		if (ballPosiY <= -0.975f) { ballDirectY = 1.0f; } // Ball impacting bottom
		else if (ballPosiY >= 0.975f) { ballDirectY = -1.0f; } // Ball impacting top

		// Ball impacting left paddle
		if (-1.65f + paddleWidth / 2 >= ballPosiX - ballSize / 2 &&
			leftpaddleY - paddleLength / 2 <= ballPosiY - ballSize / 2 &&
			leftpaddleY + paddleLength / 2 >= ballPosiY + ballSize / 2) 
		{
			ballDirectX = 1.0f; //reverse towards right
		} 

		// Ball impact right paddle - checks for 
		if (1.65f - paddleWidth / 2 <= ballPosiX + ballSize / 2 &&
			rightpaddleY - paddleLength / 2 <= ballPosiY - ballSize / 2 &&
			rightpaddleY + paddleLength / 2 >= ballPosiY + ballSize / 2) 
		{
			ballDirectX = -1.0f; //reverse towards left
		}

		// Ball impacting goal
		if (ballPosiX >= 1.752f || ballPosiX <= -1.752) {
			ballPosiX = 0.0f; ballPosiY = 0.0f; ballDirectX = 0.0f; ballDirectY = 0.0f;
			leftpaddleY = 0.0f;  rightpaddleY = 0.0f; //Resets the paddles
		} 

		float ballvertices[] = { 
			-ballSize / 2, -ballSize / 2, 
			ballSize / 2, ballSize / 2, 
			-ballSize / 2, ballSize / 2,

			-ballSize / 2, -ballSize / 2, 
			ballSize / 2, -ballSize / 2, 
			ballSize / 2, ballSize / 2 
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballvertices);
		glEnableVertexAttribArray(program.positionAttribute);

		modelMatrixBall.identity();
		modelMatrixBall.Translate(ballPosiX, ballPosiY, 0.0f);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Border?

		*/


		//HW1 Textures
		/*
		//Blue Box
		modelMatrix.identity();


		program.setModelMatrix(modelMatrix);

		float iconvertices2[] = {
			3.56f, 2.0f, //top right 
			-3.56f, 2.0f, //top left
			-3.56f, -2.0f, //bottom left
			3.56f, -2.0f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices2);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords2[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, blueTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//yellow Box
		modelMatrix.identity();
		modelMatrix.Rotate(-ticks * ticks);

		program.setModelMatrix(modelMatrix);

		float iconvertices1[] = {
			0.0f, 0.0f, //top right 
			-1.0f, 0.0f, //top left
			-1.0f, -1.0f, //bottom left
			0.0f, -1.0f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices1);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords1[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords1);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, yellowTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);



		//Red Box
		modelMatrix.identity();
		modelMatrix.Rotate(ticks);

		program.setModelMatrix(modelMatrix);

		float iconvertices3[] = { 
			0.5f, 0.5f, //top right 
			-0.5f, 0.5f, //top left
			-0.5f, -0.5f, //bottom left
			0.5f, -0.5f }; //bottom right
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, iconvertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		float iconTexCoords3[] = { 1.5f, 1.5f, -1.5f, 1.5f, -1.5f, -1.5f, 1.5f, -1.5f};
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, iconTexCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);




		glBindTexture(GL_TEXTURE_2D, redTextureID);
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);
		*/
		
		//HW1 No Textures
		/* //HW1 no textures
		modelMatrix.identity();
		modelMatrix.Rotate(ticks);
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices[] = {
			-0.5f, -0.5f, //Bottom Left
			0.5f, -0.5f, // bottom right
			0.5f, 0.5f, //Top right
			-0.5f, 0.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		modelMatrix.identity();
		modelMatrix.Rotate(-ticks);
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices2[] = {
			-0.5f, -0.5f, //Bottom Left
			-1.5f, -1.5f, // bottom right
			-1.5f, 0.5f, //Top right
			-0.5f, 0.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		modelMatrix.identity();
		modelMatrix.Rotate((ticks * ticks));
		program.setModelMatrix(modelMatrix);

		//Triangle Declaration
		float vertices3[] = {
			0.5f, 0.0f, //Bottom Left
			1.5f, 0.0f, // bottom right
			1.5f, 1.5f, //Top right
			1.0f, 1.5f //top left
		};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		//Triangle Drawing
		glDrawArrays(GL_QUADS, 0, 4);

		glDisableVertexAttribArray(program.positionAttribute);

		*/

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

//Vertex Shader

//Vertex Shader is where the model, view, and projection matrices are applied to the vertex
//Runs numerous times a second, can pass vertex color to shaders
//Animation is from the vertex shader; entities are skeletal meshes that are then animated

//Fragment Shader

//Fragment Shader is where the returns the color of each pixel when the geometry is rasterized on the GPU
//Fairly complicated, running for every single pixel, really fast. Has to run on GPU
//Rasterizing
//Responsible for determining the final pixel seen on the screen.
//Gloom and lighting is determined by Fragment Shader

//Shader Program

//Shader Program is a combination of a vertex and a fragment shader.
//Written in GLSL; openGL Shading Language

//Circle-Circle

/*
//If the distance between two circles is less or equal to the sum of their
//radii, they are colliding.
*/

//Circle-Point
/*
//If the distance between a circle and a point is less than the circle's radius
//they are colliding.
*/

//Box-Box
/*
//if any of the following are true, not intersecting

//R1's Bottom is higher than R2's Top - Case 1
//R1's Top is lower than R2's Bottom - Case 2
//R1's left is larger than R2's Right - Case 3
//R1's right is smaller than R2's left - Case 4
*/

//Box-Point
/*
// If point x is larger than box left and smaller than box right
// If point y is larger than box bottom and smaller than box top
*/

//Paddle
//y_position += elapsed * distance_to_travel_in_one_second

//Ball
//position.y += sin(angle) * elapsed * units_a_second
//position.x += cos(angle) * elapsed * units a second


//Steps of developing AI
/*
Sensing the Player
Sensing the World

Pathfinding (Main part of AI)

States of AI Entities
Wiggler from Mario Bros (Normal or Angry)
GRU Soldiers from MGS (Normal, Combat, Suspiscious, Held Up, Asleep, Dead, etc)

Collision Flags
Simplest example: AI character turns around upon hitting a wall. 
	-When the X-axis collision flag is set, reverse the X acceleration
	-Also possibly reverse the enemy's sprite

Turning Around at an edge
	-can't use flags anymore. Use collision detector point. 
	-A detector point is part of the entity, but is one unit ahead. If the Detector point
	collides with nothing and the bottom collision flag is set to true then you're on an edge
	-You can also use a sensor (detector) point to see if you can jump accross a gap.
	-You can use two or three sensor points to jump up a platform that's coming above you (but isn't yet).
	-Or you can use them to see if a fall is safe
	-You can use them on non-axis aligned collisions
	-You can adjust the sensor distance

	In 3D games your character uses a Ray from the Camera's point of view to interact with the first object 
	it collides with (with a certain maximum distance).

Simulating Senses
	-Sensor Range (usually a circle around an entity that detects (or somethings) the player when it enters)
	-Vision Cone (simulates vision, uses atan2 and rotation between the entity and the player. If the absolute
	value of the difference is larger than half the vision angle you can't see the object). 
	-Vision Obstacles (stuff that obscures vission like a wall. Do a ray test from the current position to the
	player, and if there is something in between, you can't see the player. 

	Finite State Machines are used for states. 
	-Can only be in one predefined state at a time. Best conceptualized via flow charts.


*/